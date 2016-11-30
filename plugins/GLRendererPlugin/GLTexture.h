#ifndef __GLTEXTURE__
#define __GLTEXTURE__

#include <glew-1.13.0/include/GL/glew.h>

#include <renderer/IGPUTexture.h>

namespace FreeWorldEngine {

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

    GLuint GLid() const;

    static TextureFormat fromGLinternalFormat(GLenum format);
	static GLenum GLformat(TextureFormat::ChannelsCount channelsCount);
    static GLenum GLinternalFormat(TextureFormat& format);
    static GLenum GLtarget(IGPUTextureType type);

private:
	GLuint m_id;
	IGPUTextureType m_type;

}; // class GLTexture

} // namespace

#endif // __GLTEXTURE__
