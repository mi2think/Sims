#include "math/vector3.h"
#include "utils/unit_test.h"
using namespace sims;

UNIT_TEST(Vector3)
{
	Vector3f v1(1.0f, 2.0f, 3.0f);
	Vector3f v2(1.0f, 2.0f, 3.0f);
	Vector3f v3 = v1 + v2;

	EXPECT_TRUE(v3 == v1 + v2);
	EXPECT_FALSE(v3 == Vector3f(2.0f,3.0f,6.0f));

	Vector3f v4 = -v1;
	EXPECT_EQ(v1, -v4);

	Vector3f v5 = 3.5f * v1;
	EXPECT_EQ(v5, Vector3f(3.5f, 7.0f, 10.5f));

	Vector3f v6 = v2 / 2.0f;
	EXPECT_EQ(v6, Vector3f(0.5f, 1.0f, 1.5f));

	v6.Zero();
	EXPECT_EQ(v6, Vector3f(0,0,0));

	Vector3f v7 = v2;
	v7.Normalize();
	EXPECT_TRUE(abs(v7.LengthSQ() - 1.0f) < 1e-5);

	float dp = DotProduct(v1, v2);
	EXPECT_TRUE(abs(dp - 14.0f) < 1e-5);

	Vector3f v8 = CrossProduct(Vector3f(1,0,0), Vector3f(0, 0,1));
	EXPECT_EQ(v8, Vector3f(0.0f, -1.0f, 0.0f));

	float fAngle = VectorAngle(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 1.0f));
	GLOGS << fAngle << "\n";
	EXPECT_TRUE(equal_t(fAngle, 45.0f));

	Vector3f v9(1.0f, 0.5f, 0.0f);
	Vector3f pj = Projection(v9, Vector3f(1.0f, 0.0f, 0.0f));
	EXPECT_EQ(pj, Vector3f(1.0f, 0.0f, 0.0f));
}

