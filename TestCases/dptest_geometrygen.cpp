#include "math/vector3.h"
#include "utils/geometry_gen.h"
#include "utils/unit_test.h"
using namespace sims;

struct Vertex
{
	Vector3f pos;
	Vector3f n;
};

UNIT_TEST(GeometryGen)
{
	Vertex v[8];
	GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(Vertex));
	vb.SetVOffset(GeometryGen::VT_Position, offsetof(Vertex, pos));
	vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));

	uint16 i[36];
	GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

	bool b = GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_Normal);

}