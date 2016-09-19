#include "core/input_stream.h"
#include "core/file_system.h"
#include "core/log.h"
#include "core/image.h"
#include "platform/platform.h"
#include "utils/unit_test.h"
#include "ft2build.h"
#include FT_FREETYPE_H
using namespace sims;

void I_SimpleGlyphLoading()
{
	// 1.init library
	FT_Library library;
	auto e = FT_Init_FreeType(&library);
	ASSERT(e == FT_Err_Ok);

	// 2.loading a font face
	FT_Face face;
	e = FT_New_Face(library, "C:\\Windows\\fonts\\arial.ttf", 0, &face);
	ASSERT(e == FT_Err_Ok);
	LOG_INFO("%d faces embedded in arial", face->num_faces);
	FT_Done_Face(face);

	// 3.loading a font face from memory
	auto inputStream = Platform::GetFileSystem()->OpenInputStream("C:\\Windows\\fonts\\arial.ttf");
	vector<uint8> buffer(inputStream->GetSize());
	inputStream->Read(&buffer[0], buffer.size());
	e = FT_New_Memory_Face(library,
		&buffer[0],
		buffer.size(),
		0,
		&face);
	ASSERT(e == FT_Err_Ok);

	// 4.accessing the face data
	LOG_INFO("font size:%d", face->size);
	LOG_INFO("num glyphs:%d", face->num_glyphs);
	LOG_INFO("num faces:%d", face->num_faces);
	LOG_INFO("face flags:%d", face->face_flags);
	LOG_INFO("units per EM:%d", face->units_per_EM);
	LOG_INFO("num fixed sizes:%d", face->num_fixed_sizes);
	LOG_INFO("available sizes:%d", face->available_sizes);
	LOG_INFO("family name:%s", face->family_name);
	LOG_INFO("style name:%s", face->style_name);
	LOG_INFO("num charmaps:%d", face->num_charmaps);

	// 5.setting the current pixel size
	// FreeType2 uses size objects to model all information related to a given character size for a given face.
	// e.g. a size expressed in 1/64th of a pixel.

	// width and height of character are specified in 1/64th op points.
	// a point is a physical distance, equaling 1/72th of an inch.
	e = FT_Set_Char_Size(face,
		0, // same as character height
		16 * 64,
		300, // dots-per-inch, dpi, standard values are 72 or 96 for display device like the screen
		300);
	ASSERT(e == FT_Err_Ok);

	// 6.loading a glyph image

	// 6.a converting a character code into glyph index
	// normally, an application wants to load a glyph image base on its character code
	// a face object contain one or more tables, called charmaps, to convert character code to glyph indices.

	// by default, when a face object is created, it select a Unicode charmap.
	// to convert a Unicode character code to a font glyph index, use FT_Get_Char_Index

	wchar_t c = L'A';
	e = FT_Select_Charmap(face, FT_ENCODING_GB2312);
	auto index = FT_Get_Char_Index(face, c);

	// 6.b loading a glyph from the face
	// the latter can be stored in various formats within the font file
	// for fixed-size formats(PNT, PCF), each image is a bitmap
	// for scalable formats(TrueType, CFF), use vectorial shapes(outlines) to describe each glyph.

	// glyph image is always stored in glyph slot.
	// face->glyph->format describes format used for storing the glyph image in the slot.
	// if it's not FT_GLYPH_FORMAT_BITMAP, can use FT_Render_Glyph to convert to a bitmap
	// default 256 gray levels, can use FT_RENDER_MODE_MONO to generate 1-bit monochrome bitmap

	e = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
	ASSERT(e == FT_Err_Ok);

	auto& bitmap = face->glyph->bitmap;
	if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		LOG_INFO("num gray:%d", bitmap.num_grays);

		uint8* buffer = (uint8*)bitmap.buffer;
		if (bitmap.pitch < 0)
		{
			buffer -= bitmap.rows * bitmap.pitch;
		}

		ImageRef image(new Image(256, 256, PF_R8G8B8A8));
		uint8* dest = image->GetData();
		for (int i = 0; i < bitmap.rows; ++i)
		{
			for (int j = 0; j < bitmap.width; ++j)
			{
				dest[j * 4] = buffer[j];
				dest[j * 4 + 1] = buffer[j];
				dest[j * 4 + 2] = buffer[j];
				dest[j * 4 + 3] = buffer[j];
			}
			buffer += bitmap.pitch;
			dest += 256 * 4;
		}
		image->SaveTGA("font.tga");
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}




UNIT_TEST(FreeTypeApi)
{
	I_SimpleGlyphLoading();
}