/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   23:21
	filename: 	D:\Code\Sims\Sims\font\font.h
	file path:	D:\Code\Sims\Sims\font
	file base:	font
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Font
*********************************************************************/
#ifndef __FONT_H__
#define __FONT_H__

#include "sims.h"

namespace sims
{
	class Font
	{
	public:
		Font();
		virtual ~Font() {}
	
		uint32 GetID() const { return id_; }
		const string& GetFontName() const { return fontName_; }
		
		void SetFontName(const string& fontName) { fontName_ = fontName; }


	protected:
		uint32 id_;
		string fontName_;
	};
}

#endif