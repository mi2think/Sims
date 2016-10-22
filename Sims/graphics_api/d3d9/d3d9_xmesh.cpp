/********************************************************************
	created:	2016/10/08
	created:	8:10:2016   16:18
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_xmesh.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_xmesh
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	X Mesh
*********************************************************************/
#include "d3d9_xmesh.h"

namespace sims
{
	namespace d3d9
	{
		D3D9XMesh::D3D9XMesh(const char* filename)
			: mesh_(nullptr)
		{
			Load(filename);
		}

		D3D9XMesh::~D3D9XMesh()
		{
			Clear();
		}

		void D3D9XMesh::Clear()
		{
			for (auto& tex : textures_)
			{
				SAFE_RELEASE(tex);
			}
			textures_.clear();
			SAFE_RELEASE(mesh_);
		}

		bool D3D9XMesh::Load(const char* filename)
		{
			Clear();

			// load .x file
			ID3DXBuffer* adjBuffer = nullptr;
			ID3DXBuffer* mtrlBuffer = nullptr;
			DWORD numMtrls = 0;

			CHECK_HR = D3DXLoadMeshFromX(filename,
				D3DXMESH_MANAGED,
				d3d9::g_pD3DD,
				&adjBuffer,
				&mtrlBuffer,
				0,
				&numMtrls,
				&mesh_);

			// extract the materials, and load textures
			if (mtrlBuffer != nullptr && numMtrls > 0)
			{
				mtrls_.reserve(numMtrls);

				D3DXMATERIAL* mt = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
				for (uint32 i = 0; i < numMtrls; ++i)
				{
					D3DMATERIAL9& matD3D = mt[i].MatD3D;

					// the MatD3D property does not have an ambient value set when its loaded, so set it now
					matD3D.Ambient = matD3D.Diffuse;

					// save the material
					mtrls_.push_back(matD3D);

					// check if the material has an associative texture
					if (mt[i].pTextureFilename != nullptr)
					{
						// load texture
						IDirect3DTexture9* tex = nullptr;
						CHECK_HR = D3DXCreateTextureFromFile(d3d9::g_pD3DD,
							mt[i].pTextureFilename,
							&tex);
						textures_.push_back(tex);
					}
					else
					{
						textures_.push_back(nullptr);
					}
				}
			}
			SAFE_RELEASE(mtrlBuffer);

			// optimize mesh
			CHECK_HR = mesh_->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
				(DWORD*)adjBuffer->GetBufferPointer(),
				0,
				0,
				0);
			SAFE_RELEASE(adjBuffer);

			return true;
		}

		void D3D9XMesh::Render(D3DXMATRIX* world)
		{
			if (world)
			{
				CHECK_HR = d3d9::g_pD3DD->SetTransform(D3DTS_WORLD, world);
			}

			for (uint32 i = 0; i < mtrls_.size(); ++i)
			{
				auto& mtrl = mtrls_[i];
				CHECK_HR = d3d9::g_pD3DD->SetMaterial(&mtrl);
				CHECK_HR = d3d9::g_pD3DD->SetTexture(0, textures_[i]);
				CHECK_HR = mesh_->DrawSubset(i);
			}
		}
	}
}