/********************************************************************
	created:	2016/10/08
	created:	8:10:2016   16:18
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\xmesh_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	xmesh_dx9
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	X Mesh
*********************************************************************/
#ifndef __X_MESH_H__
#define __X_MESH_H__

#include "sims.h"
#include "common_dx9.h"

namespace sims
{
	namespace dx9
	{
		class XMesh
		{
		public:
			XMesh(const char* filename);
			~XMesh();

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
