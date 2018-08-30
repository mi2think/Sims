/********************************************************************
	created:	2018/07/16
	created:	16:7:2018   11:14
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\render_resource.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_render_resource
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Render Resource
*********************************************************************/
#include "render_resource.h"
#include "renderer_caps.h"

#include "graphics/hw/texture.h"
#include "graphics/hw/vertex_buffer.h"
#include "graphics/hw/vertex_declaration.h"
#include "graphics/hw/vertex_array.h"
#include "graphics/hw/index_buffer.h"
#include "graphics/hw/shader.h"
#include "graphics/hw/program.h"

namespace sims
{
	namespace d3d9
	{
		D3D9TextureResource::~D3D9TextureResource()
		{
			InternalReleaseResource();
		}

		D3D9TextureResource::D3D9TextureResource()
			: TextureResource()
			, resource_(nullptr)
		{}

		void D3D9TextureResource::UpdateResource()
		{
			ImageRef image = texture_->GetImage(0);
			uint32 bytesPerPixel = image->GetBytesPerPixel();
			uint32 storageFlags = texture_->GetStorageFlags();

			// create resource
			if (!resource_)
			{
				D3DFORMAT format = ToD3DFormat(texture_->GetFormat());
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((storageFlags & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				VERIFYD3DRESULT(g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture_->GetMipmapCount(),
					0,
					format,
					pool,
					&resource_,
					0));
			}

			// update texture
			IDirect3DTexture9* tempResource = resource_;

			// pool is D3DPOOL_DEFAULT. can't lock directly, 
			// create a system memory texture
			if ((storageFlags & StorageFlags::HintDynamic) != 0)
			{
				D3DFORMAT format = ToD3DFormat(texture_->GetFormat());
				VERIFYD3DRESULT(g_pD3DD->CreateTexture(image->GetWidth(),
					image->GetHeight(),
					texture_->GetMipmapCount(),
					0,
					format,
					D3DPOOL_SYSTEMMEM,
					&resource_,
					0));
			}

			// update each surface by mipmap of image
			IDirect3DSurface9* surface = nullptr;
			for (uint32 i = 0; i < tempResource->GetLevelCount(); ++i)
			{
				VERIFYD3DRESULT(tempResource->GetSurfaceLevel(i, &surface));

				image = texture_->GetImage(i);
				uint32 width = image->GetWidth();
				uint32 height = image->GetHeight();
				const char* src = image->GetConstData();

				D3DLOCKED_RECT lockedRect;
				VERIFYD3DRESULT(surface->LockRect(&lockedRect, (RECT*)&regions_[i], D3DLOCK_NOOVERWRITE));
				uint8* dest = (uint8*)lockedRect.pBits;
				for (uint32 j = 0; j < height; ++j)
				{
					memcpy(dest, src, width * bytesPerPixel);
					src += width * bytesPerPixel;
					dest += lockedRect.Pitch;
				}
				VERIFYD3DRESULT(surface->UnlockRect());

				SAFE_RELEASE(surface);
			}

			if (tempResource != resource_)
			{
				VERIFYD3DRESULT(g_pD3DD->UpdateTexture(tempResource, resource_));
				SAFE_RELEASE(tempResource);
			}
		}

		void D3D9TextureResource::BindResource() const
		{
			ASSERT(resource_ != nullptr);

			const auto& samplerStatus = texture_->GetSamplerStatus();

			VERIFYD3DRESULT(g_pD3DD->SetTexture(bindStage_, resource_));
			// filter
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MAGFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMag())));
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MINFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMin())));
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_MIPFILTER, ToD3DTextureFilterType(samplerStatus.GetFilterMip())));
			// wrap
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_ADDRESSU, ToD3DTextureAddress(samplerStatus.GetWrapS())));
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_ADDRESSV, ToD3DTextureAddress(samplerStatus.GetWrapT())));
			VERIFYD3DRESULT(g_pD3DD->SetSamplerState(bindStage_, D3DSAMP_BORDERCOLOR, samplerStatus.GetBorderColor().value));
		}

		void D3D9TextureResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9TextureResource::InternalReleaseResource()
		{
			SAFE_RELEASE(resource_);
		}

		//////////////////////////////////////////////////////////////////////////

		D3D9VertexBufferResource::D3D9VertexBufferResource()
			: VertexBufferResource()
			, vb_(nullptr)
		{}

		D3D9VertexBufferResource::~D3D9VertexBufferResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexBufferResource::UpdateResource()
		{
			if (!vb_)
			{
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((vertexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				VERIFYD3DRESULT(g_pD3DD->CreateVertexBuffer(vertexBuffer_->GetVertexCount() * vertexStream_->GetStride(),
					D3DUSAGE_WRITEONLY,
					0, // using vertex decl
					pool,
					&vb_,
					0));
			}

			// update vertex buffer
			const IndexRange& range = vertexBuffer_->GetInvalidRange();
			auto L = vertexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * vertexStream_->GetStride();
			uint32 dataOffset = L->GetOffset() * vertexStream_->GetStride();

			void* vertices = nullptr;
			VERIFYD3DRESULT(vb_->Lock(
				dataOffset,
				dataSize,
				&vertices,
				D3DLOCK_NOOVERWRITE));
			memcpy(vertices, L->GetData(), dataSize);
			VERIFYD3DRESULT(vb_->Unlock());

			vertexBuffer_->Unlock(L);
		}

		void D3D9VertexBufferResource::BindResource() const
		{
			ASSERT(vb_);

			VERIFYD3DRESULT(g_pD3DD->SetStreamSource(vertexStream_->GetIndex(), vb_, 0, vertexStream_->GetStride()));
		}

		void D3D9VertexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexBufferResource::InternalReleaseResource()
		{
			SAFE_RELEASE(vb_);
		}

		//////////////////////////////////////////////////////////////////////////

		D3D9VertexDeclarationResource::D3D9VertexDeclarationResource()
			: VertexDeclarationResource()
			, decl_(nullptr)
		{}

		D3D9VertexDeclarationResource::~D3D9VertexDeclarationResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexDeclarationResource::UpdateResource()
		{
			if (!decl_)
			{
				uint32 elementCount = 0;
				uint32 streamCount = vertexDecl_->GetStreamCount();
				for (uint32 i = 0; i < streamCount; ++i)
				{
					elementCount += vertexDecl_->GetStream(i)->GetVertexElementCount();
				}

				TBuffer<D3DVERTEXELEMENT9> elements(elementCount + 1); // +1 for D3DDECL_END
				uint32 index = 0;
				for (uint32 i = 0; i < streamCount; ++i)
				{
					const VertexStream* vertexStream = vertexDecl_->GetStream(i);
					for (uint32 j = 0; j < vertexStream->GetVertexElementCount(); ++j)
					{
						FillD3DVertexElement(&elements[index], &vertexStream->GetVertexElement(j), i);
						++index;
					}
				}
				elements[index] = D3DDECL_END();
				VERIFYD3DRESULT(g_pD3DD->CreateVertexDeclaration(elements.GetData(), &decl_));
			}
		}

		void D3D9VertexDeclarationResource::BindResource() const
		{
			ASSERT(decl_);

			VERIFYD3DRESULT(g_pD3DD->SetVertexDeclaration(decl_));
		}

		void D3D9VertexDeclarationResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexDeclarationResource::InternalReleaseResource()
		{
			SAFE_RELEASE(decl_);
		}

		//////////////////////////////////////////////////////////////////////////

		D3D9VertexArrayResource::D3D9VertexArrayResource()
			: VertexArrayResource()
		{
		}

		D3D9VertexArrayResource::~D3D9VertexArrayResource()
		{
		}

		void D3D9VertexArrayResource::UpdateResource()
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			ASSERT(vertexDecl);
			vertexDecl->Invalidate();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				ASSERT(vertexBuffer);
				vertexBuffer->Invalidate();
			}
		}

		void D3D9VertexArrayResource::BindResource() const
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			vertexDecl->Bind();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				vertexBuffer->Bind();
			}
		}

		void D3D9VertexArrayResource::ReleaseResource()
		{
			auto vertexDecl = vertexArray_->GetVertexDeclaration();
			vertexDecl->Release();

			auto count = vertexArray_->GetVertexBufferCount();
			for (uint32 i = 0; i < count; ++i)
			{
				auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
				vertexBuffer->Release();
			}

		}

		//////////////////////////////////////////////////////////////////////////

		template<typename TIndexType> D3DFORMAT ToD3DIndexFormat() { ASSERT(false && "error index type"); return D3DFMT_FORCE_DWORD; }
		template<> D3DFORMAT ToD3DIndexFormat<uint16>() { return D3DFMT_INDEX16; }
		template<> D3DFORMAT ToD3DIndexFormat<uint32>() { return D3DFMT_INDEX32; }


		D3D9IndexBufferResource::D3D9IndexBufferResource()
			: IndexBufferResource()
			, ib_(nullptr)
		{}

		D3D9IndexBufferResource::~D3D9IndexBufferResource()
		{
			InternalReleaseResource();
		}

		void D3D9IndexBufferResource::UpdateResource()
		{
			if (!ib_)
			{
				D3DPOOL pool = D3DPOOL_MANAGED;
				if ((indexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					pool = D3DPOOL_DEFAULT;

				VERIFYD3DRESULT(g_pD3DD->CreateIndexBuffer(sizeof(IndexType) * indexBuffer_->GetIndexCount(),
					D3DUSAGE_WRITEONLY,
					ToD3DIndexFormat<IndexType>(),
					pool,
					&ib_,
					nullptr));
			}

			// update index buffer
			const IndexRange& range = indexBuffer_->GetInvalidRange();
			auto L = indexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * sizeof(IndexType);
			uint32 dataOffset = L->GetOffset() * sizeof(IndexType);

			void* indices = nullptr;
			VERIFYD3DRESULT(ib_->Lock(dataOffset,
				dataSize,
				&indices,
				D3DLOCK_NOOVERWRITE));
			memcpy(indices, L->GetData(), dataSize);
			VERIFYD3DRESULT(ib_->Unlock());

			indexBuffer_->Unlock(L);
		}

		void D3D9IndexBufferResource::BindResource() const
		{
			ASSERT(ib_);
			VERIFYD3DRESULT(g_pD3DD->SetIndices(ib_));
		}

		void D3D9IndexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9IndexBufferResource::InternalReleaseResource()
		{
			SAFE_RELEASE(ib_);
		}

		//////////////////////////////////////////////////////////////////////////

		D3D9ShaderResource::D3D9ShaderResource()
			: ShaderResource()
			, resource_(nullptr)
			, table_(nullptr)
		{}

		D3D9ShaderResource::~D3D9ShaderResource()
		{
			InternalReleaseResource();
		}

		void D3D9ShaderResource::UpdateResource()
		{
			ReleaseResource();

			// compile shader
			ID3DXBuffer* shaderBuffer = nullptr;
			ID3DXBuffer* errorBuffer = nullptr;

			auto type = shader_->GetType();
			const auto& source = shader_->GetSource();

			uint32 flags = D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;
#ifdef SIMS_DEBUG
			flags |= D3DXSHADER_DEBUG;
			flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					shader_->GetEntryName().c_str(),
					g_RendererCaps.GetVSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					shader_->GetEntryName().c_str(),
					g_RendererCaps.GetPSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_));
			}

			if (errorBuffer != nullptr)
			{
				LOG_ERROR("%s", (char*)errorBuffer->GetBufferPointer());
				SAFE_RELEASE(errorBuffer);
				return;
			}

			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(g_pD3DD->CreateVertexShader((DWORD*)shaderBuffer->GetBufferPointer(), &vsResource_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(g_pD3DD->CreatePixelShader((DWORD*)shaderBuffer->GetBufferPointer(), &psResource_));
			}

			AnalyseUniforms();
		}

		void D3D9ShaderResource::BindResource() const
		{
			auto type = shader_->GetType();
			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(g_pD3DD->SetVertexShader(vsResource_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(g_pD3DD->SetPixelShader(psResource_));
			}
		}

		void D3D9ShaderResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9ShaderResource::InternalReleaseResource()
		{
			auto type = shader_->GetType();

			SAFE_RELEASE(table_);
			if (resource_)
			{
				if (type == ShaderDomain::Vertex)
					SAFE_RELEASE(vsResource_);
				else if (type == ShaderDomain::Fragment)
					SAFE_RELEASE(psResource_);

				resource_ = nullptr;
			}

			uniforms_.clear();
		}

		void D3D9ShaderResource::AnalyseUniforms()
		{
			D3DXCONSTANTTABLE_DESC tableDesc;
			VERIFYD3DRESULT(table_->GetDesc(&tableDesc));

			D3DXCONSTANT_DESC cDesc;
			for (uint32 i = 0; i < tableDesc.Constants; ++i)
			{
				uint32 count = 1;
				table_->GetConstantDesc(table_->GetConstant(0, i), &cDesc, &count);

				UniformVar var;
				var.regIndex = cDesc.RegisterIndex;
				var.name = cDesc.Name;
				var.type = cDesc.Type;
				var.hConstant = table_->GetConstant(0, i);

				uniforms_.push_back(var);
			}
		}

		void D3D9ShaderResource::SetUniform(const char* name, const void* data, uint32 dataSize)
		{
			auto it = std::find_if(uniforms_.begin(), uniforms_.end(), [&name](const UniformVar& var) { return var.name == name; });
			ASSERT(it != uniforms_.end());

			if (it->type == D3DXPT_BOOL)
			{
				uint32 count = MAX(1, dataSize / sizeof(BOOL));
				VERIFYD3DRESULT(table_->SetBoolArray(g_pD3DD, it->hConstant, (const BOOL*)data, count));
			}
			else if (it->type == D3DXPT_INT)
			{
				uint32 count = MAX(1, dataSize / sizeof(int));
				VERIFYD3DRESULT(table_->SetIntArray(g_pD3DD, it->hConstant, (const int*)data, count));
			}
			else if (it->type == D3DXPT_FLOAT)
			{
				uint32 count = MAX(1, dataSize / sizeof(float));
				VERIFYD3DRESULT(table_->SetFloatArray(g_pD3DD, it->hConstant, (const float*)data, count));
			}
			else
			{
				LOG_ERROR("D3D9ShaderResource - SetUniform not support type!");
			}
		}

		uint32 D3D9ShaderResource::GetSamplerStage(const char* name)
		{
			for (const auto& var : uniforms_)
			{
				if (name == var.name && var.type >= D3DXPT_SAMPLER)
					return var.regIndex;
			}
			ASSERT(false && "not find sampler!");
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////

		D3D9ProgramResource::D3D9ProgramResource()
			: ProgramResource()
		{
		}

		D3D9ProgramResource::~D3D9ProgramResource()
		{
		}

		void D3D9ProgramResource::UpdateResource()
		{
		}

		void D3D9ProgramResource::BindResource() const
		{
			for (int i = 0; i < ShaderDomain::Max; ++i)
			{
				auto type = ShaderDomain::Type(i);
				auto shader = program_->GetShader(type);
				if (shader)
					shader->Bind();
				else
				{
					switch (type)
					{
					case ShaderDomain::Vertex:
						g_pD3DD->SetVertexShader(nullptr);
						break;
					case ShaderDomain::Fragment:
						g_pD3DD->SetPixelShader(nullptr);
						break;
					default:
						break;
					}
				}
			}
		}

		void D3D9ProgramResource::ReleaseResource()
		{
		}
	}
}