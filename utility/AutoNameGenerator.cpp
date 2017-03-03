#include <utility/AutoNameGenerator.h>
#include <Types.h>

namespace FreeWorldEngine {

namespace Utility {

struct AutoNameGeneratorPrivate {
	std::string prefix;
	uint64 index;

	AutoNameGeneratorPrivate(const std::string& s) : prefix(s), index(0) {}
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
	return m->prefix + std::to_string((m->index)++);
}

} // namespace

} // namespace
