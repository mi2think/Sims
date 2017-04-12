/********************************************************************
	created:	2017/04/12
	created:	12:4:2017   15:41
	filename: 	D:\Code\Sims\Sims\gui\gui_draw_list.h
	file path:	D:\Code\Sims\Sims\gui
	file base:	gui_draw_list
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Gui Draw List
*********************************************************************/
#pragma once

#include "graphics/graphics_fwd.h"

namespace sims
{
	namespace gui
	{
		class SGuiDrawList
		{
		public:
			SGuiDrawList();
			~SGuiDrawList();

			void Clear();
			void PrepareBuf(uint32 vertexCount, uint32 indexCount);

			void AddLine(const Vector2f& p1, const Vector2f& p2, Color c, float thickness = 1.0f);
			void AddLine(float x1, float y1, float x2, float y2, Color c, float thickness = 1.0f);
			//void AddRect(const Rectf& rc, Color c, float thickness = 1.0f);
			//void AddRect(float x, float y, float w, float h, Color c, float thickness = 1.0f);
			//void AddRectFilled(const Rectf& rc, Color c);
			//void AddRectFilled(float x, float y, float w, float h, Color c);
			//void AddTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, Color c, float thickness = 1.0f);
			//void AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, Color c, float thickness = 1.0f);
			void AddPloyLine(const Vector2f* pt, uint32 count, Color c, bool closed, float thickness = 1.0f);

			void PathClear();
			void PathLineTo(const Vector2f& p);
			void PathStroke(Color c, bool closed, float thickness = 1.0f);
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
