#include "GLTexture.h"
#include "GLBuffer.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

GLTexture::GLTexture(IGPUTextureType type, GLenum id) :
	m_type(type),
	m_id(id)
{
}

GLTexture::~GLTexture()
{
}

IGPUTexture::IGPUTextureType GLTexture::type() const
{
	return m_type;
}

TextureFormat GLTexture::format() const
{
	pGLRenderer->bindTexture(this, 0);
    GLenum internalFormat;
	glGetTexLevelParameteriv(GLTexture::GLtarget(m_type), 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*)&internalFormat);
    return fromGLinternalFormat(internalFormat);
}

void GLTexture::size(int32 *result) const
{
    GLenum target = GLTexture::GLtarget(m_type);

	pGLRenderer->bindTexture(this, 0);

    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &(result[0]));
	if (m_type == IGPUTextureType_1D || // Нужно ли здесь проверку типа??? Или для любого типа можно брать все три размера???
		m_type == IGPUTextureType_Buffer)
		return;

    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &(result[1]));
	if (m_type == IGPUTextureType_2D || // И здесь???
		m_type == IGPUTextureType_CubeMap ||
		m_type == IGPUTextureType_1DArray ||
		m_type == IGPUTextureType_Rectangle)
		return;

    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_DEPTH, &(result[2]));
}

void GLTexture::setSubData(const uint32 *offset, const uint32 *size, TextureFormat::ChannelsCount dataChannelsCount, Type dataType, const void *data)
{
	GLenum target = GLtarget(m_type);
	GLenum format = GLformat(dataChannelsCount);
	GLenum type = GLRenderer::GLType(dataType);

	if (!target || !format || !type)
		return;

	pGLRenderer->bindTexture(this, 0);

    switch (m_type) {

	case IGPUTexture::IGPUTextureType_2DArray:
	case IGPUTexture::IGPUTextureType_3D: {
		glTexSubImage3D(target, 0, offset[0], offset[1], offset[2], size[0], size[1], size[2], format, type, data);
		break;
	}
	
	case IGPUTexture::IGPUTextureType_1DArray:
	case IGPUTexture::IGPUTextureType_2D:
	case IGPUTexture::IGPUTextureType_Rectangle: {
		glTexSubImage2D(target, 0, offset[0], offset[1], size[0], size[1], format, type, data);
		break;
	}

	case IGPUTexture::IGPUTextureType_CubeMap: {
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+offset[2], 0, offset[0], offset[1], size[0], size[1], format, type, data);
		break;
	}

    case IGPUTexture::IGPUTextureType_1D: {
        glTexSubImage1D(target, 0, offset[0], size[0], format, type, data);
		break;
    }
	
    }
}

void *GLTexture::subData(const uint32 *offset, const uint32 *size) const
{
	return 0;
}

void GLTexture::setBuffer(const IGPUBuffer* pBuffer) const
{
	if (m_type != IGPUTextureType_Buffer) // только для IGPUTextureType_Buffer типа текстур
		return;

	pGLRenderer->bindTexture(this, 0);

	GLenum internalFormat;
	glGetTexLevelParameteriv(GL_TEXTURE_BUFFER, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*)&internalFormat);

	glTexBuffer(GL_TEXTURE_BUFFER, internalFormat, static_cast<const GLBuffer*>(pBuffer)->GLid());
}

GLuint GLTexture::GLid() const
{
    return m_id;
}

