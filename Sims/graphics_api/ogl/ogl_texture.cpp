/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   17:21
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_texture.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_texture
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	OGL Texture
*********************************************************************/
#include "ogl_texture.h"

namespace sims
{
	namespace ogl
	{
		OGLTexture::OGLTexture()
			: Texture()
			, tex_(GL_INVALID_VALUE)
		{
		}

		OGLTexture::OGLTexture(uint32 width, uint32 height, PixelFormat::Type format, uint32 storgeFlags)
			: Texture(width, height, format, storgeFlags)
			, tex_(GL_INVALID_VALUE)
		{
		}

		OGLTexture::OGLTexture(const string& path, PixelFormat::Type format, uint32 storgeFlags)
			: Texture(path, format, storgeFlags)
			, tex_(GL_INVALID_VALUE)
		{
		}

		OGLTexture::OGLTexture(const ImageRef& image, uint32 storgeFlags)
			: Texture(image, storgeFlags)
			, tex_(GL_INVALID_VALUE)
		{
		}

		OGLTexture::~OGLTexture()
		{
			if (tex_ != GL_INVALID_VALUE)
			{
				glDeleteTextures(1, &tex_);
			}
		}

		void OGLTexture::HWUpdateTexture(Recti* regions)
		{
			// generate texture object
			if (!tex_)
			{
				glGenTextures(1, &tex_);
			}

			glBindTexture(GL_TEXTURE_2D, tex_);

			for (uint32 i = 0; i < mipmapCount_; ++i)
			{
				ImageRef image = GetImage(i);
				if (!image)
					continue;
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const uint8* src = image->GetConstData();

				//TODO: support more pixel format & region
				auto format = image->GetFormat();
				ASSERT(format == PixelFormat::R8G8B8A8);

				glTexImage2D(GL_TEXTURE_2D,
					i,
					GL_RGBA,
					width,
					height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					(const GLvoid *)src);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			gl_check_error("OGLRenderer::UpdateTexture");
		}

		void OGLTexture::HWBindTexture(uint32 textureUnit)
		{
			glActiveTexture(textureUnit);
			glBindTexture(GL_TEXTURE_2D, tex_);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLTextureFilterType(samplerStatus_.GetFilterMin()));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLTextureFilterType(samplerStatus_.GetFilterMin()));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLTextureWrapType(samplerStatus_.GetWrapS()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLTextureWrapType(samplerStatus_.GetWrapT()));

			auto color = samplerStatus_.GetBorderColor().GetRGBAVector4();
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&color);

			gl_check_error("OGLRenderer::BindTexture");
		}

		void OGLTexture::HWDeleteTexture()
		{
			if (tex_ != GL_INVALID_VALUE)
			{
				glDeleteTextures(1, &tex_);
			}
		}
	}
}