#include <cstring>
#include <fstream>
#include <vector>

#include <utility/XML.h>
#include <3rdparty/rapidxml/rapidxml.hpp>

namespace FreeWorldEngine {
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

XMLRoot *XMLRoot::openFromFile(const std::string& fileName)
{
	XMLRoot *ans = new XMLRoot;

	std::ifstream inFile;
	inFile.open(fileName.c_str());

	if (inFile.fail())
		return nullptr;

	inFile.seekg(0, std::ios::end);
	std::ifstream::pos_type len = inFile.tellg();
	inFile.seekg(0, std::ios::beg);
	ans->m->xmlBuffer.resize(static_cast<std::vector<char>::size_type>(len), 0);
	inFile.read(ans->m->xmlBuffer.data(), len);
	inFile.close();

	if (!ans->parseBuffer())
		return nullptr;

	return ans;
}

XMLRoot *XMLRoot::openFromData(const char *xmlData)
{
	XMLRoot *ans = new XMLRoot;

	ans->m->xmlBuffer.resize(strlen(xmlData) + 1, 0);
	strcpy_s(ans->m->xmlBuffer.data(), ans->m->xmlBuffer.size(), xmlData);

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
