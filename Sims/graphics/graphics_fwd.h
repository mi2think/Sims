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
		Color,
		Depth,
		Stencil
	END_DECLARE_ENUM()

	DECLARE_ENUM(ShaderDomain)
		Vertex,
		Fragment
	END_DECLARE_ENUM()

	DECLARE_ENUM(StorageFlags)
		Local       = BIT(0),
		Hardware    = BIT(1),
		HintDynamic = BIT(2) // hint for hardware, only valid if Hardware set
	END_DECLARE_ENUM()

	typedef void* RenderID;		// generic, platform independent
	typedef void* UniformLoc;	// generic, platform independent

	class Texture;
	class Shader;
	class Program;

	typedef Ref<Texture> TextureRef;
	typedef Ref<Program> ProgramRef;
	typedef Ref<Shader>  ShaderRef;
}

#endif