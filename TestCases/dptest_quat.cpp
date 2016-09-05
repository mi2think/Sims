#include "math/vector3.h"
#include "math/quat.h"
#include "utils/unit_test.h"
using namespace sims;

Vector3f RotateVector(const Vector3f& v, const Vector3f& axis, float angle)
{
	Vector3f _v = QuatRotateVector(v, axis, angle);
	return _v;
}


UNIT_TEST(Quat)
{
	{
		Vector3f axis(0, 1, 0);
		Vector3f v(1, 0, 0);

		Vector3f _v = RotateVector(v, axis, angle2radian(90));
		EXPECT_EQ(_v, Vector3f(0, 0, 1));
	}

	{
		Quat q1(-0.448117137f, -0.604671359f, -0.420344472f, 0.506828427f);
		Quat q2(-0.0860005245f, -0.00377009483f, -0.00102233898f, 0.996287525f);
		Quat q3 = q1 * q2;
		Quat q4(-0.48907447f, -0.64002907f, -0.36898950f, 0.46369913f);
		EXPECT_EQ(q3, q4);
	}
}
