/********************************************************************
	created:	2016/11/29
	created:	29:11:2016   11:36
	filename: 	D:\Code\Sims\Sims\core\core.h
	file path:	D:\Code\Sims\Sims\core
	file base:	type
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Type & Common macros
*********************************************************************/
#pragma once

#include <cassert>
#include <cstring> // memcpy

#define ASSERT	assert
#define BIT(x)	(1<<(x))
#define ARRAY_COUNT(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#define SAFE_DELETE(p)		do { if (p != nullptr) { delete p; p = nullptr; } } while(0)
#define SAFE_DELETEARRAY(p) do { if (p != nullptr) { delete[] p; p = nullptr; } } while(0)
#define SAFE_RELEASE(p)		do { if (p != nullptr) { p->Release(); p = nullptr; } } while(0)

#ifndef NDEBUG
	#define SIMS_DEBUG 1 
#endif

namespace sims
{
	typedef unsigned char		uint8;
	typedef unsigned short int	uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;
	typedef signed char			int8;
	typedef signed short int	int16;
	typedef signed int			int32;
	typedef signed long long	int64;
}
