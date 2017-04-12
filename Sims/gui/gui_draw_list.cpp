/********************************************************************
	created:	2017/04/12
	created:	12:4:2017   15:43
	filename: 	D:\Code\Sims\Sims\gui\gui_draw_list.cpp
	file path:	D:\Code\Sims\Sims\gui
	file base:	gui_draw_list
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Gui Draw List
*********************************************************************/
#pragma once

#include "gui_draw_list.h"
#include "math/vector2.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"

namespace sims
{
	namespace gui
	{
		SGuiDrawList::SGuiDrawList()
			: vbWritePos_(0)
			, ibWritePos_(0)
		{
			VertexStream streams[3] = 
			{
				VertexStream(VertexStreamUsage::Position, 0, VertexStreamElementType::F32, 3),
				VertexStream(VertexStreamUsage::TexCoord, 0, VertexStreamElementType::F32, 2),
				VertexStream(VertexStreamUsage::Color, 0, VertexStreamElementType::U32, 1),
			};
			VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], count_of(streams));
			vertexBuf_ = new VertexBuffer(vertexDecl);
			indexBuf_ = new IndexBuffer();
		}

		SGuiDrawList::~SGuiDrawList()
		{
		}

		void SGuiDrawList::Clear()
		{
		}

		void SGuiDrawList::PrepareBuf(uint32 vertexCount, uint32 indexCount)
		{
			uint32 vertexSize = vertexBuf_->GetVertexCount();
			vertexBuf_->Resize(vertexSize + vertexCount);
			vbWritePos_ = vertexSize;

			uint32 indexSize = indexBuf_->GetIndexCount();
			indexBuf_->Resize(indexSize + indexCount);
			ibWritePos_ = indexSize;
		}

		void SGuiDrawList::AddLine(const Vector2f& p1, const Vector2f& p2, Color c, float thickness)
		{
			if (c.a == 0)
				return;
		
			PathLineTo(p1 + Vector2f(0.5f, 0.5f));
			PathLineTo(p2 + Vector2f(0.5f, 0.5f));
			PathStroke(c, false);
		}

		void SGuiDrawList::AddLine(float x1, float y1, float x2, float y2, Color c, float thickness)
		{
			AddLine(Vector2f(x1, y1), Vector2f(x2, y2), c);
		}

		void SGuiDrawList::AddPloyLine(const Vector2f* pt, uint32 count, Color c, bool closed, float thickness)
		{
			if (count < 2)
				return;

			// edges of closed line equals point count
			uint32 edges = closed ? count : count - 1;

			// take thickness into account, every edge needs:
			// vertex:4,index:6 for 1 quad.
			uint32 indexCount = edges * 6;
			uint32 vertexCount = edges * 4;
			PrepareBuf(vertexCount, indexCount);

			// for each edge, build 1 quad.
			uint32 vbPos = vbWritePos_;
			uint32 ibPos = ibWritePos_;

			for (uint32 e0 = 0; e0 < edges; ++e0)
			{
				// two points
				uint32 i1 = e0;
				uint32 i2 = i1 + 1;
				if (i2 == count)
					i2 = 0;
				const auto& pt1 = pt[i1];
				const auto& pt2 = pt[i2];
				
				// dx & dy
				auto diff = pt2 - pt1;
				diff.Normalize();
				float dx = diff.x * (thickness * 0.5f);
				float dy = diff.y * (thickness * 0.5f);


				vbPos += 4;

			}
		}

		void SGuiDrawList::PathLineTo(const Vector2f& p)
		{
			path_.push_back(p);
		}

		void SGuiDrawList::PathStroke(Color c, bool closed, float thickness)
		{
			AddPloyLine(&path_[0], path_.size(), closed, thickness);
		}
	}
}
