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
#include "math/math.h"
#include "graphics/hw/vertex_buffer.h"
#include "graphics/hw/index_buffer.h"

namespace sims
{
	namespace utils
	{
		class Model
		{
		public:
			Model(const VertexBufferRef& vb, const IndexBufferRef& ib);

			const VertexBufferRef& GetVertexBuffer() const { return vb_; }
			const IndexBufferRef&  GetIndexBuffer() const { return ib_; }
		private:
			VertexBufferRef	vb_;
			IndexBufferRef	ib_;
		};
		typedef Ref<Model> ModelRef;


		struct GenOptions
		{
			// triangle or lines.
			uint8 bSolid : 1;
			// vertex color
			uint8 bGenColor : 1;
			// normal
			uint8 bGenNormal : 1;
			// UVs
			uint8 bGenTex1 : 1;
			uint8 bGenTex2 : 1;

			Color color;

			GenOptions()
				: bSolid(true)
				, bGenColor(false)
				, bGenNormal(false)
				, bGenTex1(false)
				, bGenTex2(false)
				, color(Color::c_white)

			{
			}

		};

		// face +Y axis
		ModelRef GenPlane(float width, float depth, uint32 slices, uint32 stacks, GenOptions options);
		ModelRef GenCube(float width, float height, float depth, GenOptions options);
		ModelRef GenScreenQuad(float z, GenOptions options);
	}
}

#endif
