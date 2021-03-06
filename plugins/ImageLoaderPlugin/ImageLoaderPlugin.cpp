#include <cstdio>

#include <3rdparty/libjpeg/jpeglib.h>
#include <3rdparty/libjpeg/jerror.h>
//#include "libpng/png.h"

#include <FreeWorldEngine.h>

#include "ImageLoaderPlugin.h"

namespace FreeWorldEngine {

ImageLoaderPlugin::ImageLoaderPlugin()
{
}

ImageLoaderPlugin::~ImageLoaderPlugin()
{
}

std::string ImageLoaderPlugin::name() const
{
	return std::string("ImageLoaderPlugin");
}

std::string ImageLoaderPlugin::info() const
{
	return std::string("Image Manager Plugin");
}

bool ImageLoaderPlugin::initialize()
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	pCore->imageLoader()->registerDataLoader("jpg", loadJPEGFile);
	pCore->imageLoader()->registerDataLoader("jpeg", loadJPEGFile);

	return true;
}

void ImageLoaderPlugin::deinitialize()
{
	ICore *pCore = getCoreEngine();

}

Raster *ImageLoaderPlugin::loadJPEGFile(const std::string& filename)
{
	FILE *file;
	if (!(file = fopen(filename.c_str(), "rb")))
		return 0; // �� ������� ������� ����

	unsigned char *rowptr[1];
	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;

	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info);

	if (!file)
		return 0; // ������ ������ �����

	jpeg_stdio_src(&info, file);
	jpeg_read_header(&info, TRUE);
	jpeg_start_decompress(&info);
	uint32 width = info.output_width;
	uint32 height = info.output_height;
	uint32 numComponents = info.num_components;
	Type type = TYPE_UNSIGNED_INT_8;
	unsigned char *data = (unsigned char*)malloc(width * height * numComponents);
	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char*)data + numComponents * info.output_width * (info.output_height - info.output_scanline - 1);
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	jpeg_finish_decompress(&info);

	Raster *pRaster = new Raster;
	pRaster->size[0] = width;
	pRaster->size[1] = height;
	pRaster->numComponents = numComponents;
	pRaster->type = type;
	pRaster->pData = data;

	return pRaster;
}

Raster *ImageLoaderPlugin::loadPNGFile(const std::string& filename)
{
	/*png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, bit_depth;
	FILE *fp;

	if ((fp = fopen(filename.c_str(), "rb")) == NULL)
		return false;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

	depth = 1;

	switch (bit_depth) {
	case 8: { type = TYPE_UNSIGNED_INT_8; break; }
	case 16: { type = TYPE_UNSIGNED_INT_16; break; }
	case 32: { type = TYPE_UNSIGNED_INT_32; break; }
	default: {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}
	}

	switch (color_type) {
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA: { numComponents = 1; break; }
	case PNG_COLOR_TYPE_RGB: { numComponents = 3; break; }
	case PNG_COLOR_TYPE_RGBA: { numComponents = 4; break; }
	default: {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}
	};

	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	data = new uint8[numComponents * width * height];

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for (uint32 i = 0; i < height; ++i)
		memcpy((uint8*)data + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);*/
	return 0;
}

} // namespace