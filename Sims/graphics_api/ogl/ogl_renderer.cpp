/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:16
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Renderer
*********************************************************************/
#include "ogl_renderer.h"
#include "ogl_common.h"
#include "graphics/texture.h"

namespace sims
{
	namespace ogl
	{
		void OGLRenderer::BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			if ((clearFlags & ClearFlags::Color) != 0)
			{
				auto c = color.GetRGBAVector4();
				glClearColor(c.x, c.y, c.z, c.w);
			}
			if ((clearFlags & ClearFlags::Depth) != 0)
			{
				glClearDepth(depth);
			}
			if ((clearFlags & ClearFlags::Stencil) != 0)
			{
				glClearStencil(stencil);
			}
		}

		void OGLRenderer::EndFrame()
		{
		}

		void OGLRenderer::PresentFrame()
		{
			glut_swap_buffers();
		}

		void OGLRenderer::UpdateTexture(Texture& texture, Recti* regions)
		{
			GLuint id = (GLuint)texture.GetRenderID();

			// generate texture object
			if (!id)
			{
				glGenTextures(1, &id);
				texture.SetRenderID((RenderID)id);
			}

			glBindTexture(GL_TEXTURE_2D, id);

			for (uint32 i = 0; i < texture.GetMipmapCount(); ++i)
			{
				ImageRef image = texture.GetImage(i);
				if (! image)
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

			gl_check_error("OGLRenderer::UpdateTexture");

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void OGLRenderer::BindTexture(Texture& texture, uint32 textureUnit)
		{
			GLuint id = (GLuint)texture.GetRenderID();
			glActiveTexture(textureUnit);
			glBindTexture(GL_TEXTURE_2D, id);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLTextureFilterType(texture.GetFilterMin()));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLTextureFilterType(texture.GetFilterMin()));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLTextureWrapType(texture.GetWrapS()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLTextureWrapType(texture.GetWrapT()));
			
			gl_check_error("OGLRenderer::BindTexture");
		}

		void OGLRenderer::DeleteTexture(Texture& texture)
		{
			GLuint id = (GLuint)texture.GetRenderID();
			glDeleteTextures(1, &id);
			texture.SetRenderID(0);
		}
	}
}