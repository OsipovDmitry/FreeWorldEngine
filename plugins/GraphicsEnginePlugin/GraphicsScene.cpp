#include <algorithm>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

#include "GraphicsScene.h"
#include "GraphicsModel.h"
#include "GPUMesh.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneNode::GraphicsSceneNode(GraphicsSceneNode *pParentNode) :
	m_pParentNode(pParentNode),
	m_pModel(nullptr),
	m_childNodes(),
	m_position(),
	m_orientation(),
	m_boundingBoxMinSize(),
	m_boundingBoxMaxSize(),
	m_pBoundingBoxGpuMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines)),
	m_needUpdateBoundingBox(false),
	m_needUpdateTransformation(true)
{
}

GraphicsSceneNode::~GraphicsSceneNode()
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p){delete p; });
}

IGraphicsSceneNode * GraphicsSceneNode::clone(IGraphicsSceneNode * pParent) const
{
	/*IGraphicsSceneNode *pNewNode = pParent->createChild();
	pNewNode->setOrientation(m_orientation);
	pNewNode->setPosition(m_position);
	pNewNode->setModel(m_pModel);
	std::for_each(m_childNodes.cbegin(), m_childNodes.cend(), [pNewNode](const GraphicsSceneNode *p) {
		p->clone(pNewNode);
	});
	return pNewNode;*/
	return nullptr;
}

glm::vec3 GraphicsSceneNode::position() const
{
	return m_position;
}

void GraphicsSceneNode::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	
	m_needUpdateTransformation = true;
	updateTransformationRecursiveDown();

	updateBoundingBoxRecursiveUp();
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;

	m_needUpdateTransformation = true;
	updateTransformationRecursiveDown();

	m_needUpdateBoundingBox = true;
	updateBoundingBoxRecursiveDown();
	updateBoundingBoxRecursiveUp();
}

const glm::mat4x4& GraphicsSceneNode::worldTransformation() const
{
	if (m_needUpdateTransformation) {
		recalcTransformation();
		m_needUpdateTransformation = false;
	}
	return m_cacheWorldlTransform;
}

IGraphicsSceneNode *GraphicsSceneNode::createChild()
{
	GraphicsSceneNode *pNode = new GraphicsSceneNode(this);
	m_childNodes.push_back(pNode);
	return pNode;
}

void GraphicsSceneNode::destroyChild(IGraphicsSceneNode *pNode)
{
	auto it = std::find(m_childNodes.begin(), m_childNodes.end(), pNode);
	if (it != m_childNodes.end()) {
		m_childNodes.erase(it);
		delete *it;

		m_needUpdateBoundingBox = true;
		updateBoundingBoxRecursiveUp();
	}
}

uint32 GraphicsSceneNode::numChildren() const
{
	return m_childNodes.size();
}

IGraphicsSceneNode *GraphicsSceneNode::childAt(const uint32 idx) const
{
	return m_childNodes.at(idx);
}

IGraphicsSceneNode *GraphicsSceneNode::parentNode() const
{
	return m_pParentNode;
}

IGraphicsModel *GraphicsSceneNode::model() const
{
	return m_pModel;
}

void GraphicsSceneNode::setModel(IGraphicsModel *pModel)
{
	m_pModel = static_cast<GraphicsModel*>(pModel);
	m_needUpdateBoundingBox = true;
	updateBoundingBoxRecursiveUp();
}

GPUMesh * GraphicsSceneNode::gpuMeshAabb() const
{
	return m_pBoundingBoxGpuMesh;
}

Math::Aabb GraphicsSceneNode::boundingBox() const
{
	if (m_needUpdateBoundingBox) {
		recalcBoundingBox();
		m_needUpdateBoundingBox = false;
	}

	glm::vec3 nodePos(worldTransformation()[3]);
	return Math::Aabb(nodePos + m_boundingBoxMinSize, nodePos + m_boundingBoxMaxSize);
}

void GraphicsSceneNode::updateTransformationRecursiveDown() const
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p) {
		p->m_needUpdateTransformation = true;
		p->updateTransformationRecursiveDown();
	});
}

void GraphicsSceneNode::recalcTransformation() const
{
	glm::mat4x4 localTransformation = glm::translate(glm::mat4x4(), m_position) * glm::mat4_cast(m_orientation);
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->worldTransformation() : glm::mat4x4()) * localTransformation;
}

void GraphicsSceneNode::updateBoundingBoxRecursiveUp() const
{
	if (m_pParentNode) {
		m_pParentNode->m_needUpdateBoundingBox = true;
		m_pParentNode->updateBoundingBoxRecursiveUp();
	}
}

void GraphicsSceneNode::updateBoundingBoxRecursiveDown() const
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p) {
		p->m_needUpdateBoundingBox = true;
		p->updateBoundingBoxRecursiveDown();
	});
}

void GraphicsSceneNode::recalcBoundingBox() const
{
	const Math::Sphere& modelSphere = m_pModel ? m_pModel->boundingSphere() : Math::Sphere(0.0f,0.0f,0.0f,0.0f);
	const glm::vec3 spherePos = glm::vec3(worldTransformation() * glm::vec4(modelSphere.x, modelSphere.y, modelSphere.z, 1.0f));
	
	glm::vec3 boxSize(modelSphere.w);
	Math::Aabb thisBox(spherePos - boxSize, spherePos + boxSize);

	for (const auto& it : m_childNodes) {
		const Math::Aabb childBox = it->boundingBox();
		if (childBox.vMin.x < thisBox.vMin.x) thisBox.vMin.x = childBox.vMin.x;
		if (childBox.vMin.y < thisBox.vMin.y) thisBox.vMin.y = childBox.vMin.y;
		if (childBox.vMin.z < thisBox.vMin.z) thisBox.vMin.z = childBox.vMin.z;
		if (childBox.vMax.x > thisBox.vMax.x) thisBox.vMax.x = childBox.vMax.x;
		if (childBox.vMax.y > thisBox.vMax.y) thisBox.vMax.y = childBox.vMax.y;
		if (childBox.vMax.z > thisBox.vMax.z) thisBox.vMax.z = childBox.vMax.z;
	}

	const glm::vec3 nodePos(worldTransformation()[3]);
	m_boundingBoxMinSize = thisBox.vMin - nodePos;
	m_boundingBoxMaxSize = thisBox.vMax - nodePos;

	updateBoundingBoxRecursiveUp();

	Mesh aabbMesh;
	Math::MeshWrapper aabbWrapper(&aabbMesh);
	aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	aabbWrapper.setVertexStride(3);
	glm::vec3 aabbSize = m_boundingBoxMaxSize - m_boundingBoxMinSize;
	glm::vec3 aabbPos = nodePos + 0.5f * (m_boundingBoxMinSize + m_boundingBoxMaxSize);
	Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
	aabbWrapper.translateMesh((float*)&(aabbPos));
	m_pBoundingBoxGpuMesh->setMesh(&aabbMesh);
}

GraphicsScene::GraphicsScene(const std::string& name) :
	m_name(name),
	m_pRootNode(new GraphicsSceneNode(nullptr))
{
}

GraphicsScene::~GraphicsScene()
{
	delete m_pRootNode;
}

std::string GraphicsScene::name() const
{
	return m_name;
}

IGraphicsSceneNode *GraphicsScene::rootNode() const
{
	return m_pRootNode;
}

} // namespace

} // namespace