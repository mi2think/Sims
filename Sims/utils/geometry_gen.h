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
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"

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


		enum GenOptions
		{
			Gen_Color	= BIT(0),
			Gen_Tex1	= BIT(1),
			Gen_Tex2	= BIT(2),
			Gen_Normal	= BIT(3),
		};
		// face +Y axis
		ModelRef GenPlane(float width, float depth, uint32 slices, uint32 stacks, uint32 options = 0);
	}
}

#endif
