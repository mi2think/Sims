#include "utils/unit_test.h"
#include "core/vfs.h"
#include <cstdlib>

using namespace sims;


int main()
{
	VFS::Init();
	UnitTest::GetInstance().Run();
	VFS::Shutdown();
	system("pause");

	return 0;
}
