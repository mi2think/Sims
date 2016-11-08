/********************************************************************
	created:	2016/11/08
	created:	8:11:2016   14:21
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_program.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_program
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Program
*********************************************************************/
#ifndef __D3D9_PROGRAM_H__
#define __D3D9_PROGRAM_H__

#include "graphics/program.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9Program : public Program
		{
		public:
			D3D9Program();
			~D3D9Program();

			virtual void BindProgram();
			virtual void DeleteProgram();

			virtual UniformLoc VSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const;
			virtual UniformLoc FSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const;
			virtual void VSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type);
			virtual void FSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type);
		private:
			void DeleteProgramInternal();
			void BindUniform(ID3DXConstantTable* table, UniformLoc loc, const void* data, uint32 count, DataType::Type type);
		};
	}
}

#endif