TextureFormat GLTexture::fromGLinternalFormat(GLenum format)
{
	switch (format) {

	case GL_R8: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_1);
    case GL_R16: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_1);
    case GL_RG8: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_2);
    case GL_RG16: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_2);
    case GL_RGB8: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3);
    case GL_RGB16: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_3);
    case GL_RGBA8: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_4);
    case GL_RGBA16: return TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_4);

    case GL_R8_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_1);
    case GL_R16_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_1);
    case GL_RG8_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_2);
    case GL_RG16_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_2);
    case GL_RGB8_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3);
    case GL_RGB16_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_3);
    case GL_RGBA8_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_4);
    case GL_RGBA16_SNORM: return TextureFormat(TextureFormat::PixelFormat_NormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_4);
	
	case GL_R16F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_1);
    case GL_R32F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1);
    case GL_RG16F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_2);
    case GL_RG32F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_2);
    case GL_RGB16F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_3);
    case GL_RGB32F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_3);
    case GL_RGBA16F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_4);
    case GL_RGBA32F: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeFloat, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_4);

	case GL_R8I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_1);
    case GL_R16I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_1);
    case GL_R32I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1);
    case GL_RG8I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_2);
    case GL_RG16I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_2);
    case GL_RG32I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_2);
    case GL_RGB8I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3);
    case GL_RGB16I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_3);
    case GL_RGB32I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_3);
    case GL_RGBA8I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_4);
    case GL_RGBA16I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_4);
    case GL_RGBA32I: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeSigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_4);

    case GL_R8UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_1);
    case GL_R16UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_1);
    case GL_R32UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1);
    case GL_RG8UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_2);
    case GL_RG16UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_2);
    case GL_RG32UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_2);
    case GL_RGB8UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3);
    case GL_RGB16UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_3);
    case GL_RGB32UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_3);
    case GL_RGBA8UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_4);
    case GL_RGBA16UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_16, TextureFormat::ChannelsCount_4);
    case GL_RGBA32UI: return TextureFormat(TextureFormat::PixelFormat_UnnormalizeUnsigned, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_4);

	default: return TextureFormat(TextureFormat::PixelFormat_None, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_1);
	}
}

GLenum GLTexture::GLformat(TextureFormat::ChannelsCount channelsCount)
{
    switch (channelsCount) {
    case TextureFormat::ChannelsCount_1: return GL_RED;
    case TextureFormat::ChannelsCount_2: return GL_RG;
    case TextureFormat::ChannelsCount_3: return GL_RGB;
    case TextureFormat::ChannelsCount_4: return GL_RGBA;
    }
    return 0;
}

