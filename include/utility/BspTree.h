#ifndef __BSPTREE__
#define __BSPTREE__

#include <vector>

#include "../Types.h"
#include "MathTypes.h"

namespace FreeWorldEngine {

namespace Math {

struct BspNode {
	BspNode *pLeftNode, *pRightNode; // ����������
	uint32 *pIndices; // ������� ������ ������������� � ������ ����
	uint32 numIndices; // ���������� ��������. ������ ���� ������ 3, ��� ��� BSP ������ �������� ��� ����������������� ������

	BspNode() : pLeftNode(pLeftChild), pRightNode(pRightChild), pIndices(pTris), numIndices(numTris) {} 
};

class BspTree {
public:
	BspTree();
	~BspTree();

	void destroy();
	void build(Mesh *pSourceMesh, const float eps = 0.0001f);

private:
	BspNode *m_pRootNode; // �������� ����
	Mesh *m_pDestMesh; // ���, ���������� � ���������� ����������� BSP ������

	Plane calcSeparatingPlane(const std::vector<uint32>& indices) const;
	BspNode *buildNode(const std::vector<uint32>& indices, const float eps = 0.0001f);
};

} // namespace
} // namespace

#endif // __BSPTREE__