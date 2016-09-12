/********************************************************************
	created:	2016/09/10
	created:	10:9:2016   0:47
	filename: 	D:\Code\Sims\Sims\utils\geometry_gen.h
	file path:	D:\Code\Sims\Sims\utils
	file base:	geometry_gen
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Geometry Generate
*********************************************************************/
#ifndef __GEOMETRY_GEN_H__
#define __GEOMETRY_GEN_H__

#include "sims.h"

namespace sims
{
	class GeometryGen
	{
	public:
		enum VertexType
		{
			VT_Position = BIT(0),
			VT_Normal	= BIT(1),
			VT_UV		= BIT(2),

			VT_Num
		};

		struct VBDesc
		{
			uint8* vb;		// vertex buffer
			uint32 offset;	// offset of vertex buffer
			uint16 stride;	// vertex stride
			uint16 voffsets[VT_Num];

			VBDesc(uint8* _vb, uint16 _stride, uint32 _offset = 0)
				: vb(_vb)
				, stride(_stride)
				, offset(_offset)
			{
				memset(voffsets, 0, sizeof(voffsets));
			}

			void SetVOffset(VertexType vt, uint16 ofs) { ASSERT(vt < VT_Num); voffsets[vt] = ofs; }
		};

		struct IBDesc
		{
			enum IndexType { Index16, Index32 };
			uint8* ib;		// index buffer
			uint16 offset;	// offset of vertex buffer
			IndexType indexType;	// index type

			IBDesc(uint8* _ib, IndexType _indexType, uint16 _offset = 0)
				: ib(_ib)
				, indexType(_indexType)
				, offset(_offset)
			{}
		};

		struct TriIndex16
		{
			uint16 i0;
			uint16 i1;
			uint16 i2;
		};

		struct TriIndex32
		{
			uint32 i0;
			uint32 i1;
			uint32 i2;
		};

		
		// Plane
		// no UV
		// slices number of slices about the x axis
		// stacks number of stacks about the y axis
		static bool GenPlane(float width,
			float height,
			uint32 slices,
			uint32 stacks,
			const VBDesc& vbDesc,
			const IBDesc& ibDesc,
			int vts);

		// Quad
		// 4 vertices, 6 indices(2 triangles)
		// x-z axis
		static bool GenQuad(float width,
			float height,
			float z,
			const VBDesc& vbDesc,
			const IBDesc& ibDesc,
			int vts);

		// Quad
		// 6 vertices(2 triangles)
		// x-z axis,  no index
		static bool GenQuad(float width,
			float height,
			float z,
			const VBDesc& vbDesc,
			int vts);

		// Box
		// 8  vertices, 36 indices(12 triangles) or
		// 24 vertices, 36 indices(12 triangles) if box has UV 
		static bool GenBox(float width,
			float height,
			float depth,
			const VBDesc& vbDesc,
			const IBDesc& ibDesc,
			int vts);

		// sky box
		// 14 vertices, 36 indices(12 triangles)
		// special UV at vertex
		static bool GenSkyBox(float width,
			float height,
			float depth,
			const VBDesc& vbDesc,
			const IBDesc& ibDesc,
			int vts);


		static void GenNormals(uint32 vertexNum,
			uint32 triNum,
			const VBDesc& vbDesc,
			const IBDesc& ibDesc);
	};
}

#endif
