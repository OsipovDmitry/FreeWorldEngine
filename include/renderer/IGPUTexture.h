#ifndef __IGPUTEXTURE__
#define __IGPUTEXTURE__

#include "Types.h"

struct TextureFormat {

    enum PixelFormat {
        PixelFormat_NormalizeUnsigned, // в памяти [0..MAXINT-1], в шейдере [0..1]
        PixelFormat_NormalzieSigned, // в памяти [0..MAXINT-1], в шейдере [-1..1]

        PixelFormat_UnnormalizeUnsigned, // и в памяти и в шейдере [0..MAXINT-1]
        PixelFormat_UnnormalizeSigned, // и в памяти и в шейдере [-MAXINT/2..MAXINT/2-1]
        PixelFormat_UnnormalizeFloat, // и в памяти и в шейдере [-FLTMAX..FLTMAX]

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

class IGPUTexture {
public:

    enum IGPUTextureType {
        IGPUTextureType_1D,
        IGPUTextureType_2D,
        IGPUTextureType_3D,
        IGPUTextureType_CubeMap,
        IGPUTextureType_1DArray,
        IGPUTextureType_2DArray,
        IGPUTextureType_CubeMapArray,
        IGPUTextureType_Rectangle,
        IGPUTextureType_Buffer
    };

    virtual ~IGPUTexture() {}

    virtual IGPUTextureType type() const = 0;
    virtual TextureFormat format() const = 0;
    virtual uint32* size() const = 0; // width: return[0], height: return[1], depth: return[2]

	virtual void setSubData(void *data, uint32 *offset = 0, uint32 *size = 0) = 0;
	virtual void *subData(uint32 *offset = 0, uint32 *size = 0) const = 0;

};



#endif // __IGPUTEXTURE__
