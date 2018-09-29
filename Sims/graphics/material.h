/********************************************************************
	created:	2018/09/25
	created:	25:9:2018   11:14
	filename: 	D:\Code\Sims\Sims\graphics\material.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	material
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Material
	
		In UE4, mainly use a uniform shading model call "default lit", also provide such as "subsurface", "hair",
	etc. Its input in material will change while choose specify shading model(by enable or disable some property).
	UE4 also provide material domain to classify different usage, such as "surface", "post process", "user interface"
	etc.
		For our library, material is more general object: shader-code + render state + uniforms, that's all. And material
	instance has their own override uniforms(subset of uniforms in material). shader-code maybe specify directly, or maybe 
	generate by a Graph-Node-System.
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "hw/shader.h"
#include "hw/program.h"

namespace sims
{
	class Material
	{
	public:

	private:
	};
}

