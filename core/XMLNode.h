#ifndef __XMLNODE__
#define __XMLNODE__

#include <string>
#include <list>

namespace FreeWorldEngine {

struct XMLNodePrivate;

class XMLNode {
public:
	XMLNode();
	~XMLNode();

	std::string name() const;

	std::string attribute(const std::string& attributeName) const;
	std::list<std::string> attributeList() const;

	std::string text() const;

	XMLNode *child(const std::string& childName) const;
	std::list<XMLNode*> children() const;

	XMLNode *parentNode() const;
	XMLNode *rootNode() const;

	static XMLNode *openFromFile(const std::string& fileName);
	static XMLNode *openFromData(const std::string& xmlData);
	static void close(XMLNode *pXML);

private:
	XMLNodePrivate *m_pPrivateData;

};

} // namespace

#endif // __XMLNODE__