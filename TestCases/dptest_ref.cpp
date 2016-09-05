#include "core/ref.h"
#include "utils/unit_test.h"
using namespace sims;

class Base
{
public:
	virtual ~Base() {}

	virtual int GetId() const { return 1; }
};

class Derived : public Base
{
public:
	virtual int GetId() const override { return 2; }
};


UNIT_TEST(Ref)
{
	Ref<int> r1(new int(1));
	EXPECT_TRUE(r1.Count() == 1);

	{
		Ref<int> r2 = r1;
		EXPECT_TRUE(r1.Count() == 2);
	}
	EXPECT_TRUE(r1.Count() == 1);

	Ref<Base> r3(new Base());
	EXPECT_TRUE(r3->GetId() == 1);
	EXPECT_TRUE(r3.Count() == 1);

	{
		Ref<Base> r4(new Derived());
		EXPECT_TRUE(r4->GetId() == 2);
		EXPECT_TRUE(r4.Count() == 1);

		r3 = r4;
		EXPECT_TRUE(r3->GetId() == 2);
		EXPECT_TRUE(r3.Count() == 2);
		EXPECT_TRUE(r4.Count() == 2);

		Ref<Derived> r5 = r3.As<Derived>();
		EXPECT_TRUE(r5->GetId() == 2);
		EXPECT_TRUE(r3.Count() == 3);
		EXPECT_TRUE(r4.Count() == 3);
		EXPECT_TRUE(r5.Count() == 3);
	}

	EXPECT_TRUE(r3->GetId() == 2);
	EXPECT_TRUE(r3.Count() == 1);

	{
		Ref<char> ch(new char[10], &default_array_destory<char>);
	}
}