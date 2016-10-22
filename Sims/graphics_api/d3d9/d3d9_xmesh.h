/********************************************************************
	created:	2016/10/08
	created:	8:10:2016   16:18
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_xmesh.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_xmesh
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	X Mesh
*********************************************************************/
#ifndef __X_MESH_H__
#define __X_MESH_H__

#include "sims.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9XMesh
		{
		public:
			D3D9XMesh(const char* filename);
			~D3D9XMesh();

			void Clear();
			
			bool Load(const char* filename);

			void Render(D3DXMATRIX* world);
		private:
			ID3DXMesh* mesh_;
			vector<D3DMATERIAL9> mtrls_;
			vector<IDirect3DTexture9*> textures_;
		};
	}
}

#endif
