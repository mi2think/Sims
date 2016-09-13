#include "math/vector2.h"
#include "math/vector3.h"
#include "math/matrix44.h"
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

struct VertexUVN
{
	Vector3f pos;
	Vector3f n;
	Vector2f uv;
};

UNIT_TEST(GeometryGen)
{
	{
		Vertex v[8];
		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(Vertex));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(Vertex, pos));
		vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));
		Matrix44f m;
		MaxtrixTranslation(m, Vector3f(0, 0, 10));
		vb.SetTransform(m);

		uint16 i[36];
		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		bool b1 = GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_Normal);
	}

	{
		VertexUV v[24];
		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUV));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUV, pos));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUV, uv));

		uint16 i[36];
		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		bool b2 = GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV);
	}


	{
		VertexUV v[14];
		uint16 i[36];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUV));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUV, pos));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUV, uv));

		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		bool b3 = GeometryGen::GenSkyBox(2, 2, 2, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV);
	}

	{
		VertexUVN v[4];
		uint16 i[6];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUVN));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUVN, pos));
		vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUVN, uv));

		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		bool b4 = GeometryGen::GenQuad(2, 2, 2, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV | GeometryGen::VT_Normal);
	}

	{
		VertexUVN v[6];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUVN));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUVN, pos));
		vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUVN, uv));

		bool b5 = GeometryGen::GenQuad(2, 2, 2, vb, GeometryGen::VT_Position | GeometryGen::VT_UV | GeometryGen::VT_Normal);
	}

}