#ifndef __XMLNODE__
#define __XMLNODE__

#include <string>
#include <list>

#include "UtilitySettings.h"

namespace FreeWorldEngine {

namespace Utility {

class File;

struct XMLNodePrivate;
class UTILITY_EXPORT XMLNode {
	friend class XMLRoot;

public:
	typedef std::list<std::string> AttributeList;
	typedef std::list<XMLNode*> NodeList;

	std::string name() const;

	std::string attributeValue(const std::string& attributeName, const std::string& defaultValue = std::string()) const;
	AttributeList attributes() const;

	std::string text() const;

	XMLNode *child(const std::string& childName) const;
	NodeList children() const;

	XMLNode *parent() const;

protected:
	XMLNode();
	virtual ~XMLNode();

private:
	XMLNodePrivate *const m;

	void generateChildren() const;
};

struct XMLRootPrivate;
class UTILITY_EXPORT XMLRoot : public XMLNode {
public:
	static XMLRoot *openFromFile(const File& file);
	static XMLRoot *openFromData(const char *xmlData);
	static XMLRoot *openFromData(const std::string& xmlData);
	static void close(XMLRoot*);

private:
	XMLRootPrivate *const m;

	XMLRoot();
	~XMLRoot();

	bool parseBuffer();
};

} // namespace
} // namespace

#endif // __XMLNODE__
