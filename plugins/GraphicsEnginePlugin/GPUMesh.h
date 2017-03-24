#ifndef __GPUMESH__
#define __GPUMESH__

#include <Types.h>

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUBuffer;
	class IGPUBufferContainer;
}
	
namespace GraphicsEngine {

class GPUMesh {
public:
	GPUMesh(Renderer::IGPUBuffer *pVBO, Renderer::IGPUBuffer *pIBO, Renderer::IGPUBufferContainer *pVAO, uint32 numIndices, PrimitiveFormat primitiveFormat);
	~GPUMesh();

	void destroy();

	Renderer::IGPUBufferContainer *bufferContainer() const;
	uint32 numIndices() const;
	PrimitiveFormat primitiveFormat() const;

	void setMesh(Mesh *pMesh);

private:
	Renderer::IGPUBuffer *m_pVertexBuffer, *m_pIndexBuffer;
	Renderer::IGPUBufferContainer *m_pVertexArray;
	uint32 m_numIndices;
	PrimitiveFormat m_primitiveFormat;

}; // class GPUMesh

} // namespace
} // namespace

#endif // __GPUMESH__