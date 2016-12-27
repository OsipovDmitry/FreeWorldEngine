#include <iostream>
#include <iterator>
#include <algorithm>

#include "core/XML.h"

int main()
{
	using namespace FreeWorldEngine;

	bool ok;
	std::cout << std::boolalpha;
	std::cout << "Testing XML module...\n\n";

	std::cout << "example.xml\n";
	auto xml = XMLRoot::openFromFile("example.xml");

	if (xml) {
		ok = xml->name() == "item_icon_generation";
		std::cout << "Root node name == \"item_icon_generation\": " << ok << "\n";

		ok = xml->children().size() == 12;
		std::cout << "Nested nodes count == 12: " << ok << "\n";

		if (xml->children().size() > 3) {
			XMLNode *fourthNode = *std::next(xml->children().begin(), 3);
			ok = fourthNode->attributeValue("category") == "boots";
			std::cout << "Fourth nested node \"category\" attribute value == \"boots\": " << ok << "\n";

			auto lightNode = fourthNode->child("light");
			if (lightNode) {
				ok = lightNode->attributeValue("angle") == "40";
				std::cout << "Value of \"angle\" attribute of \"light\" nested node == \"40\": " << ok << "\n";
			} else
				std::cout << "Error: nested node with name \"light\" is not found\n";
		}
	} else
		std::cout << "Error reading example.xml\n";
	std::cout << "\n";

	XMLRoot::close(xml);


	std::cout << "incorrect.xml\n";
	xml = XMLRoot::openFromFile("incorrect.xml");

	ok = !xml;
	std::cout << "File wasn't opened: " << ok << "\n";

	XMLRoot::close(xml);


	std::cout << "menu.xml\n";
	xml = XMLRoot::openFromFile("menu.xml");

	if (xml) {
		std::cout << "File content:\n";
		std::cout << xml->attributeValue("caption") << "\n";
		for (XMLNode *child : xml->children()) {
			if (child->name() == "item") {
				std::cout << "\t" << child->text();
				if (child->attributeValue("state") == "disabled")
					std::cout << " (not available)";
				std::cout << "\n";
			}
		}
	} else
		std::cout << "Error reading menu.xml\n";

	XMLRoot::close(xml);

	return 0;
}
