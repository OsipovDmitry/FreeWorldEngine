#ifndef __VERTEXARRAYOBJECT__
#define __VERTEXARRAYOBJECT__

#include "IVertexArrayObject.h"

namespace FreeWorldEngine {

class GLVertexArrayObject : public IVertexArrayObject {
public:
	GLVertexArrayObject(GLuint id);
	~GLVertexArrayObject();

	GLuint id() const;
	void bind() const;
	void unbind() const;

	static void bind(GLuint id);

private:
	GLuint m_id;

	static GLuint m_cache;
	
}; // class GLVertexArrayObject

} // namespace

#endif // __VERTEXARRAYOBJECT__