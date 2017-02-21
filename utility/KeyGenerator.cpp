#include <set>

#include <utility/KeyGenerator.h>

namespace FreeWorldEngine {

namespace Utility {

struct KeyGeneratorPrivate {
	uint32 startId;
	uint32 numIds;
	std::set<uint32> freeIds;

	KeyGeneratorPrivate(const uint32 start) : startId(start), numIds(0), freeIds() {}
};

KeyGenerator::KeyGenerator(const uint32 startId) :
	m(new KeyGeneratorPrivate(startId))
{
}

KeyGenerator::~KeyGenerator()
{
	delete m;
}

uint32 KeyGenerator::generateId()
{
	if (m->freeIds.empty())
		return m->startId + (m->numIds)++;

	uint32 res = *(m->freeIds.begin());
	m->freeIds.erase(res);
	return res;
}

void KeyGenerator::clearId(const uint32 id)
{
	m->freeIds.insert(id);
}


} // namesapce
} // namespace