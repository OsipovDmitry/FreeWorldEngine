#ifndef __BSPTREE__
#define __BSPTREE__

#include <vector>

#include "../Types.h"
#include "MathTypes.h"

namespace FreeWorldEngine {

namespace Math {

struct BspNode {
	BspNode *pLeftNode, *pRightNode; // поддеревья
	uint32 *pIndices; // индексы вершин треугольников в данном узле
	uint32 numIndices; // количество индексов. Должно быть кратно 3, так как BSP дерево строится для триангулированной модели

	BspNode() : pLeftNode(0), pRightNode(0), pIndices(0), numIndices(0) {} 
};

class BspTree {
public:
	BspTree();
	~BspTree();

	void destroy();
	void build(Mesh *pSourceMesh, const float eps = 0.0001f);

private:
	BspNode *m_pRootNode; // корневой узел
	Mesh *m_pDestMesh; // меш, полученный в результате постороения BSP дерева

	Plane calcSeparatingPlane(const std::vector<uint32>& indices) const;
	BspNode *buildNode(const std::vector<uint32>& indices, const float eps = 0.0001f);
};

} // namespace
} // namespace

#endif // __BSPTREE__