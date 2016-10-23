/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   15:50
	filename: 	D:\Code\Sims\Sims\font\bitmap_font.cpp
	file path:	D:\Code\Sims\Sims\font
	file base:	bitmap_font
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Bitmap Font
*********************************************************************/
#include "bitmap_font.h"
#include "glyph.h"
#include "core/path.h"
#include "core/log.h"
#include "core/file_system.h"
#include "core/file_input_stream.h"
#include "graphics/texture.h"
#include "platform/platform.h"
#include "utils/xml_utils.h"

namespace sims
{
	BitmapFont::BitmapFont(const string& path)
		: Font()
		, fontSize_(0)
		, lineHeight_(0)
		, imageWidth_(0)
		, imageHeight_(0)
	{
		Load(path);
	}

	bool BitmapFont::Load(const string& path)
	{
		auto fs = Platform::GetFileSystem();
		if (!fs->FileExists(path))
		{
			LOG_ERROR("file '%s' not exist!");
			return false;
		}
		auto stream = fs->OpenInputStream(path);
		auto fsize = stream->GetSize();
		vector<uint8> buffer(fsize + 1);
		stream->Read(&buffer[0], fsize);
		buffer[fsize] = 0;

		xml_document<> doc;
		doc.parse<0>((char*)&buffer[0]);
		// root
		auto font_node = doc.first_node("font");
		ASSERT(font_node);

		// info
		auto info_node = font_node->first_node("info");
		ASSERT(info_node);
		xml_get_attrite(info_node, "face", fontName_);
		xml_get_attrite(info_node, "size", fontSize_);

		// common
		auto common_node = font_node->first_node("common");
		ASSERT(common_node);
		xml_get_attrite(common_node, "lineHeight", lineHeight_);
		uint32 pages = 0;
		xml_get_attrite(common_node, "pages", pages);
		xml_get_attrite(common_node, "scaleW", imageWidth_);
		xml_get_attrite(common_node, "scaleH", imageWidth_);

		// pages
		auto pages_node = font_node->first_node("pages");
		ASSERT(pages_node);
		textureMap_.reserve(pages);
		string dir = Path::Parent(path);
		for (auto node = pages_node->first_node("page"); node; node = node->next_sibling("page"))
		{
			int32 id = 0;
			string fileName;
			xml_get_attrite(node, "id", id);
			xml_get_attrite(node, "file", fileName);

			string imagePath = Path::Join(dir, fileName);
			if (Path::FileExists(imagePath))
			{
				TextureRef texture(new Texture(Image::FromFile(imagePath, PixelFormat::A8), StorageFlags::Hardware | StorageFlags::HintDynamic));
				textureMap_[id] = texture;
			}
			else
			{
				LOG_ERROR("file '%s' can not find!", imagePath.c_str());
			}
		}

		// chars
		auto chars_node = font_node->first_node("chars");
		ASSERT(chars_node);
		uint32 chars = 0;
		xml_get_attrite(chars_node, "count", chars);
		glyphParamsMap_.reserve(chars);
		for (auto node = chars_node->first_node("char"); node; node = node->next_sibling("char"))
		{
			uint16 id = 0;
			GlyphParamsRef param(new GlyphParams);

			xml_get_attrite(node, "id", id);
			xml_get_attrite(node, "xadvance", param->xadvance);
			xml_get_attrite(node, "xoffset", param->xoffset);
			xml_get_attrite(node, "yoffset", param->yoffset);
			xml_get_attrite(node, "x", param->x);
			xml_get_attrite(node, "y", param->y);
			xml_get_attrite(node, "width", param->width);
			xml_get_attrite(node, "height", param->height);
			xml_get_attrite(node, "page", param->page);

			glyphParamsMap_[id] = param;
		}

		// kerning
		auto kernings_node = font_node->first_node("kernings");
		ASSERT(kernings_node);
		for (auto node = kernings_node->first_node("kerning"); node; node = node->next_sibling("kerning"))
		{
			Kerning k;
			xml_get_attrite(node, "first", k.c1);
			xml_get_attrite(node, "second", k.c2);
			xml_get_attrite(node, "amount", k.amount);
			kerningSet_.insert(k);
		}

		return true;
	}

	GlyphRef BitmapFont::GetGlyph(wchar_t c) const
	{
		// find it, return
		auto it = glyphMap_.find(c);
		if (it != glyphMap_.end())
			return it->second;

		// try create glyph from param
		auto itParam = glyphParamsMap_.find(c);
		if (itParam != glyphParamsMap_.end())
		{
			auto& param = itParam->second;

			// font texture
			auto itTex = textureMap_.find(param->page);
			ASSERT(itTex != textureMap_.end());
			auto& texture = itTex->second;
			if (texture)
			{
				texture->Invalidate();
			}

			Rectf bbox((float)param->xoffset, (float)param->yoffset, (float)param->xoffset + param->width, (float)param->yoffset + param->height);
			Rectf uvbox(param->x / imageWidth_, param->y / imageHeight_, (param->x + param->width) / imageWidth_, (param->y + param->height) / imageHeight_);		
			GlyphRef glyph(new Glyph(*this, c, bbox, uvbox, param->xadvance, texture));

			glyphMap_[c] = glyph;
			return glyph;
		}

		return GlyphRef();
	}

	void BitmapFont::DrawChar(FontCanvas&, wchar_t, float, float) const
	{
		ASSERT(false && "bitmap font on need draw");
	}

	void BitmapFont::DrawString(FontCanvas&, const wstring&, float, float) const
	{
		ASSERT(false && "bitmap font on need draw string");
	}

	float BitmapFont::GetCharAdvance(wchar_t c) const
	{
		auto itParam = glyphParamsMap_.find(c);
		if (itParam != glyphParamsMap_.end())
			return itParam->second->xadvance;

		return 0.0f;
	}

	float BitmapFont::GetStringAdvance(const wstring& s) const
	{
		float len = 0.0f;
		for (auto& c : s)
			len += GetCharAdvance(c);
		return len;
	}

	Recti BitmapFont::GetCharBoundingBox(wchar_t c) const
	{
		auto itParam = glyphParamsMap_.find(c);
		if (itParam != glyphParamsMap_.end())
		{
			auto& param = itParam->second;
			Recti bbox(param->xoffset, param->yoffset, param->xoffset + param->width, param->yoffset + param->height);
			return bbox;
		}
		return Recti();
	}

	Recti BitmapFont::GetStringBoundingBox(const wstring& s) const
	{
		int32 top = 0;
		int32 bottom = 0;
		for (auto& c : s)
		{
			Recti rc = GetCharBoundingBox(c);
			top = MIN(top, rc.top);
			bottom = MAX(bottom, rc.bottom);
		}
		int32 width = (int32)GetStringAdvance(s);

		return Recti(0, top, width, bottom);
	}

	float BitmapFont::GetLineHeight() const
	{
		return lineHeight_;
	}
}