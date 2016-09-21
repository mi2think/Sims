/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:20
	filename: 	D:\Code\Sims\Sims\core\log.h
	file path:	D:\Code\Sims\Sims\core
	file base:	log
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	LOG
*********************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <sstream>
#include <iostream>
#include <type_traits>

namespace sims
{
	class Log
	{
	public:
		struct ScopeNewline
		{
			ScopeNewline(bool newline);
			~ScopeNewline();
			bool newline_;
		};

		enum LogLevel
		{
			Info,
			Warn,
			Error,
			Fatal,
		};

		static Log& GetInstance() { static Log s_ins; return s_ins; }
		Log();

		void WriteFormat(int level, const char* fmt, ...);
		void WriteString(int level, const char* s);
		unsigned short GetTextColor(int level);
		void SetTextColor(unsigned short color);
		void SetNewline(bool newline) { newline_ = newline; }
		bool GetNewline() const { return newline_; }
	private:
		void WriteBegin(int level);
		void WriteEnd();

		std::ostream* os_;
		void* hwnd_;
		bool newline_;
	};

#define GLOG Log::GetInstance()
#define LOG_TO(level, fmt, ...) GLOG.WriteFormat(level, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)	LOG_TO(Log::Info, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)	LOG_TO(Log::Warn, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...)	LOG_TO(Log::Error, fmt, __VA_ARGS__)
#define LOG_FATAL(fmt, ...)	LOG_TO(Log::Fatal, fmt, __VA_ARGS__)

#define LOGSCOPE_NAMELINE_CAT(name, line) name##line
#define LOGSCOPE_NAMELINE(name, line) LOGSCOPE_NAMELINE_CAT(name, line)
#define LOGSCOPE_NEWLINE(newline) Log::ScopeNewline LOGSCOPE_NAMELINE(LogScope, __LINE__)(newline)

	// check if T has member function like:
	// string T::ToString() const;
	template<typename T>
	struct has_ToString {
		template<typename U, std::string (U::*)() const> struct HELPS;
		template<typename U> static char Test(HELPS<U, &U::ToString>*);
		template<typename U> static int Test(...);
		const static bool value = sizeof(Test<T>(0)) == sizeof(char);
	};

	// log stream
	class LogStream
	{
	public:
		static LogStream& GetInstance() { static LogStream s_ins; return s_ins; }
		LogStream() : level_(Log::Info) {}

		void SetLevel(int level) { level_ = level; }

		template<typename T>
		LogStream& operator<<(const T& t)
		{
			LOGSCOPE_NEWLINE(false);
			GLOG.WriteFormat(level_, "%s", ToStr(t).c_str());
			return *this;
		}

		// no need format
		LogStream& operator<<(char* s);
		LogStream& operator<<(const char* s);
		LogStream& operator<<(std::string& s);
		LogStream& operator<<(const std::string& s);

		template<typename T, typename std::enable_if<has_ToString<T>::value>::type* = nullptr>
		std::string ToStr(const T& t)
		{
			std::ostringstream oss;
			oss << t.ToString();
			return oss.str();
		}

		template<typename T, typename std::enable_if<!has_ToString<T>::value>::type* = nullptr>
		std::string ToStr(const T& t)
		{
			std::ostringstream oss;
			oss << t;
			return oss.str();
		}
	private:
		int level_;
	};

#define GLOGS LogStream::GetInstance()
}

#endif
