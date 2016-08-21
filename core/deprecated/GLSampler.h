#ifndef __SAMPLER__
#define __SAMPLER__

#include "glew-1.13.0/include/gl/glew.h"

namespace FreeWorldEngine {

class GLSampler {
public:
	GLSampler();
	~GLSampler();

	void create();
	void clear();
	GLuint id() const;
	void bind(unsigned int unit = 0) const;
	void unbind() const;

	static void bind(GLuint id, unsigned int  unit = 0);
	static bool isSampler(GLuint id);
	static void setParameter(GLuint id, GLenum pname, GLint param);
	static void setParameter(GLuint id, GLenum pname, GLfloat param);
	static void setParameter(GLuint id, GLenum pname, const GLint *param);
	static void setParameter(GLuint id, GLenum pname, const GLfloat *param);
	static void setParameterI(GLuint id, GLenum pname, const GLint *param);
	static void setParameterI(GLuint id, GLenum pname, const GLuint *param);

private:
	GLuint m_id;
	mutable unsigned int m_unit;

	static const int CACHE_SIZE = 32;
	static GLuint m_cache[CACHE_SIZE];
	
}; // class GLSampler

} // namespace

#endif // __SAMPLER__