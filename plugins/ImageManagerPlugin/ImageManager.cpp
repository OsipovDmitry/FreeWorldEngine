#include <FreeWorldEngine.h>
#include <algorithm>

#include "Image.h"
#include "ImageManager.h"

#include "libjpeg/jpeglib.h"
#include "libjpeg/jerror.h"

#include "libpng/png.h"

namespace FreeWorldEngine {

ImageManager::ImageManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForImages"))
{
}

ImageManager::~ImageManager()
{
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

IImage *ImageManager::loadImage(const std::string& filename)
{
	IImage *pImage = getByName(filename);
	if (pImage)
		return pImage;

	uint32 imageWidth, imageHeight, imageDepth, imageNumComponents;
	Type imageType;
	void *imageData;
	std::string fileExt = fileExtension(filename);
	
	bool isOk = false;
	if (fileExt == "jpg" || fileExt == "jpeg") {
		isOk = loadJPEGFile(filename, imageWidth, imageHeight, imageDepth, imageNumComponents, imageType, imageData);
	}
	else if (fileExt == "png") {
		isOk = loadPNGFile(filename, imageWidth, imageHeight, imageDepth, imageNumComponents, imageType, imageData);
	}
	else
		isOk = false;

	if (!isOk) {
		getCoreEngine()->logger()->printMessage("Failed open file \"" + filename + "\"", ILog::MessageType_Error); 
		return 0;
	}

	pImage = new Image(filename);
	pImage->setWidth(imageWidth);
	pImage->setHeight(imageHeight);
	pImage->setDepth(imageDepth);
	pImage->setNumComponents(imageNumComponents);
	pImage->setType(imageType);
	pImage->setData(imageData);
	m_pResourceManager->addResource(pImage);
	return pImage;
}

IImage *ImageManager::getByName(const std::string& name) const
{
	return reinterpret_cast<IImage*>(m_pResourceManager->getByName(name));
}

std::string ImageManager::fileExtension(const std::string& filename)
{
	const std::string::size_type dotPos = filename.find_last_of(".");
	if (dotPos != std::string::npos) {
		std::string ext = filename.substr(dotPos+1);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		return ext;
	}
    return "";
}

bool ImageManager::loadJPEGFile(const std::string& filename, uint32& width, uint32& height, uint32& depth, uint32& numComponents, Type& type, void *&data)
{
	FILE *file;
	if (!(file = fopen(filename.c_str(), "rb")))
		return false; // Не удалось открыть файл
	
	unsigned char *rowptr[1];
	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;

	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info);

	if (!file)
		return false; // Ошибка чтения файла

	jpeg_stdio_src(&info, file);
	jpeg_read_header(&info, TRUE);
	jpeg_start_decompress(&info);
	width = info.output_width;
	height = info.output_height;
	depth = 1;
	numComponents = info.num_components;
	type = TYPE_UNSIGNED_INT_8;
	data = (unsigned char*)malloc(width * height * numComponents);
	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char*)data + numComponents * info.output_width * (info.output_height - info.output_scanline - 1);
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	jpeg_finish_decompress(&info);
	return true;
}

bool ImageManager::loadPNGFile(const std::string& filename, uint32& width, uint32& height, uint32& depth, uint32& numComponents, Type& type, void *&data)
{
	png_structp png_ptr;
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
    data = new uint8 [numComponents * width * height];
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (uint32 i = 0; i < height; ++i)
		memcpy((uint8*)data + (row_bytes * (height-1-i)), row_pointers[i], row_bytes);
 
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	return true;
}

} // namespace