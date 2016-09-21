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
#include "math/rectangle.h"

namespace sims
{
	// canvas for font draw, usually using a bitmap
	class FontCanvas
	{
	public:
		virtual ~FontCanvas() {}

		virtual void Reset() = 0;
		virtual void Clear() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void InvalidateRegion(const Recti&) = 0;
	};

	class Font
	{
	public:
		Font();
		virtual ~Font() {}
	
		uint32 GetID() const { return id_; }
		const string& GetFontName() const { return fontName_; }		
		void SetFontName(const string& fontName) { fontName_ = fontName; }

		// for font self contain a texture region. e.g. bitmap fonts
		virtual GlyphRef GetGlyph(wchar_t c) const = 0;
		
		// for font need draw
		virtual void DrawChar(FontCanvas& canvas, wchar_t c, float x, float y) const = 0;
		virtual void DrawString(FontCanvas& canvas, const wstring& s, float x, float y) const = 0;

		// distance from character to next
		virtual float GetCharAdvance(wchar_t c) const = 0;
		virtual float GetStringAdvance(const wstring& s) const = 0;

		// bounding box for draw
		virtual Recti GetCharBoundingBox(wchar_t c) const = 0;
		virtual Recti GetStringBoundingBox(const wstring& s) const = 0;

		virtual float GetLineHeight() const = 0;
	protected:
		uint32 id_;
		string fontName_;
	};
}

#endif