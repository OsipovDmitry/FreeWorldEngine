#ifndef __TREE__
#define __TREE__

#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <iterator>


namespace FreeWorldEngine {

namespace Utility {

template <class T>
class Tree;

template <class T>
class TreeNode {
public:
	void destroyChildren();
	void copyFrom(const TreeNode *pNode);

	const T& data() const;
	void setData(const T& data);

	const TreeNode<T>* parentNode() const;
	void setParent(const TreeNode<T> *pParentNode);

	unsigned int numChidNodes() const;
	TreeNode<T> *atChild(const unsigned int pos);
	const TreeNode<T> *atChild(const unsigned int pos) const;
	TreeNode<T> *insertChild(const unsigned int pos, const T& data = T());
	TreeNode<T> *addChild(const T& data = T());
	void eraseChild(const unsigned int pos);

	bool isRoot() const { m_pParent == 0; }
	bool isLeaf() const { m_children.size() == 0; }

private:
	TreeNode(const T& data, TreeNode<T> *parent = 0);
	TreeNode(TreeNode<T> *parent = 0);
	TreeNode(const TreeNode<T>& other);
	~TreeNode();

	TreeNode<T>& operator =(const TreeNode<T>& other);

	T m_data;
	TreeNode<T> *m_pParent;
	std::vector<TreeNode<T>*> m_children;

	friend class Tree<T>;
};

template <class T>
class Tree {
public:
	typedef TreeNode<T> Node;

	Tree(const T& rootNodeValue = T());
	Tree(const Tree& other);
	~Tree();

	Tree<T>& operator =(const Tree& other);

	const Node *rootNode() const;
	Node *rootNode();

private:
	Node *m_pRootNode;

};

template<class T>
inline void TreeNode<T>::destroyChildren()
{
	std::for_each(m_children.begin(), m_children.end(),
		[](TreeNode<T> *p) { p->destroyChildren(); delete p; });
	m_children.clear();
}

template<class T>
inline void TreeNode<T>::copyFrom(const TreeNode *pNode)
{
	destroyChildren();

	m_data = pNode->m_data;
	m_children.reserve(pNode->m_children.size());
	std::transform(pNode->m_children.cbegin(), pNode->m_children.cend(), std::back_inserter(m_children),
		[this](const TreeNode<T> *p) { TreeNode<T> *pNewNode = new TreeNode<T>(this); pNewNode->copyFrom(p); return pNewNode; })
}

template<class T>
inline const T & TreeNode<T>::data() const
{
	return m_data;
}

template<class T>
inline void TreeNode<T>::setData(const T & data)
{
	m_data = data;
}

template<class T>
inline const TreeNode<T>* TreeNode<T>::parentNode() const
{
	return m_pParent;
}

template<class T>
inline void TreeNode<T>::setParent(const TreeNode<T>* pParentNode)
{
	m_pParent = pParentNode;
}

template<class T>
inline unsigned int TreeNode<T>::numChidNodes() const
{
	return m_children.size();
}

template<class T>
inline TreeNode<T>* TreeNode<T>::atChild(const unsigned int pos)
{
	return m_children.at(pos);
}

template<class T>
inline const TreeNode<T>* TreeNode<T>::atChild(const unsigned int pos) const
{
	return m_children.at(pos);
}

template<class T>
inline TreeNode<T>* TreeNode<T>::insertChild(unsigned int pos, const T& data)
{
	if (pos >= m_children.size())
		pos = m_children.size();
	m_children.insert(pos, new TreeNode<T>(data, this));
	return m_children.at(pos);
}

template<class T>
inline TreeNode<T> *TreeNode<T>::addChild(const T& data)
{
	return insertChild(m_children.size());
}

template<class T>
inline void TreeNode<T>::eraseChild(const unsigned int pos)
{
	if (pos < m_children.size()) {
		delete m_children.at(pos);
		m_children.erase(pos);
	}
	
}

template<class T>
inline TreeNode<T>::TreeNode(const T &data, TreeNode<T> *parent) :
	m_data(T),
	m_pParent(parent),
	m_children()
{
}

template<class T>
inline TreeNode<T>::TreeNode(TreeNode *parent) :
	m_data(),
	m_pParent(parent),
	m_children()
{
}

template<class T>
inline TreeNode<T>::TreeNode(const TreeNode<T>& other)
{
	copyFrom(&other);
}

template<class T>
inline TreeNode<T>::~TreeNode()
{
	destroyChildren();
}

template<class T>
inline TreeNode<T>& TreeNode<T>::operator=(const TreeNode<T>& other)
{
	copyFrom(&other);
	return *this;
}

template<class T>
inline Tree<T>::Tree(const T& rootNodeValue) :
	m_pRootNode(new Node(rootNodeValue))
{
}

template<class T>
inline Tree<T>::Tree(const Tree &other)
{
	delete m_pRootNode;
	m_pRootNode = new Node(*(other.m_pRootNode));
}

template<class T>
inline Tree<T>::~Tree()
{
	delete m_pRootNode;
}

template<class T>
inline Tree<T>& Tree<T>::operator=(const Tree & other)
{
	delete m_pRootNode;
	m_pRootNode = new Node(*(other.m_pRootNode));
	return *this;
}

template<class T>
inline const typename Tree<T>::Node *Tree<T>::rootNode() const
{
	return m_pRootNode;
}

template<class T>
inline typename Tree<T>::Node *Tree<T>::rootNode()
{
	return m_pRootNode;
}

} // namespace
} // namespace

#endif // __TREE__
