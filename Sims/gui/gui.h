/********************************************************************
	created:	2017/04/11
	created:	11:4:2017   11:00
	filename: 	D:\Code\Sims\Sims\gui\gui.h
	file path:	D:\Code\Sims\Sims\gui
	file base:	gui
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	SGui
*********************************************************************/
#pragma once

#include "graphics/graphics_fwd.h"
#include "math/vector2.h"
#include "math/vector3.h"

namespace sims
{
	namespace gui
	{
		class SGuiRenderer
		{
		public:
			SGuiRenderer();

			void operator()(class SGuiDrawList& drawList);
		};

		class SGuiDrawList
		{
		public:
			SGuiDrawList();
			~SGuiDrawList();

			void Clear();
			void PrepareBuf(uint32 vertexCount, uint32 indexCount);

			void AddLine(const Vector2f& p1, const Vector2f& p2, Color c, float thickness = 1.0f);
			void AddLine(float x1, float y1, float x2, float y2, Color c, float thickness = 1.0f);
			void AddPloyline(const Vector2f* pt, uint32 count, Color c, bool closed, float thickness = 1.0f);

			void PathClear();
			void PathLineTo(const Vector2f& p);
			void PathStroke(Color c, bool closed, float thickness = 1.0f);

			VertexBufferRef GetVB() const { return vertexBuf_; }
			IndexBufferRef  GetIB() const { return indexBuf_; }
		private:
			// for render
			VertexBufferRef vertexBuf_;
			IndexBufferRef  indexBuf_;
			uint32 vbWritePos_;
			uint32 ibWritePos_;

			// internal data
			vector<Vector2f> path_;
		};
	}
}
