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
#include "platform/platform.h"
#include "core/log.h"
#include "core/file_system.h"
#include "core/file_input_stream.h"

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
		using namespace rapidxml;

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
		xml_get_float_attrite(info_node, "face", fontName_);
		xml_get_uint32_attrite(info_node, "size", fontSize_);

		// common
		auto common_node = font_node->first_node("common");
		ASSERT(common_node);
		xml_get_float_attrite(common_node, "lineHeight", lineHeight_);
		uint32 pages = 0;
		xml_get_uint32_attrite(common_node, "lineHeight", pages);

		// pages
		auto pages_node = font_node->first_node("pages");
		ASSERT(pages_node);
		imageMap_.reserve(pages);

		return true;
	}

	GlyphRef BitmapFont::GetGlyph(wchar_t c) const
	{
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