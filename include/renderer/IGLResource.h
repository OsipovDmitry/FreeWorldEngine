#ifndef __IGLRESOURCE__
#define __IGLRESOURCE__

#include "glew/glew.h"

namespace FreeWorldEngine {

class IGLResource {
public:
	
	enum IGLResourceType {
		IGLResourceType_None = 0,
		IGLResourceType_BufferObject, // Vertex Buffer Object
		IGLResourceType_VertexArrayObject,
		IGLResourceType_Shader,
		IGLResourceType_ShaderProgram,
		IGLResourceType_Texture,
		IGLResourceType_Sampler,
		IGLResourceType_FrameBufferObject,
		IGLResourceType_RenderBufferObject,
		IGLResourceType_OcclusionQuery,
		IGLResourceType_Count
	}; // enum IGLResourceType

	virtual ~IGLResource() = 0 {}

	virtual GLuint id() const = 0; // Получить идентификатор ресурса
	virtual IGLResourceType type() const = 0; // Получить тип ресурса

}; // class IGLResource

}

#endif // __IGLRESOURCE__