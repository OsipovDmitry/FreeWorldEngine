#ifndef __IGPUTEXTURE__
#define __IGPUTEXTURE__

#include <Types.h>

namespace FreeWorldEngine {

namespace Renderer {

struct TextureFormat {

    enum PixelFormat {
		PixelFormat_None,
        PixelFormat_NormalizeUnsigned, // � ������ [0..MAXINT-1], � ������� [0..1]
        PixelFormat_NormalizeSigned, // � ������ [0..MAXINT-1], � ������� [-1..1]

        PixelFormat_UnnormalizeUnsigned, // � � ������ � � ������� [0..MAXINT-1]
        PixelFormat_UnnormalizeSigned, // � � ������ � � ������� [-MAXINT/2..MAXINT/2-1]
        PixelFormat_UnnormalizeFloat, // � � ������ � � ������� [-FLTMAX..FLTMAX]

		PixelFormat_SpecialDepth,
        //    PixelFormat_SpecialR11G11B10,
        //    PixelFormat_SpecialR10G10B10A2,
    };

    enum ChannelSize {
        ChannelSize_8 = 8,
        ChannelSize_16 = 16,
        ChannelSize_32 = 32,
    };

    enum ChannelsCount {
        ChannelsCount_1 = 1,
        ChannelsCount_2 = 2,
        ChannelsCount_3 = 3,
        ChannelsCount_4 = 4
    };

    TextureFormat(const PixelFormat pf, const ChannelSize cs, const ChannelsCount cc) : pixelFormat(pf), channelSize(cs), channelsCount(cc) {}
	TextureFormat() : pixelFormat(PixelFormat_NormalizeUnsigned), channelSize(ChannelSize_8), channelsCount(ChannelsCount_4) {}

    PixelFormat pixelFormat;
    ChannelSize channelSize;
    ChannelsCount channelsCount;

};

class IGPUBuffer;

class IGPUTexture {
public:
    enum IGPUTextureType {
        IGPUTextureType_1D,
        IGPUTextureType_2D,
        IGPUTextureType_3D,
        IGPUTextureType_CubeMap,
        IGPUTextureType_1DArray,
        IGPUTextureType_2DArray,
        IGPUTextureType_Rectangle,
        IGPUTextureType_Buffer,
		IGPUTextureType_Count
    };
	enum IGPUTextureMinFilter {
		IGPUTextureMinFilter_Nearest,
		IGPUTextureMinFilter_Linear,
		IGPUTextureMinFilter_NearestMipmapNearest,
		IGPUTextureMinFilter_NearestMipmapLinear,
		IGPUTextureMinFilter_LinearMipmapNearest,
		IGPUTextureMinFilter_LinearMipmapLinear
	};
	enum IGPUTextureMagFilter {
		IGPUTextureMagFilter_Nearest,
		IGPUTextureMagFilter_Linear
	};
	enum IGPUTextureWrap {
		IGPUTextureWrap_Clamp,
		IGPUTextureWrap_Repeat,
		IGPUTextureWrap_MirroredRepeat
	};

    virtual ~IGPUTexture() {}

    virtual IGPUTextureType type() const = 0;
    virtual TextureFormat format() const = 0;
    virtual void size(int32 *result) const = 0; // width: result[0], height: result[1], depth: result[2]

	virtual void setSubData(const uint32 *offset, const uint32 *size, TextureFormat::ChannelsCount dataChannelsCount, Type dataType, const void *data) = 0;
	virtual void *subData(const uint32 *offset = 0, const uint32 *size = 0) const = 0;

	virtual void setBuffer(const IGPUBuffer* pBuffer) const = 0; // ���������� ��� IGPUTextureType_Buffer �������

	virtual void setMinFilter(IGPUTextureMinFilter filter) = 0;
	virtual void setMagFilter(IGPUTextureMagFilter filter) = 0;

	virtual void setWrapS(IGPUTextureWrap wrap) = 0;
	virtual void setWrapT(IGPUTextureWrap wrap) = 0;
	virtual void setWrapR(IGPUTextureWrap wrap) = 0;

	virtual void generateMipMaps() = 0;
};

} // namespace

} // namespace

#endif // __IGPUTEXTURE__
