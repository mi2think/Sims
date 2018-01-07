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

	DECLARE_ENUM(DataType)
		S8,
		U8,
		S16,
		U16,
		S32,
		U32,
		F32
	END_DECLARE_ENUM()

	DECLARE_ENUM(VertexElementUsage)
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

	DECLARE_ENUM(Transform)
		World,
		View,
		Projection
	END_DECLARE_ENUM()

	DECLARE_ENUM(PrimitiveType)
		Points,
		Lines,
		Triangles
	END_DECLARE_ENUM()

	typedef void* RenderID;		// generic, platform independent
	typedef void* UniformLoc;	// generic, platform independent

	class RenderResource;
	class Texture;
	class TextureResource;
	class VertexElement;
	class VertexStream;
	class VertexDeclaration;
	class VertexDeclarationResource;
	class VertexBuffer;
	class VertexBufferResource;
	class IndexBuffer;
	class IndexBufferResource;
	class Shader;
	class ShaderResource;
	class Program;
	class ProgramResource;

	typedef Ref<Texture> TextureRef;
	typedef Ref<TextureResource> TextureResourceRef;
	typedef Ref<VertexDeclaration> VertexDeclarationRef;
	typedef Ref<VertexDeclarationResource> VertexDeclarationResourceRef;
	typedef Ref<VertexBuffer> VertexBufferRef;
	typedef Ref<VertexBufferResource> VertexBufferResourceRef;
	typedef Ref<IndexBuffer> IndexBufferRef;
	typedef Ref<IndexBufferResource> IndexBufferResourceRef;
	typedef Ref<Shader>  ShaderRef;
	typedef Ref<ShaderResource> ShaderResourceRef;
	typedef Ref<Program> ProgramRef;
	typedef Ref<ProgramResource> ProgramResourceRef;

	template<typename T>
	struct ElementTypeTraits {};
	template<> struct ElementTypeTraits<Color>    { static const DataType::Type Ty = DataType::U32; static const uint32 N = 1; };
	template<> struct ElementTypeTraits<Vector2f> { static const DataType::Type Ty = DataType::F32; static const uint32 N = 2; };
	template<> struct ElementTypeTraits<Vector3f> { static const DataType::Type Ty = DataType::F32; static const uint32 N = 3; };

	inline uint32 GetVertexNumPerPrimitive(PrimitiveType::Type primitive)
	{
		uint32 num = 0;
		switch (primitive)
		{
		case PrimitiveType::Triangles:
			num = 3;
			break;
		case PrimitiveType::Lines:
			num = 2;
			break;
		case PrimitiveType::Points:
			num = 1;
			break;
		default:
			ASSERT(0);
			break;
		}
		return num;
	}
}

#endif