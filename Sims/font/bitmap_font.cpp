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
#include "core/image.h"
#include "core/path.h"
#include "core/log.h"
#include "core/file_system.h"
#include "core/file_input_stream.h"
#include "platform/platform.h"
#include "utils/xml_utils.h"

namespace sims
{
	BitmapFont::BitmapFont(const string& path)
		: Font()
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
		xml_get_attrite(common_node, "lineHeight", pages);

		// pages
		auto pages_node = font_node->first_node("pages");
		ASSERT(pages_node);
		imageMap_.reserve(pages);
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
				ImageRef image = Image::FromFile(imagePath, PF_A8);
				imageMap_[id] = image;
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
		auto it2 = glyphParamsMap_.find(c);
		if (it2 != glyphParamsMap_.end())
		{
			auto& param = it2->second;
		}


		return GlyphRef();
	}

	void BitmapFont::DrawChar(FontCanvas& canvas, wchar_t c, float x, float y) const
	{

	}

	void BitmapFont::DrawString(FontCanvas& canvas, const wstring& s, float x, float y) const
	{

	}

	float BitmapFont::GetCharAdvance(wchar_t c) const
	{
		return 0.0f;
	}

	float BitmapFont::GetStringAdvance(const wstring& s) const
	{
		return 0.0f;
	}

	Recti BitmapFont::GetCharBoundingBox(wchar_t c) const
	{
		return Recti();
	}

	Recti BitmapFont::GetStringBoundingBox(const wstring& s) const
	{
		return Recti();
	}

	float BitmapFont::GetLineHeight() const
	{
		return lineHeight_;
	}
}