#include "core/platform.h"
#include "utils/unit_test.h"
using namespace sims;

UNIT_TEST(Platform)
{
	// GetEnv
	{
		string ev1 = Platform::GetEnv("SystemRoot");
		EXPECT_EQ(ev1, string("C:\\WINDOWS"));
	}

	// SetEnv
	{
		bool b = Platform::SetEnv("test_set_env", "hi");
		EXPECT_TRUE(b);

		string ev1 = Platform::GetEnv("test_set_env");
		EXPECT_EQ(ev1, string("hi"));
	}

	{
		auto& baseDir = Platform::GetBaseDir();
		EXPECT_TRUE(! baseDir.empty());
	}
}
