/********************************************************************
	created:	2017/08/14
	created:	14:8:2017   11:26
	filename: 	D:\Code\Sims\Sims\core\platform_def.h
	file path:	D:\Code\Sims\Sims\core
	file base:	platform_def
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Platform Def
*********************************************************************/
#pragma once

// Windows
// _WIN32 is defined by the VC++, Intel and GCC compilers.
// _WIN64 is defined by the VC++, Intel and GCC compilers.
// __WIN32__ is defined by the Borland compiler.
// __INTEL__ is defined by the Metrowerks compiler.
// _M_IX86, _M_AMD64 and _M_IA64 are defined by the VC++, Intel, and Borland compilers.
// _X86_, _AMD64_, and _IA64_ are defined by the Metrowerks compiler.
// _M_ARM is defined by the VC++ compiler.
#if (defined(PLATFORM_WINDOWS)) || (defined(_WIN32)) || (defined(__WIN32__)) || (defined(_WIN64))
#undef	PLATFORM_WINDOWS
#define	PLATFORM_WINDOWS 1
#define	PLATFORM_NAME "Windows"
#ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
#define PLATFORM_WIN64 1
#else
#define PLATFORM_WIN32 1

#define WIN32_LEAN_AND_MEAN
#endif

// Linux
// __linux and __linux__ are defined by the GCC and Borland compiler.
// __i386__ and __intel__ are defined by the GCC compiler.
// __i386__ is defined by the Metrowerks compiler.
// _M_IX86 is defined by the Borland compiler.
// __sparc__ is defined by the GCC compiler.
// __powerpc__ is defined by the GCC compiler.
#elif (defined(PLATFORM_LINUX)) || (defined(__linux)) || (defined(__linux__))
#undef  PLATFORM_LINUX
#define PLATFORM_LINUX 1
#define PLATFORM_UNIX 1
#define PLATFORM_POSIX 1
#define	PLATFORM_NAME "Linux"

// Android
#elif (defined(PLATFORM_ANDROID)) || (defined(__ANDROID__))
#undef  PLATFORM_ANDROID
#define PLATFORM_ANDROID 1
#define PLATFORM_LINUX 1
#define PLATFORM_UNIX 1
#define PLATFORM_POSIX 1
#define	PLATFORM_NAME "Android"
#endif

