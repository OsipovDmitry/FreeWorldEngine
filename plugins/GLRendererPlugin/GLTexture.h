#ifndef __GLTEXTURE__
#define __GLTEXTURE__

#include <3rdparty/glew/glew.h>
#include <renderer/IGPUTexture.h>

namespace FreeWorldEngine {

namespace Renderer {

class GLTexture : public IGPUTexture {
public:
	GLTexture(IGPUTextureType type, GLenum id);
	~GLTexture();

	IGPUTextureType type() const;
    TextureFormat format() const;
    void size(int32 *result) const;

	void setSubData(const uint32 *offset, const uint32 *size, TextureFormat::ChannelsCount dataChannelsCount, Type dataType, const void *data);
	void *subData(const uint32 *offset = 0, const uint32 *size = 0) const;

	void setBuffer(const IGPUBuffer* pBuffer) const;

	void setMinFilter(IGPUTextureMinFilter filter) const;
	void setMagFilter(IGPUTextureMagFilter filter) const;

	void generateMipMaps() const;

    GLuint GLid() const;

    static TextureFormat fromGLinternalFormat(GLenum format);
	static GLenum GLformat(TextureFormat::ChannelsCount channelsCount);
    static GLenum GLinternalFormat(const TextureFormat& format);
    static GLenum GLtarget(IGPUTextureType type);
	static GLenum GLMinFilter(IGPUTextureMinFilter filter);
	static GLenum GLMagFilter(IGPUTextureMagFilter filter);

private:
	GLuint m_id;
	IGPUTextureType m_type;

}; // class GLTexture

} /// namespace

} // namespace

#endif // __GLTEXTURE__
