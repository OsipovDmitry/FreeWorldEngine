#ifndef __TREE__
#define __TREE__

#include <vector>
#include <algorithm>

namespace FreeWorldEngine {

template <class T>
class Tree {
public:
	Tree(const T& data = T(), Tree *parent = 0);
	Tree(Tree *parent = 0);
	Tree(const Tree& other);

	Tree<T>& operator =(const Tree& other);

	Tree<T> *parent();
	const Tree<T> *parent() const;

	std::vector<Tree<T>*>& children();
	const std::vector<Tree<T>*>& children() const;

	T& data();
	const T& data() const;

	bool isRoot() const;
	bool isLeaf() const;

	class Iterator {
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
	};

private:
	Tree<T> *m_pParent;
	std::vector<Tree<T>*> m_children;
	T m_data;

}; // class Tree


template<class T>
inline Tree<T>::Tree(const T & data, Tree * parent) :
	m_pParent(parent),
	m_childs(),
	m_data(data)
{
}

template<class T>
inline Tree<T>::Tree(Tree * parent) :
	m_pParent(parent),
	m_childs(),
	m_data()
{
}

template<class T>
inline Tree<T>::Tree(const Tree & other)
{
	// copy
}

template<class T>
inline Tree<T> & Tree<T>::operator=(const Tree & other)
{
	// copy
	return *this;
}

template<class T>
inline Tree<T> * Tree<T>::parent()
{
	return m_pParent;
}

template<class T>
inline const Tree<T> * Tree<T>::parent() const
{
	return m_pParent;
}

template<class T>
inline std::vector<Tree<T>*>& Tree<T>::children()
{
	return m_children;
}

template<class T>
inline const std::vector<Tree<T>*>& Tree<T>::children() const
{
	return m_children;
}

template<class T>
inline T & Tree<T>::data()
{
	return m_data;
}

template<class T>
inline const T & Tree<T>::data() const
{
	return m_data;
}

template<class T>
inline bool Tree<T>::isRoot() const
{
	return m_pParent == 0;
}

template<class T>
inline bool Tree<T>::isLeaf() const
{
	return m_children.empty();
}

template<class T>
inline T & Tree<T>::Iterator::operator*()
{
	return m_p->data();
}

template<class T>
inline const T & Tree<T>::Iterator::operator*() const
{
	return m_p->data();
}

template<class T>
inline T & Tree<T>::Iterator::operator->()
{
	return m_p->data();
}

template<class T>
inline const T & Tree<T>::Iterator::operator->() const
{
	return m_p->data();
}

template<class T>
inline Tree<T>::Iterator & Tree<T>::Iterator::operator++()
{
	if (m_p->m_pParent = 0) {
		if (m_p->m_children.empty() == false)
			m_p = m_p->m_children.at(0);
		else
			// end();
	}

	auto it = std::find(m_p->m_children->cbegin(), m_p->m_children->cend(), this);
	if (it != m_p->m_children->cend()) {
		m_p = (*(it + 1));
	}

	return &this;
}

template<class T>
inline Tree<T>::Iterator::Iterator(Tree<T>* p) :
	m_p(p)
{
}

} // namespace

#endif // __TREE__