GLenum GLTexture::GLinternalFormat(TextureFormat& format)
{
    switch (format.pixelFormat) {
    case TextureFormat::PixelFormat_NormalizeUnsigned: {
        switch (format.channelSize) {
        case TextureFormat::ChannelSize_8: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R8;
            case TextureFormat::ChannelsCount_2: return GL_RG8;
            case TextureFormat::ChannelsCount_3: return GL_RGB8;
            case TextureFormat::ChannelsCount_4: return GL_RGBA8;
            }
            break;
        }
        case TextureFormat::ChannelSize_16: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R16;
            case TextureFormat::ChannelsCount_2: return GL_RG16;
            case TextureFormat::ChannelsCount_3: return GL_RGB16;
            case TextureFormat::ChannelsCount_4: return GL_RGBA16;
            }
            break;
        }
        case TextureFormat::ChannelSize_32: {
            break;
        }
        }
        break;
    }
    case TextureFormat::PixelFormat_NormalizeSigned: {
        switch (format.channelSize) {
        case TextureFormat::ChannelSize_8: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R8_SNORM;
            case TextureFormat::ChannelsCount_2: return GL_RG8_SNORM;
            case TextureFormat::ChannelsCount_3: return GL_RGB8_SNORM;
            case TextureFormat::ChannelsCount_4: return GL_RGBA8_SNORM;
            }
            break;
        }
        case TextureFormat::ChannelSize_16: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R16_SNORM;
            case TextureFormat::ChannelsCount_2: return GL_RG16_SNORM;
            case TextureFormat::ChannelsCount_3: return GL_RGB16_SNORM;
            case TextureFormat::ChannelsCount_4: return GL_RGBA16_SNORM;
            }
            break;
        }
        case TextureFormat::ChannelSize_32: {
            break;
        }
        }
        break;
    }
    case TextureFormat::PixelFormat_UnnormalizeUnsigned: {
        switch (format.channelSize) {
        case TextureFormat::ChannelSize_8: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R8UI;
            case TextureFormat::ChannelsCount_2: return GL_RG8UI;
            case TextureFormat::ChannelsCount_3: return GL_RGB8UI;
            case TextureFormat::ChannelsCount_4: return GL_RGBA8UI;
            }
            break;
        }
        case TextureFormat::ChannelSize_16: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R16UI;
            case TextureFormat::ChannelsCount_2: return GL_RG16UI;
            case TextureFormat::ChannelsCount_3: return GL_RGB16UI;
            case TextureFormat::ChannelsCount_4: return GL_RGBA16UI;
            }
            break;
        }
        case TextureFormat::ChannelSize_32: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R32UI;
            case TextureFormat::ChannelsCount_2: return GL_RG32UI;
            case TextureFormat::ChannelsCount_3: return GL_RGB32UI;
            case TextureFormat::ChannelsCount_4: return GL_RGBA32UI;
            }
            break;
        }
        }
        break;
    }
    case TextureFormat::PixelFormat_UnnormalizeSigned: {
        switch (format.channelSize) {
        case TextureFormat::ChannelSize_8: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R8I;
            case TextureFormat::ChannelsCount_2: return GL_RG8I;
            case TextureFormat::ChannelsCount_3: return GL_RGB8I;
            case TextureFormat::ChannelsCount_4: return GL_RGBA8I;
            }
            break;
        }
        case TextureFormat::ChannelSize_16: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R16I;
            case TextureFormat::ChannelsCount_2: return GL_RG16I;
            case TextureFormat::ChannelsCount_3: return GL_RGB16I;
            case TextureFormat::ChannelsCount_4: return GL_RGBA16I;
            }
            break;
        }
        case TextureFormat::ChannelSize_32: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R32I;
            case TextureFormat::ChannelsCount_2: return GL_RG32I;
            case TextureFormat::ChannelsCount_3: return GL_RGB32I;
            case TextureFormat::ChannelsCount_4: return GL_RGBA32I;
            }
            break;
        }
        }
        break;
    }
    case TextureFormat::PixelFormat_UnnormalizeFloat: {
        switch (format.channelSize) {
        case TextureFormat::ChannelSize_8: {
            break;
        }
        case TextureFormat::ChannelSize_16: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R16F;
            case TextureFormat::ChannelsCount_2: return GL_RG16F;
            case TextureFormat::ChannelsCount_3: return GL_RGB16F;
            case TextureFormat::ChannelsCount_4: return GL_RGBA16F;
            }
            break;
        }
        case TextureFormat::ChannelSize_32: {
            switch (format.channelsCount) {
            case TextureFormat::ChannelsCount_1: return GL_R32F;
            case TextureFormat::ChannelsCount_2: return GL_RG32F;
            case TextureFormat::ChannelsCount_3: return GL_RGB32F;
            case TextureFormat::ChannelsCount_4: return GL_RGBA32F;
            }
            break;
        }
        }
        break;
    }
    }
    return 0;
}

GLenum GLTexture::GLtarget(IGPUTextureType type)
{
    switch (type) {
	case IGPUTextureType_1D: return GL_TEXTURE_1D;
	case IGPUTextureType_2D: return GL_TEXTURE_2D;
    case IGPUTextureType_3D: return GL_TEXTURE_3D;
	case IGPUTextureType_CubeMap: return GL_TEXTURE_CUBE_MAP;
	case IGPUTextureType_1DArray: return GL_TEXTURE_1D_ARRAY;
	case IGPUTextureType_2DArray: return GL_TEXTURE_2D_ARRAY;
	case IGPUTextureType_Rectangle: return GL_TEXTURE_RECTANGLE;
	case IGPUTextureType_Buffer: return GL_TEXTURE_BUFFER;
	}
    return 0;
}

} // namespace
