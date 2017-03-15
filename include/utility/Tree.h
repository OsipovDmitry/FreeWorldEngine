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

private:
	class Iterator : public std::iterator<std::forward_iterator_tag, Node*> {
	public:
		bool operator ==(const Iterator& other) const { return m_pNode == other.m_pNode; }
		bool operator !=(const Iterator& other) const { return m_pNode != other.m_pNode; }
		Node *operator *() { return m_pNode; }
		int depth() const { int res = 0; Node *p = m_pNode; while (p->m_pParent) { p = p->m_pParent; ++res; } return res; }
	protected:
		Node *m_pNode;
		const Tree<T> *m_pTree;
		Iterator(const Tree<T> *pTree, Node *pNode) : m_pTree(pTree), m_pNode(pNode) {}
	};

public:
	class DepthIterator : public Iterator {
	public:
		DepthIterator& operator ++() {
			if (!m_pNode->m_children.empty()) {
				m_pNode = m_pNode->m_children.at(0);
			}
			else {
				int thisPos;
				do {
					thisPos = std::find(m_pNode->m_pParent->m_children.cbegin(), m_pNode->m_pParent->m_children.cend(), m_pNode) - m_pNode->m_pParent->m_children.cbegin();
					m_pNode = m_pNode->m_pParent;
				} while ((thisPos == m_pNode->m_children.size()-1) && (m_pNode != m_pTree->m_pRootNode));
				m_pNode = (thisPos != m_pNode->m_children.size() - 1) ? m_pNode->m_children.at(thisPos + 1) : m_pTree->m_pEnd;
			}
			return *this;
		}
		DepthIterator operator ++(int) {
			Node *pOldNode = m_pNode;
			operator ++();
			return DepthIterator(m_pTree, pOldNode);
		}
	private:
		DepthIterator(const Tree<T> *pTree, Node *pNode) : Iterator(pTree, pNode) {}
		friend class Tree<T>;
	};

	class WidthIterator : public Iterator {
	public:
		WidthIterator& operator ++() {
			//
			return *this;
		}
		WidthIterator operator ++(int) {
			Node *pOldNode = m_pNode;
			operator ++();
			return WidthIterator(m_pTree, pOldNode);
		}
	private:
		WidthIterator(Tree<T> *pTree, Node *pNode) : Iterator(pTree, pNode) {}
		friend class Tree<T>;
	};

	Tree(const T& rootNodeValue = T());
	Tree(const Tree& other);
	~Tree();

	Tree<T>& operator =(const Tree& other);

	const Node *rootNode() const;
	Node *rootNode();

	DepthIterator beginDepth() const { return DepthIterator(this, m_pRootNode); }
	DepthIterator endDepth() const { return DepthIterator(this, m_pEnd); }

	WidthIterator beginWidth() { return WidthIterator(this, m_pRootNode); }
	WidthIterator endWidth() { return WidthIterator(this, m_pEnd); }

private:
	Node *m_pRootNode;
	Node *m_pEnd;

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
	if (pos > m_children.size())
		pos = m_children.size();
	m_children.insert(m_children.begin()+pos, new TreeNode<T>(data, this));
	return m_children.at(pos);
}

template<class T>
inline TreeNode<T> *TreeNode<T>::addChild(const T& data)
{
	return insertChild(m_children.size(), data);
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
	m_data(data),
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
	m_pRootNode(new Node(rootNodeValue)),
	m_pEnd(new Node())
{
}

template<class T>
inline Tree<T>::Tree(const Tree &other) :
	m_pEnd(new Node())
{
	delete m_pRootNode;
	m_pRootNode = new Node(*(other.m_pRootNode));
}

template<class T>
inline Tree<T>::~Tree()
{
	delete m_pRootNode;
	delete m_pEnd;
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
