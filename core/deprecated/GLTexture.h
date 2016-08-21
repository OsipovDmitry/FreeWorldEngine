#ifndef __GLTEXTURE__
#define __GLTEXTURE__

#include "ITexture.h"

namespace FreeWorldEngine {

class GLTexture : public ITexture {
public:
	GLTexture(GLenum target, GLuint id);
	~GLTexture();

	GLuint id() const;
	GLenum target() const;
	void bind(unsigned int unit = 0) const;
	void unbind() const;

	static void bind(GLenum target, GLuint id, unsigned int  unit = 0);

private:
	GLuint m_id;
	mutable GLenum m_target;
	mutable unsigned int m_unit;

	static const int CACHE_SIZE = 32;
	static GLuint m_cache[CACHE_SIZE];

}; // class GLTexture

} // namespace

#endif // __GLTEXTURE__