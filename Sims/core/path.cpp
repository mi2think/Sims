/********************************************************************
	created:	2016/09/17
	created:	17:9:2016   22:45
	filename: 	D:\Code\Sims\Sims\core\path.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	path
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Path
*********************************************************************/
#include "path.h"
#include "file_system.h"
#include "platform.h"
#include "utils/string_utils.h"

namespace sims
{
	string Path::Join(const string& parent, const string& child)
	{
		string path;
	
		bool pSlash = (!parent.empty()) && parent.back() == '/';
		bool cSlash = (!child.empty()) && child.front() == '/';

		path.reserve(parent.size() + child.size() + 1);
		path += parent;
		if (!pSlash && !cSlash)
			path += "/";
		else if (pSlash && cSlash) // remove '/'
			path.resize(path.size() - 1);

		path += child;
		return path;
	}

	string Path::Parent(const string& path)
	{
		string::size_type pos;

		if (path.empty())
			return "";

		if (path.back() == '/')
		{
			// path ends slash, ignore it
			if (path.size() > 1)
				pos = path.rfind('/', path.size() - 2);
			// path just is "/"
			else
				return path;
		}
		else
			pos = path.rfind('/');

		if (pos == string::npos)
			return "";
		else if (pos == 0)
			return "/";
		else
			return path.substr(0, pos);
	}

	string Path::Filename(const string& path)
	{
		string::size_type pos = path.rfind('/');
		if (pos == string::npos)
			return "";
		else
			return path.substr(++pos, path.size());
	}

	string Path::Extension(const string& path)
	{
		string::size_type pos = path.rfind('.');
		if (pos == string::npos)
			return "";
		else
			return path.substr(pos, path.size());
	}

	string Path::Normalize(const string& path)
	{
		string npath = path;
		str_replace(npath, path, "\\", "/");

		struct PathIter
		{
			string path_;
			string::size_type pos_;

			PathIter(const string& path)
				: path_(path)
				, pos_(0)
			{
				if (!path.empty())
				{
#ifdef PLATFORM_WINDOWS
					if (path.size() >= 3 && path[1] == ':')
						pos_ += 3; // skip e.g. "C:\"
#else
					if (path[0] == '/')
						++pos_;
#endif
				}
			}
			bool HasNext() const { return pos_ < path_.size(); }
			string Next()
			{
				string::size_type pos = path_.find('/', pos_);
				if (pos == string::npos)
					pos = path_.size();
				string result = path_.substr(pos_, pos - pos_);
				pos_ = pos + 1;
				return result;
			}
		};

		PathIter it(npath);
		string result;
		if (IsAbsolute(npath))
		{
#ifdef PLATFORM_WINDOWS
			result = npath.substr(0, 3);
#else
			result = "/";
#endif
		}
		while (it.HasNext())
		{
			string e = it.Next();
			if (e == "..")
			{
				if (!result.empty())
					result = Parent(result);
				else
					result = e; // ".." is head, skip
			}
			else if (e == ".")
				; // do nothing
			else
				result = Join(result, e);
		}
		return result;
	}

	bool Path::IsAbsolute(const string& path)
	{
		if (path.empty())
			return false;
#ifdef PLATFORM_WINDOWS
		return path.size() > 1 && path[1] == ':';
#else
		return path.front() == '/';
#endif
	}

	bool Path::IsRelative(const string& path)
	{
		return !IsAbsolute(path);
	}

	bool Path::Exists(const string& path)
	{
		return Platform::GetFileSystem()->Exists(path);
	}

	bool Path::FileExists(const string& path)
	{
		return Platform::GetFileSystem()->FileExists(path);
	}

	bool Path::DirectoryExists(const string& path)
	{
		return Platform::GetFileSystem()->DirectoryExists(path);
	}
}