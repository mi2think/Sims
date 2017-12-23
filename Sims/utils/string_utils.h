/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:51
	filename: 	D:\Code\Sims\Sims\utils\string_utils.h
	file path:	D:\Code\Sims\Sims\utils
	file base:	string_utils
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	String Utils
*********************************************************************/
#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <cstring>
#include <string>
#include <vector>

namespace sims
{
	using std::string;
	using std::wstring;
	using std::vector;

	// atoi with check
	int atoi_s(const char* s);
	// string copy
	void strncpy_s(char* pDest, const char* pSrc, int destSize);
	// string format
	// use internal static space hold format result, length limits to 1024
	const char* str_format(const char* format, ...);
	// string trim
	enum { TRIM_LEFT = 1, TRIM_RIGHT };
	void str_trim(string& str, int flag = TRIM_LEFT | TRIM_RIGHT);
	// string strip
	void str_strip(char* strDest, const char* strSrc, const char* strPattern);
	void str_strip(string& strDest, const string& strSrc, const string& strPattern);
	// string strip any char
	// common used in parse file, e.g. "<1,2> <2,3>" -> "1,2 2,3", where any char is:"<>" 
	void str_strip_anychar(char* strDest, const char* strSrc, const char* anychar);
	void str_strip_anychar(string& strDest, const string& strSrc, const string& anychar);
	// string split
	void str_split(vector<string>& vec, const string& str, char ch);
	void str_split_int(vector<int>& ivec, const string& str, char ch);
	// string replace
	void str_replace(string& strDest, const string& strSrc, const string& strPattern, const string& strReplace);
	// string replace by regex pattern
	void str_regex_replace(string& strDest, const string& strSrc, const string& strRegexPattern, const string& strReplace);
	// string replace any char
	void str_replace_anychar(string& strDest, const string& strSrc, const string& anychar, const string& strReplace);
	// string is valid float format
	bool str_is_valid_float(const string& str);
	// string is starts with
	bool str_starts_with(const string& str, const string& pattern);
	// string is ends with
	bool str_ends_with(const string& str, const string& pattern);
}

#endif
