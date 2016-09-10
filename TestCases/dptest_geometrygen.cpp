#include "math/vector2.h"
#include "math/vector3.h"
#include "utils/geometry_gen.h"
#include "utils/unit_test.h"
using namespace sims;

struct Vertex
{
	Vector3f pos;
	Vector3f n;
};

struct VertexUV
{
	Vector3f pos;
	Vector2f uv;
};

UNIT_TEST(GeometryGen)
{
	Vertex v[8];
	GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(Vertex));
	vb.SetVOffset(GeometryGen::VT_Position, offsetof(Vertex, pos));
	vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));

	uint16 i[36];
	GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

	bool b1 = GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_Normal);

	VertexUV vuv[14];
	uint16 iuv[36];

	GeometryGen::VBDesc vbuv((uint8*)&vuv[0], sizeof(VertexUV));
	vbuv.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUV, pos));
	vbuv.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUV, uv));

	GeometryGen::IBDesc ibuv((uint8*)&iuv[0], GeometryGen::IBDesc::Index16);

	bool b2 = GeometryGen::GenUVCube(2, vbuv, ibuv, GeometryGen::VT_Position | GeometryGen::VT_UV);
}