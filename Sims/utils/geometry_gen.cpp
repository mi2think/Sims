/********************************************************************
	created:	2016/09/10
	created:	10:9:2016   0:48
	filename: 	D:\Code\Sims\Sims\utils\geometry_gen.cpp
	file path:	D:\Code\Sims\Sims\utils
	file base:	geometry_gen
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Geometry Generate
*********************************************************************/
#include "geometry_gen.h"

namespace sims
{
	namespace utils
	{
		struct TriIndex
		{
			IndexType i0;
			IndexType i1;
			IndexType i2;
		};

		Model::Model(const VertexBufferRef& vb, const IndexBufferRef& ib)
			: vb_(vb)
			, ib_(ib)
		{}


		VertexDeclarationRef GetVertexDeclaration(GenOptions options)
		{
			vector<VertexElement> elements;
			elements.reserve(5);
			elements.push_back(VertexElement(VertexElementUsage::Position, 0, VertexDataType::F32, 3));

			if (options.bGenNormal)
			{
				elements.push_back(VertexElement(VertexElementUsage::Normal, 0, VertexDataType::F32, 3));
			}

			if (options.bGenColor)
			{
				elements.push_back(VertexElement(VertexElementUsage::Color, 0, VertexDataType::U8, 4));
			}

			if (options.bGenTex1)
			{
				elements.push_back(VertexElement(VertexElementUsage::TexCoord, 0, VertexDataType::F32, 2));
			}

			if (options.bGenTex1)
			{
				elements.push_back(VertexElement(VertexElementUsage::TexCoord, 1, VertexDataType::F32, 2));
			}

			VertexStream stream(&elements[0], elements.size());
			return VertexDeclaration::Get(&stream, 1);
		}

		void GenNormals(uint32 vertexNum, uint32 triNum, uint32 vertexStride, uint8* pos, uint8* index, uint8* normal)
		{
			uint32* count = new uint32[vertexNum];
			memset(count, 0, vertexNum * sizeof(uint32));

			uint32 triStride = sizeof(IndexType);

			// compute normal
			for (uint32 i = 0; i < triNum; ++i)
			{
				uint32 offset = i * triStride;
				TriIndex* p = (TriIndex*)(index + offset);

				// position
				Vector3f* p0 = (Vector3f*)(pos + (p->i0 * vertexStride));
				Vector3f* p1 = (Vector3f*)(pos + (p->i1 * vertexStride));
				Vector3f* p2 = (Vector3f*)(pos + (p->i2 * vertexStride));

				auto u = *p1 - *p0;
				auto v = *p2 - *p0;
				auto n = CrossProduct(u, v);

				// normal
				Vector3f* n0 = (Vector3f*)(normal + (p->i0 * vertexStride));
				Vector3f* n1 = (Vector3f*)(normal + (p->i1 * vertexStride));
				Vector3f* n2 = (Vector3f*)(normal + (p->i2 * vertexStride));

				*n0 += n;
				*n1 += n;
				*n2 += n;

				++count[p->i0];
				++count[p->i1];
				++count[p->i2];
			}

			// average normal
			for (uint32 i = 0; i < vertexNum; ++i)
			{
				uint32 offset = i * vertexStride;

				Vector3f* n = (Vector3f*)(normal + offset);
				*n /= count[i];
				n->Normalize();
			}

			delete[] count;
		}

