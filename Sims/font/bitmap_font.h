/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   15:24
	filename: 	D:\Code\Sims\Sims\font\bitmap_font.h
	file path:	D:\Code\Sims\Sims\font
	file base:	bitmap_font
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Bitmap Font
*********************************************************************/
#ifndef __BITMAP_FONT_H__
#define __BITMAP_FONT_H__

#include "sims.h"
#include "font.h"
#include <unordered_map>
#include <set>

namespace sims
{
	class BitmapFont : public Font
	{
	public:
		BitmapFont(const string& path);

		bool Load(const string& path);

		virtual GlyphRef GetGlyph(wchar_t c) const;

		virtual float GetCharAdvance(wchar_t c) const;
		virtual float GetStringAdvance(const wstring& s) const;

		virtual Recti GetCharBoundingBox(wchar_t c) const;
		virtual Recti GetStringBoundingBox(const wstring& s) const;

		virtual float GetLineHeight() const;

		virtual void DrawChar(FontCanvas&, wchar_t, float, float) const;
		virtual void DrawString(FontCanvas&, const wstring&, float, float) const;
	private:
		struct GlyphParams
		{
			// advance for draw next character
			float xadvance;
			// draw offset
			int32 xoffset;
			int32 yoffset;
			// position of character image in texture
			int32 x;
			int32 y;
			// width and height of character image
			int32 width;
			int32 height;
			// character image id
			int32 page;
		};

		uint32 fontSize_;
		float lineHeight_;
		float imageWidth_;
		float imageHeight_;

		// <page id, TextureRef>
		typedef std::unordered_map<int32, TextureRef> TextureMap;
		TextureMap textureMap_;
		
		// <character, GlyphRef>
		typedef std::unordered_map<wchar_t, GlyphRef> GlyphMap;
		mutable GlyphMap glyphMap_;

		// <character, GlyphParams>
		typedef Ref<GlyphParams> GlyphParamsRef;
		typedef std::unordered_map<wchar_t, GlyphParamsRef> GlyphParamsMap;
		GlyphParamsMap glyphParamsMap_;

		struct Kerning
		{
			wchar_t c1;
			wchar_t c2;
			float amount;

			bool operator<(const Kerning& k) const
			{
				return c1 < k.c1;
			}
		};
		typedef std::multiset<Kerning> KerningSet;
		KerningSet kerningSet_;
	};
}

#endif