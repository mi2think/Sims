/********************************************************************
	created:	2017/04/16
	created:	16:4:2017   8:21
	filename: 	D:\Code\Sims\Sims\gui\gui.cpp
	file path:	D:\Code\Sims\Sims\gui
	file base:	gui
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	SGui
*********************************************************************/
#include "gui.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	namespace gui
	{
		SGuiRenderer::SGuiRenderer()
		{
		}

		void SGuiRenderer::operator()(class SGuiDrawList& drawList)
		{
			auto vb = drawList.GetVB();
			auto ib = drawList.GetIB();

			vb->Invalidate();
			ib->Invalidate();

			auto renderer = hw::GetRenderer();
			renderer->DrawIndexedPrimitive(PrimitiveType::Triangles, *vb->HWResource(), *ib->HWResource(), vb->GetVertexCount(), ib->GetIndexCount() / 3);
		}

		//////////////////////////////////////////////////////////////////////////

		struct Vertex
		{
			typedef Vector3f V3F;
			typedef Vector2f V2F;
			typedef Color C;

			V3F p;
			V2F uv;
			C c;
		};

		SGuiDrawList::SGuiDrawList()
			: vbWritePos_(0)
			, ibWritePos_(0)
		{
			VertexStream streams[3] = 
			{
				VertexStream(VertexStreamUsage::Position, 0, ElementTypeTraits<Vertex::V3F>::Ty, ElementTypeTraits<Vertex::V3F>::N),
				VertexStream(VertexStreamUsage::TexCoord, 0, ElementTypeTraits<Vertex::V2F>::Ty, ElementTypeTraits<Vertex::V2F>::N),
				VertexStream(VertexStreamUsage::Color, 0, ElementTypeTraits<Vertex::C>::Ty, ElementTypeTraits<Vertex::C>::N),
			};
			VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], count_of(streams));
			ASSERT(vertexDecl->GetStride() == sizeof(Vertex));

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
			PathStroke(c, false, thickness);
		}

		void SGuiDrawList::AddLine(float x1, float y1, float x2, float y2, Color c, float thickness)
		{
			AddLine(Vector2f(x1, y1), Vector2f(x2, y2), c, thickness);
		}

		void SGuiDrawList::AddPloyline(const Vector2f* pt, uint32 count, Color c, bool closed, float thickness)
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
			auto* vbL = vertexBuf_->Lock(LockFlags::LockWrite, vbWritePos_, 4);
			auto* ibL = indexBuf_->Lock(LockFlags::LockWrite, ibWritePos_, 6);
			auto vbData = (Vertex*)vbL->GetData();
			auto ibData = (IndexType*)ibL->GetData();

			Vector2f uv{ 0.0f, 0.0f };

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
				float dy = diff.y * (thickness * 0.5f); // why *0.5 ?

				vbData[0].p = Vector3f(pt1.x + dy, pt1.y - dx, 0.0f); vbData[0].uv = uv; vbData[0].c = c;
				vbData[1].p = Vector3f(pt2.x + dy, pt2.y - dx, 0.0f); vbData[1].uv = uv; vbData[1].c = c;
				vbData[2].p = Vector3f(pt2.x - dy, pt2.y + dx, 0.0f); vbData[2].uv = uv; vbData[2].c = c;
				vbData[3].p = Vector3f(pt1.x - dy, pt1.y + dx, 0.0f); vbData[3].uv = uv; vbData[3].c = c;
				vbData += 4;

				ibData[0] = 0; ibData[1] = 1; ibData[2] = 2;
				ibData[3] = 0; ibData[4] = 2; ibData[5] = 3;
				ibData += 6;
			}

			vertexBuf_->Unlock(vbL);
			indexBuf_->Unlock(ibL);
		}

		void SGuiDrawList::PathClear()
		{
			path_.clear();
		}

		void SGuiDrawList::PathLineTo(const Vector2f& p)
		{
			path_.push_back(p);
		}

		void SGuiDrawList::PathStroke(Color c, bool closed, float thickness)
		{
			AddPloyline(&path_[0], path_.size(), c, closed, thickness);
		}
	}
}