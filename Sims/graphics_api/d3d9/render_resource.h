/********************************************************************
	created:	2018/07/16
	created:	16:7:2018   11:10
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\render_resource.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_render_resource
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Render Resource
*********************************************************************/
#pragma once

#include "graphics/hw/render_resource.h"
#include "common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9TextureResource : public TextureResource
		{
		public:
			D3D9TextureResource();
			~D3D9TextureResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			IDirect3DTexture9* resource_;
		};

		class D3D9VertexBufferResource : public VertexBufferResource
		{
		public:
			D3D9VertexBufferResource();
			~D3D9VertexBufferResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			IDirect3DVertexBuffer9* vb_;
		};

		class D3D9VertexDeclarationResource : public VertexDeclarationResource
		{
		public:
			D3D9VertexDeclarationResource();
			~D3D9VertexDeclarationResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			IDirect3DVertexDeclaration9* decl_;
		};

		class D3D9VertexArrayResource : public VertexArrayResource
		{
		public:
			D3D9VertexArrayResource();
			~D3D9VertexArrayResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		};

		class D3D9IndexBufferResource : public IndexBufferResource
		{
		public:
			D3D9IndexBufferResource();
			~D3D9IndexBufferResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			IDirect3DIndexBuffer9* ib_;
		};

		class D3D9ShaderResource : public ShaderResource
		{
		public:
			D3D9ShaderResource();
			~D3D9ShaderResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

			virtual void SetUniform(const char* name, const void* data, uint32 dataSize);

			uint32 GetSamplerStage(const char* name);
		private:
			void AnalyseUniforms();
			void InternalReleaseResource();
			union
			{
				void* resource_;
				IDirect3DVertexShader9* vsResource_;
				IDirect3DPixelShader9*  psResource_;
			};
			ID3DXConstantTable* table_;

			struct UniformVar
			{
				uint32 regIndex;
				string name;
				D3DXPARAMETER_TYPE type;
				D3DXHANDLE hConstant;
			};
			vector<UniformVar> uniforms_;
		};

		class D3D9ProgramResource : public ProgramResource
		{
		public:
			D3D9ProgramResource();
			~D3D9ProgramResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		};
	}
}