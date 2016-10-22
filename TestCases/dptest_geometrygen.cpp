#include "math/vector2.h"
#include "math/vector3.h"
#include "math/matrix44.h"
#include "utils/geometry_gen.h"
#include "utils/unit_test.h"
#include "graphics_api/sims_sdk_d3d9.h"
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

		GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_Normal);
	}

	{
		VertexUV v[24];
		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUV));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUV, pos));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUV, uv));

		uint16 i[36];
		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		GeometryGen::GenBox(2, 4, 6, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV);
	}


	{
		VertexUV v[14];
		uint16 i[36];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUV));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUV, pos));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUV, uv));

		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		GeometryGen::GenSkyBox(2, 2, 2, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV);
	}

	{
		VertexUVN v[4];
		uint16 i[6];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUVN));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUVN, pos));
		vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUVN, uv));

		GeometryGen::IBDesc ib((uint8*)&i[0], GeometryGen::IBDesc::Index16);

		GeometryGen::GenQuad(2, 2, 2, vb, ib, GeometryGen::VT_Position | GeometryGen::VT_UV | GeometryGen::VT_Normal);
	}

	{
		VertexUVN v[6];

		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUVN));
		vb.SetVOffset(GeometryGen::VT_Position, offsetof(VertexUVN, pos));
		vb.SetVOffset(GeometryGen::VT_Normal, offsetof(Vertex, n));
		vb.SetVOffset(GeometryGen::VT_UV, offsetof(VertexUVN, uv));

		GeometryGen::GenQuad(2, 2, 2, vb, GeometryGen::VT_Position | GeometryGen::VT_UV | GeometryGen::VT_Normal);
	}

	{

		VertexUV v[16];
		uint16 i[54];
		GeometryGen::VBDesc vb((uint8*)&v[0], sizeof(VertexUV), 0, offsetof(VertexUV, uv));

		GeometryGen::GenPlane(2, 4, 4, 4, vb, GeometryGen::IBDesc((uint8*)&i[0], GeometryGen::IBDesc::Index16), GeometryGen::VT_Position | GeometryGen::VT_UV);
	}


	{
		d3d9::GeoBase* quad = new d3d9::GeoQuad<d3d9::VertexN>();
		delete quad;

		d3d9::GeoBase* plane = new d3d9::GeoPlane<d3d9::VertexUVN>();
		delete plane;

		d3d9::GeoBase* box = new d3d9::GeoBox<d3d9::Vertex>();
		delete box;

		d3d9::GeoBase* skybox = new d3d9::GeoSkyBox<d3d9::VertexUVN>();
		delete skybox;
	}
}