		ModelRef GenPlane(float width, float depth, uint32 slices, uint32 stacks, GenOptions options)
		{
			ASSERT(slices > 1 && stacks > 1);

			VertexDeclarationRef decl = GetVertexDeclaration(options);
			uint32 vertexNum = stacks * slices;
			VertexBufferRef vb = new VertexBuffer(decl, vertexNum, 0);

			uint32 triNum = (stacks - 1) * (slices - 1) * 2;
			IndexBufferRef ib = new IndexBuffer(triNum * 3);
			{
				auto Lvb = vb->Lock(LockFlags::LockWrite);
				auto Lib = ib->Lock(LockFlags::LockWrite);

				uint8* pos = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Position);
				uint8* color = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Color);
				uint8* normal = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Normal);
				uint8* tex = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::TexCoord);
				uint32 stride = Lvb->GetStride();

				TriIndex* index = (TriIndex*)Lib->GetData();

				float xStep = width / (slices - 1);
				float zStep = depth / (stacks - 1);
				float xOrigin = -width / 2;
				float zOrigin = -depth / 2;

				float uOrigin = 0.0f;
				float vOrigin = 1.0f;
				float uStep = 1.0f / (slices - 1);
				float vStep = -1.0f / (stacks - 1);

				for (uint32 j = 0; j < stacks; ++j)
				{
					for (uint32 i = 0; i < slices; ++i)
					{
						// write position
						uint32 offset = (j * slices + i) * stride;
						Vector3f* p = (Vector3f*)(pos + offset);
						p->x = xOrigin + i * xStep;
						p->y = 0.0f;
						p->z = zOrigin + j * zStep;

						// write color
						if (color)
						{
							uint32* c = (uint32*)(color + offset);
							*c = options.color.value;
						}

						// write normal
						if (normal)
						{
							Vector3f* n = (Vector3f*)(normal + offset);
							*n = Vector3f(0.0f, 1.0f, 0.0f);
						}

						// write uv
						if (tex)
						{
							Vector2f* uv = (Vector2f*)(tex + offset);
							uv->x = uOrigin + i * uStep;
							uv->y = vOrigin + j * vStep;
						}

						// write index data
						if (j < stacks - 1 && i < slices - 1)
						{
							uint32 triIndex = 2 * ((slices - 1) * j + i);

							TriIndex* p0 = index + triIndex;
							p0->i0 = (IndexType)(j * slices + i);
							p0->i1 = (IndexType)((j + 1) * slices + i + 1);
							p0->i2 = (IndexType)(j * slices + i + 1);

							++p0;
							p0->i0 = (IndexType)(j * slices + i);
							p0->i1 = (IndexType)((j + 1) * slices + i);
							p0->i2 = (IndexType)((j + 1) * slices + i + 1);
						}
					}
				}

				vb->Unlock(Lvb);
				ib->Unlock(Lib);
			}
			ModelRef model = new Model(vb, ib);
			return model;
		}

		ModelRef GenCube(float width, float height, float depth, GenOptions options)
		{
			VertexDeclarationRef decl = GetVertexDeclaration(options);
			uint32 vertexNum = 8 * 3;
			VertexBufferRef vb = new VertexBuffer(decl, vertexNum, 0);

			uint32 triNum = 12;
			IndexBufferRef ib = new IndexBuffer(triNum * 3);
			{
				auto Lvb = vb->Lock(LockFlags::LockWrite);
				auto Lib = ib->Lock(LockFlags::LockWrite);

				uint8* pos = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Position);
				uint8* color = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Color);
				uint8* normal = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Normal);
				//uint8* tex = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::TexCoord);
				uint32 stride = Lvb->GetStride();

				TriIndex* index = (TriIndex*)Lib->GetData();

				/* As generated by follow: first x, next y, last z.
				 * 8 points, each point has 3 vertices.
				 *
				 *       6-------7
				 *      /|      /|
				 *     2-------3 |
				 *	   | 4-----|-5
				 *     |/      |/
				 *     0-------1
				 */

				float xOrigin = -width / 2;
				float yOrigin = -height / 2;
				float zOrigin = -depth / 2;

				for (uint32 k = 0; k < 2; ++k)
				{
					for (uint32 j = 0; j < 2; ++j)
					{
						for (uint32 i = 0; i < 2; ++i)
						{
							uint32 idx = 4 * k + 2 * j + i;
							uint32 offset = idx * stride;

							// write vertex
							Vector3f* p = (Vector3f*)(pos + offset);
							p->x = xOrigin + width * i;
							p->y = yOrigin + height * j;
							p->z = zOrigin + depth * k;

							// write color
							if (color)
							{
								uint32* c = (uint32*)(color + offset);
								*c = Color(255, 0, 0).value;
							}
						}
					}
				}

				// The next two-group vertices.
				memcpy((uint8*)Lvb->GetData() + 8 * stride, Lvb->GetData(), 8 * stride);
				memcpy((uint8*)Lvb->GetData() + 16 * stride, Lvb->GetData(), 8 * stride);

				// generate triangle index
				TriIndex n[36] = {
					0, 3, 1, //front
					0, 2, 3,
					4, 7, 6, //back
					4, 5, 7,
					12,10,8, //left
					12,14,10,
					9, 11,15,//right
					9, 15,13,
					18,23,19,//top
					18,22,23,
					20,17,21,//bottom
					20,16,17
				};
				memcpy(index, n, sizeof(n));

				vb->Unlock(Lvb);
				ib->Unlock(Lib);
			}
			ModelRef model = new Model(vb, ib);
			return model;
		}

		ModelRef GenScreenQuad(float z, GenOptions options)
		{
			VertexDeclarationRef decl = GetVertexDeclaration(options);
			VertexBufferRef vb = new VertexBuffer(decl, 4, 0);

			auto Lvb = vb->Lock(LockFlags::LockWrite);
			uint32 stride = Lvb->GetStride();

			uint8* pos = (uint8*)Lvb->GetDataByUsage(VertexElementUsage::Position);
			Vector3f* p0 = (Vector3f*)(pos + 0 * stride); *p0 = Vector3f(-1, -1, z);
			Vector3f* p1 = (Vector3f*)(pos + 1 * stride); *p1 = Vector3f( 1, -1, z);
			Vector3f* p2 = (Vector3f*)(pos + 2 * stride); *p2 = Vector3f(-1,  1, z);
			Vector3f* p3 = (Vector3f*)(pos + 3 * stride); *p3 = Vector3f( 1,  1, z);

			static IndexType tris[] = { 0, 3, 1, 0, 2, 3 };
			IndexBufferRef ib = new IndexBuffer(6, tris);

			ModelRef model = new Model(vb, ib);
			return model;
		}
	}
}

