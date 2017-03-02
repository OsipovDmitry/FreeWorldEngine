#include <utility/AutoNameGenerator.h>
#include <Types.h>

namespace FreeWorldEngine {

namespace Utility {

struct AutoNameGeneratorPrivate {
	std::string prefix;

	AutoNameGeneratorPrivate(const std::string& s) : prefix(s) {}
};

AutoNameGenerator::AutoNameGenerator(const std::string& prefix) :
	m(new AutoNameGeneratorPrivate(prefix))
{
}

AutoNameGenerator::~AutoNameGenerator()
{
	delete m;
}

std::string AutoNameGenerator::generateName()
{
	static uint64 index = 0;
	return m->prefix + std::to_string(index++);
}

} // namespace
} // namespace