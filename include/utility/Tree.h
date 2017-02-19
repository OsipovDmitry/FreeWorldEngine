#ifndef __TREE__
#define __TREE__

#include <vector>
#include <algorithm>

namespace FreeWorldEngine {

template <class T>
class Tree {
public:
	template <class T> class Node {
	public:
		Node<T> *parent() { return m_pParent; }
		const Node<T> *parent() const { return m_pParent; }

		std::vector<Node<T>*>& children() { return m_children; }
		const std::vector<Node<T>*>& children() const { return m_children; }

		T& data() { return m_data; }
		const T& data() const { return m_data; }

		bool isRoot() const { return m_pParent == nullptr; }
		bool isLeaf() const { return m_children.empty(); }

	private:
		Node<T> *m_pParent;
		std::vector<Node<T>*> m_children;
		T m_data;

		Node(const T& data = T(), Node<T> *parent = 0) : m_pParent(parent), m_children(), m_data(data) {}
		Node(Node<T> *parent = 0) : m_pParent(parent), m_children(), m_data() {}
		Node(const Node<T>& other) { /* */ }
		~Node() { /**/ }

		Node<T>& operator =(const Node<T>& other) { /* */ }
	};

	/*class Iterator {
	public:
		T& operator *();
		const T& operator *() const;

		T& operator ->();
		const T& operator ->() const;

		Iterator& operator ++();
		Iterator& operator --();

	private:
		Iterator(Tree<T> *p);
		Tree *m_p;
	};*/

	Tree()

	Node<T> *rootNode() { return m_pRootNode; }
	const Node<T> *rootNode() const { return m_pRootNode; }

	bool isNull() const { return m_pRootNode == nullptr; }

private:
	Node<T> *m_pRootNode;

}; // class Tree

} // namespace

#endif // __TREE__
