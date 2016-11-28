/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   16:14
	filename: 	D:\Code\Sims\Sims\graphics\vertex_buffer.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Vertex Buffer
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "vertex_stream.h"
#include "vertex_declaration.h"

namespace sims
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount);

		const VertexDeclarationRef& GetVertexDeclaration() const;
		int GetVertexCount() const;

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }
	private:
		uint32 storageFlags_;

		bool isLocked_;
	};
}

