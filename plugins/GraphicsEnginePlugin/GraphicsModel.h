#ifndef __GRAPHICSMODEL__
#define __GRAPHICSMODEL__

#include <math/MathTypes.h>
#include <graphics_engine/IGraphicsModel.h>

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUBuffer;
	class IGPUBufferContainer;
}
	
namespace GraphicsEngine {

class GPUMesh;

class GraphicsModel : public IGraphicsModel {
public:
	GraphicsModel(const std::string& name);
	~GraphicsModel();

	std::string name() const;

	IGraphicsMaterial *material() const;
	void setMaterial(IGraphicsMaterial *pMaterial);

	void setMesh(Mesh *pMesh);

	GPUMesh *gpuMesh() const;
	GPUMesh *gpuMeshBoundSphere() const;
	GPUMesh *gpuMeshAabb() const;

	const Math::Sphere& boundingSphere() const;
	const Math::Aabb& aabb() const;

private:
	std::string m_name;
	Math::Sphere m_boundSphere;
	Math::Aabb m_aabb;
	IGraphicsMaterial *m_pMaterial;

	GPUMesh *m_pGPUMesh, *m_pBoudSphereGPUMesh, *m_pAabbGPUMesh;

}; // class GraphicsModel

} // namespace

} // namespace

#endif // __GRAPHICSMODEL__