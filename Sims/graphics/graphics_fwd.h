/********************************************************************
	created:	2016/10/22
	created:	22:10:2016   18:39
	filename: 	D:\Code\Sims\Sims\graphics\graphics_fwd.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	graphics_fwd
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Graphics
*********************************************************************/
#ifndef __GRAPHICS_FWD_H__
#define __GRAPHICS_FWD_H__

#include "sims.h"
#include "core/color.h"

namespace sims
{
	DECLARE_ENUM(PixelFormat)
		Unknown,
		A8,
		R8G8B8,
		R8G8B8A8,
		A8R8G8B8,
		FloatRGB,
		FloatRGBA
	END_DECLARE_ENUM()

	DECLARE_ENUM(TextureFilter)
		Point,
		Linear,
		Anisotropic
	END_DECLARE_ENUM()

	DECLARE_ENUM(TextureWrap)
		Reapeat,
		Mirror,
		Clamp,
		Border
	END_DECLARE_ENUM()

	DECLARE_ENUM(ClearFlags)
		Color	= BIT(0),
		Depth	= BIT(1),
		Stencil	= BIT(2)
	END_DECLARE_ENUM()

	DECLARE_ENUM(ShaderDomain)
		Vertex,
		Fragment
	END_DECLARE_ENUM()

	DECLARE_ENUM(UniformDataType)
		Unknown,

		Int,
		Int2,
		Int3,
		Int4,

		F32,
		Vec2,
		Vec3,
		Vec4,

		Mat3,
		Mat4,
		
		Sampler2D,

		Color
	END_DECLARE_ENUM()

	DECLARE_ENUM(VertexStreamElementType)
		S8,
		U8,
		S16,
		U16,
		S32,
		U32,
		F32
	END_DECLARE_ENUM()

	DECLARE_ENUM(VertexStreamUsage)
		Position,
		Normal,
		TexCoord,
		Color
	END_DECLARE_ENUM()

	DECLARE_ENUM(StorageFlags)
		Local       = BIT(0),
		Hardware    = BIT(1),
		HintDynamic = BIT(2) // hint for hardware, only valid if Hardware set
	END_DECLARE_ENUM()

	typedef void* RenderID;		// generic, platform independent
	typedef void* UniformLoc;	// generic, platform independent

	class Texture;
	class TextureResource;
	class Shader;
	class Program;
	class VertexStream;
	class VertexDeclaration;
	class VertexBuffer;
	class VertexBufferResource;
	class IndexBuffer;
	class IndexBufferResource;

	typedef Ref<Texture> TextureRef;
	typedef Ref<TextureResource> TextureResourceRef;
	typedef Ref<Program> ProgramRef;
	typedef Ref<Shader>  ShaderRef;
	typedef Ref<VertexDeclaration> VertexDeclarationRef;
	typedef Ref<VertexBuffer> VertexBufferRef;
	typedef Ref<VertexBufferResource> VertexBufferResourceRef;
	typedef Ref<IndexBuffer> IndexBufferRef;
	typedef Ref<IndexBufferResource> IndexBufferResourceRef;
}

#endif