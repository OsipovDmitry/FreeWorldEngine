#include <cstring>
#include <fstream>
#include <vector>

#include <3rdparty/rapidxml/rapidxml.hpp>

#include <utility/XML.h>
#include <utility/File.h>

namespace FreeWorldEngine {

namespace Utility {

using namespace rapidxml;

struct XMLNodePrivate {
	xml_node<> *pData;
	const XMLNode *pParent;
	XMLNode::NodeList children;
	bool childrenIsGenerated;

	XMLNodePrivate() :
	    pData(nullptr),
	    pParent(nullptr),
	    childrenIsGenerated(false)
	{}
};

std::string XMLNode::name() const
{
	return m->pData->name();
}

std::string XMLNode::attributeValue(const std::string &attributeName, const std::string& defaultValue) const
{
	auto attrib = m->pData->first_attribute(attributeName.c_str());
	return attrib ? attrib->value() : defaultValue;
}

XMLNode::AttributeList XMLNode::attributes() const
{
	AttributeList ans;
	for (auto a = m->pData->first_attribute(); a; a = a->next_attribute())
		ans.push_back(std::string(a->name()));
	return ans;
}

std::string XMLNode::text() const
{
	return m->pData->value();
}

XMLNode *XMLNode::child(const std::string &childName) const
{
	if (!m->childrenIsGenerated)
		generateChildren();

	for (auto c = m->children.begin(); c != m->children.end(); ++c) {
		if (strcmp((*c)->m->pData->name(), childName.c_str()) == 0)
			return *c;
	}

	return nullptr;
}

XMLNode::NodeList XMLNode::children() const
{
	if (!m->childrenIsGenerated)
		generateChildren();

	return m->children;
}


XMLNode::XMLNode() :
	m(new XMLNodePrivate)
{}

XMLNode::~XMLNode()
{
	for (auto child = m->children.begin(); child != m->children.end(); ++child)
		delete *child;
	delete m;
}

void XMLNode::generateChildren() const
{
	for (auto pChildData = m->pData->first_node(); pChildData; pChildData = pChildData->next_sibling()) {
		auto pChildNode = new XMLNode;
		pChildNode->m->pData = pChildData;
		pChildNode->m->pParent = this;
		m->children.push_back(pChildNode);
	}
	m->childrenIsGenerated = true;
}

struct XMLRootPrivate {
	xml_document<> document;
	std::vector<char> xmlBuffer;
};

XMLRoot::XMLRoot() :
	m(new XMLRootPrivate)
{}

XMLRoot::~XMLRoot()
{
	delete m;
}

bool XMLRoot::parseBuffer()
{
	try {
		m->document.parse<0>(m->xmlBuffer.data());
		XMLNode::m->pData = m->document.first_node();
		return true;
	}
	catch (const rapidxml::parse_error&) {
		return false;
	}
}

XMLRoot *XMLRoot::openFromFile(const File& file)
{
	XMLRoot *ans = new XMLRoot;

	bool isOpened = file.isOpened();
	if (!isOpened)
		if (!file.open(Utility::File::OpenMode_ReadOnly))
			return nullptr;

	int64 len = file.size();
	ans->m->xmlBuffer.resize(len+1, 0);
	file.readAll(len, ans->m->xmlBuffer.data());
	ans->m->xmlBuffer[len] = '\0';

	if (!isOpened)
		file.close();

	if (!ans->parseBuffer())
		return nullptr;

	return ans;
}

XMLRoot *XMLRoot::openFromData(const char *xmlData)
{
	XMLRoot *ans = new XMLRoot;

	ans->m->xmlBuffer.resize(strlen(xmlData) + 1, 0);
	//strcpy_s(ans->m->xmlBuffer.data(), ans->m->xmlBuffer.size(), xmlData); // Не собралось на линуксе
	strcpy(ans->m->xmlBuffer.data(), xmlData);

	if (!ans->parseBuffer())
		return nullptr;
	return ans;
}

XMLRoot *XMLRoot::openFromData(const std::string& xmlData)
{
	return XMLRoot::openFromData(xmlData.c_str());
}

void XMLRoot::close(XMLRoot *pRoot)
{
	if (pRoot)
		delete pRoot;
}

} // namespace
} // namespace
