#include <core/ICore.h>

int main(int argc, char *argv[])
{
	FreeWorldEngine::ICore::instance()->initialize(argc, argv);
    return 0;
}
