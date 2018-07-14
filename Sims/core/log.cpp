/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:22
	filename: 	D:\Code\Sims\Sims\core\log.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	log
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	LOG
*********************************************************************/
#include "log.h"
#include "platform.h"

#include <windows.h>
#include <cstdarg>
#include <string>

namespace sims
{
	Log::Log()
		: hwnd_(nullptr)
		, newline_(true)
		, os_(nullptr)
	{}

	void Log::WriteBegin(int level)
	{
		SetTextColor(GetTextColor(level));

		if (level == Error || level == Fatal)
			os_ = &std::cout;
		else
			os_ = &std::cerr;
	}

	void Log::WriteEnd()
	{
		if (newline_)
		{
			(*os_) << std::endl;

			Platform::LowLevelOutputDebugString("\n");
		}
	}

	void Log::WriteFormat(int level, const char* fmt, ...)
	{
		WriteBegin(level);
	
		static std::string buffer(1024, '\0');

		va_list ap;
		va_start(ap, fmt);

		// return require length
		int len = vsnprintf(nullptr, 0, fmt, ap);
		if (len > (int)buffer.size())
			buffer.resize(len);
		len = vsnprintf(&buffer[0], buffer.size(), fmt, ap);
		buffer[len] = 0;

		va_end(ap);

		(*os_) << buffer.c_str();

		Platform::LowLevelOutputDebugString(buffer.c_str());

		WriteEnd();
	}

	void Log::WriteString(int level, const char* s)
	{
		WriteBegin(level);
		(*os_) << s;
		WriteEnd();
	}

	LogStream& LogStream::operator<<(char* s)
	{
		return operator<<((const char*)s);
	}

	LogStream& LogStream::operator<<(const char* s)
	{
		LOGSCOPE_NEWLINE(false);
		GLOG.WriteString(level_, s);
		return *this;
	}

	LogStream& LogStream::operator<<(std::string& s)
	{
		return operator<<((const char*)s.c_str());
	}

	LogStream& LogStream::operator<<(const std::string& s)
	{
		return operator<<(s.c_str());
	}

	unsigned short Log::GetTextColor(int level)
	{
		unsigned short color;
		switch (level)
		{
		case Info:
			// info is bright white
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		case Warn:
			// warn is bright yellow
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case Error:
			// error is red
			color = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case Fatal:
			// fatal is white on red
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;
			break;
		default:
			// other is cyan
			color = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		}
		return color;
	}

	void Log::SetTextColor(unsigned short color)
	{
		if (hwnd_ == NULL)
		{
			hwnd_ = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		SetConsoleTextAttribute(hwnd_, color);
	}

	Log::ScopeNewline::ScopeNewline(bool newline)
	{
		newline_ = GLOG.GetNewline();
		GLOG.SetNewline(newline);
	}

	Log::ScopeNewline::~ScopeNewline()
	{
		GLOG.SetNewline(newline_);
	}
}