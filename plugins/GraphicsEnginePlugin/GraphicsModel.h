#ifndef __GRAPHICSMODEL__
#define __GRAPHICSMODEL__

#include <graphics_engine/IGraphicsModel.h>

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUBuffer;
	class IGPUBufferContainer;
}
	
namespace GraphicsEngine {

class GraphicsModel : public IGraphicsModel {
public:
	struct RenderData {
		Renderer::IGPUBufferContainer *pBufferContainer;
		uint32 numIndices;
		PrimitiveFormat primitiveFormat;

		RenderData(Renderer::IGPUBufferContainer *pCont, uint32 numInd, PrimitiveFormat prim) : pBufferContainer(pCont), numIndices(numInd), primitiveFormat(prim) {}
	};

	GraphicsModel(const std::string& name);
	~GraphicsModel();

	std::string name() const;

	IGraphicsMaterial *material() const;
	void setMaterial(IGraphicsMaterial *pMaterial);

	void setMesh(Mesh *pMesh);

	Renderer::IGPUBufferContainer *bufferContainer() const;
	uint32 numIndices() const;
	PrimitiveFormat primitiveFormat() const;

private:
	std::string m_name;
	IGraphicsMaterial *m_pMaterial;
	Renderer::IGPUBuffer *m_pVertexBuffer, *m_pIndexBuffer;
	Renderer::IGPUBufferContainer *m_pVertexArray;
	uint32 m_numIndices;
	PrimitiveFormat m_primitiveFormat;

}; // class GraphicsModel

} // namespace

} // namespace

#endif // __GRAPHICSMODEL__