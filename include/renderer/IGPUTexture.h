#ifndef __IGPUTEXTURE__
#define __IGPUTEXTURE__

#include "Types.h"

namespace FreeWorldEngine {

struct TextureFormat {

    enum PixelFormat {
		PixelFormat_None,
        PixelFormat_NormalizeUnsigned, // в памяти [0..MAXINT-1], в шейдере [0..1]
        PixelFormat_NormalizeSigned, // в памяти [0..MAXINT-1], в шейдере [-1..1]

        PixelFormat_UnnormalizeUnsigned, // и в памяти и в шейдере [0..MAXINT-1]
        PixelFormat_UnnormalizeSigned, // и в памяти и в шейдере [-MAXINT/2..MAXINT/2-1]
        PixelFormat_UnnormalizeFloat, // и в памяти и в шейдере [-FLTMAX..FLTMAX]

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

    virtual ~IGPUTexture() {}

    virtual IGPUTextureType type() const = 0;
    virtual TextureFormat format() const = 0;
    virtual void size(int32 *result) const = 0; // width: result[0], height: result[1], depth: result[2]

	virtual void setSubData(const uint32 *offset, const uint32 *size, TextureFormat::ChannelsCount dataChannelsCount, Type dataType, const void *data) = 0;
	virtual void *subData(const uint32 *offset = 0, const uint32 *size = 0) const = 0;

	virtual void setBuffer(const IGPUBuffer* pBuffer) const = 0; // специально для IGPUTextureType_Buffer текстур

};

} // namespace

#endif // __IGPUTEXTURE__
