#ifndef __XMLNODE__
#define __XMLNODE__

#include <string>
#include <list>

namespace FreeWorldEngine {

class XMLNode;
struct XMLNodePrivate;
struct XMLDocumentPrivate;

class XMLDocument {
public:
	XMLNode *rootNode() const;

	static XMLDocument *openFromFile(const std::string& fileName);
	static XMLDocument *openFromData(const std::string& xmlData);
	static void close(XMLDocument*);

private:
	XMLDocumentPrivate *m_pData;
};

typedef std::list<XMLNode*> XMLNodeList;

class XMLNode {
public:
	typedef std::list<std::string> AttributeList;

	XMLNode();
	~XMLNode();

	std::string name() const;

	std::string attribute(const std::string& attributeName) const;
	AttributeList attributes() const;

	std::string text() const;

	XMLNode *child(const std::string& childName) const;
	XMLNodeList children() const;

	XMLNode *parent() const;

private:
	XMLNodePrivate *m_pData;
};

} // namespace

#endif // __XMLNODE__
