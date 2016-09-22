/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   10:52
	filename: 	D:\Code\Sims\Sims\font\glyph.h
	file path:	D:\Code\Sims\Sims\font
	file base:	glyph
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Glyph
*********************************************************************/
#ifndef __GLYPH_H__
#define __GLYPH_H__

#include "sims.h"
#include "math/rectangle.h"

namespace sims
{
	// a character for render
	class Glyph
	{
	public:
		Glyph(const Font& font, 
			wchar_t character, 
			const Rectf& bbox, 
			const Rectf& uvbox,
			float advance,
			const TextureRef& texture);

		wchar_t GetCharacter() const { return character_; }
		float GetAdvance() const { return advance_; }
		const Rectf& GetBoundingBox() const { return bbox_; }
		const Rectf& GetUVBox() const { return uvbox_; }

		const Font& GetFont() const { return font_; }
		const TextureRef& GetTexture() const { return texture_; }
	private:
		// font of character
		const Font& font_;
		// character code
		wchar_t character_;
		// bounding box for character	
		Rectf bbox_;
		// uv box in texture
		Rectf uvbox_;
		float advance_;
		// texture contain glyph
		TextureRef texture_;
	};
}

#endif