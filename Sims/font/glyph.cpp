/********************************************************************
	created:	2016/09/22
	created:	22:9:2016   19:28
	filename: 	D:\Code\Sims\Sims\font\glyph.cpp
	file path:	D:\Code\Sims\Sims\font
	file base:	glyph
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Glyph
*********************************************************************/
#include "glyph.h"

namespace sims
{
	Glyph::Glyph(const Font& font,
		wchar_t character,
		const Rectf& bbox,
		const Rectf& uvbox,
		float advance,
		const TextureRef& texture)
		: font_(font)
		, character_(character)
		, bbox_(bbox)
		, uvbox_(uvbox)
		, advance_(advance)
		, texture_(texture)
	{
	}
}