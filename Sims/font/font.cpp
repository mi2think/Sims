/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   14:58
	filename: 	D:\Code\Sims\Sims\font\font.cpp
	file path:	D:\Code\Sims\Sims\font
	file base:	font
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Font
*********************************************************************/
#include "font.h"

namespace sims
{
	static uint32 GetFontID() { static uint32 id = 0; return ++id; }

	Font::Font()
		: id_(GetFontID())
	{}
}