#ifndef __GLOCCLUSIONQUERY__
#define __GLOCCLUSIONQUERY__

#include <3rdparty/glew/glew.h>

#include <renderer/IGPUOcclusionQuery.h>

namespace FreeWorldEngine {

namespace Renderer {

class GLOcclusionQuery : public IGPUOcclusionQuery {
public:
	GLOcclusionQuery(const GLuint id);
	~GLOcclusionQuery();

	void begin() const;
	void end() const;
	bool isReady() const;
	uint32 result() const;

	GLuint GLid() const;

private:
	GLuint m_id;

}; // class IGPUBuffer

} // namespace

} // namespace

#endif // __GLOCCLUSIONQUERY__

