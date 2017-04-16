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
#include "graphics/texture.h"

namespace sims
{
	namespace ogl
	{
		OGLTextureResource::OGLTextureResource()
			: TextureResource()
			, resource_(GL_INVALID_VALUE)
		{
		}

		OGLTextureResource::~OGLTextureResource()
		{
			InternalReleaseResource();
		}

		void OGLTextureResource::UpdateResource()
		{
			// generate texture object
			if (resource_ == GL_INVALID_VALUE)
			{
				glGenTextures(1, &resource_);
			}

			glBindTexture(GL_TEXTURE_2D, resource_);

			for (uint32 i = 0; i < texture_->GetMipmapCount(); ++i)
			{
				ImageRef image = texture_->GetImage(i);
				if (!image)
					continue;
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const char* src = image->GetConstData();

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

			// set sampling params of texture object
			OnSamplerStatusUpdated();

			gl_check_error("OGLRenderer::UpdateTexture");
		}

		void OGLTextureResource::BindResource() const
		{
			glActiveTexture(GL_TEXTURE0 + bindStage_);
			glBindTexture(GL_TEXTURE_2D, resource_);
			gl_check_error("OGLRenderer::BindTexture");
		}

		void OGLTextureResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLTextureResource::InternalReleaseResource()
		{
			if (resource_ != GL_INVALID_VALUE)
			{
				glDeleteTextures(1, &resource_);
				resource_ = GL_INVALID_VALUE;
			}
		}

		void OGLTextureResource::OnSamplerStatusUpdated()
		{
			glBindTexture(GL_TEXTURE_2D, resource_);

			// set sampling params of texture object
			const auto& samplerStatus = texture_->GetSamplerStatus();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLTextureFilterType(samplerStatus.GetFilterMin()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLTextureFilterType(samplerStatus.GetFilterMin()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLTextureWrapType(samplerStatus.GetWrapS()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLTextureWrapType(samplerStatus.GetWrapT()));
			auto color = samplerStatus.GetBorderColor().GetRGBAVector4();
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&color);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}