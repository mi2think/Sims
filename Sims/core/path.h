/********************************************************************
	created:	2016/09/17
	created:	17:9:2016   22:13
	filename: 	D:\Code\Sims\Sims\core\path.h
	file path:	D:\Code\Sims\Sims\core
	file base:	path
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Path
*********************************************************************/
#ifndef __PATH_H__
#define __PATH_H__

#include "sims.h"

namespace sims
{
	class Path
	{
	public:
		// get path which correctly joins a parent with a child path
		// e.g.
		// Join("/doc",  "a.txt")   -> "/doc/a.txt"
		// Join("/doc/", "a.txt")   -> "/doc/a.txt"
		// Join("/doc",  "/a.txt")  -> "/doc/a.txt"
		// Join("/doc/", "/a.txt")  -> "/doc/a.txt"
		static string Join(const string& parent, const string& child);

		// get path without its final element
		// e.g.
		// "/doc/a.txt"    -> "/doc"
		// "/doc/"         -> "/"
		// "/doc"          -> "/"
		static string Parent(const string& path);

		// get final component of a path, without any leading slash
		// e.g.
		// "/doc/a.txt"    -> "a.txt"
		// "/doc"          -> "doc"   <- doc may be a file without OS support
		static string Filename(const string& path);

		// get file extension, if any. including the prefix '.'
		// e.g.
		// "/doc/a.txt"    -> ".txt"
		// "/doc/a"        -> ""
		// "/doc/a."       -> "."
		static string Extension(const string& path);

		// resolve ".." and "." path elements, and convert "\\" to "/"
		// e.g.
		// "/doc/../a.txt"      -> "/a.txt"
		// "/doc/../ad/a.txt"   -> "/ad/a.txt"
		// "/doc/ad/././a.txt"  -> "/doc/ad/a.txt"
		static string Normalize(const string& path);

		// test whether a path is absolute
		// on Unix, that means it begins with a slash, 
		// on Windows that it begins with a (back)slash after chopping off a potential drive letter.
		// e.g.
		// "C:\\a.txt"      -> true on Windows
		// "/usr"           -> true on Liunx/Unix
		static bool IsAbsolute(const string& path);

		// test whether a path is relative
		static bool IsRelative(const string& path);

		// test whether a file or directory exists
		static bool Exists(const string& path);

		// whether a file exists
		static bool FileExists(const string& path);

		// whether a Directory exists
		static bool DirectoryExists(const string& path);
	};
}


#endif