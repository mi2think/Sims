/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:44
	filename: 	D:\Code\Sims\Sims\sims.h
	file path:	D:\Code\Sims\Sims
	file base:	sims
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	
*********************************************************************/
#ifndef __SIMS_H__
#define __SIMS_H__

#include "math/math_fwd.h"
#include "core/core.h"
#include "core/color.h"
#include "core/log.h"
#include "core/ref.h"
#include "core/buffer.h"
#include "core/platform_def.h"
#include "utils/string_utils.h"

#include <sstream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cstdlib>

namespace sims
{
#define DECLARE_ENUM(name) \
		namespace name { \
			enum Type {

#define END_DECLARE_ENUM() \
				,Max \
			}; \
		}
	
	DECLARE_ENUM(LockFlags)
		LockRead    = BIT(0),
		LockWrite   = BIT(1),
		LockRW      = LockRead | LockWrite
	END_DECLARE_ENUM()

	enum SeekType
	{
		ST_Start = SEEK_SET,
		ST_Current = SEEK_CUR,
		ST_End = SEEK_END
	};

	using std::vector;
	using std::map;
	using std::pair;
	using std::ostringstream;

	class Font;
	class Glyph;
	class Event;
	class Image;
	class IInputStream;
	class IOutputStream;
	class IFileSystem;

	typedef Ref<Font> FontRef;
	typedef Ref<Glyph> GlyphRef;
	typedef Ref<Event> EventRef;
	typedef Ref<Image> ImageRef;
	typedef Ref<IInputStream> IInputStreamRef;
	typedef Ref<IOutputStream> IOutputStreamRef;
	typedef Ref<IFileSystem> IFileSystemRef;

	class NonCopyable
	{
	public:
		NonCopyable() {}
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	class ScopeGuard : public NonCopyable
	{
	public:
		ScopeGuard(std::function<void()> func) : func_(func) {}
		~ScopeGuard() { func_(); }
	private:
		std::function<void()> func_;
	};
#define SCOPEGUARD_NAMELINE_CAT(name, line) name##line
#define SCOPEGUARD_NAMELINE(name, line) SCOPEGUARD_NAMELINE_CAT(name, line)
#define ON_SCOPE_EXIT(func) ScopeGuard SCOPEGUARD_NAMELINE(EXIT, __LINE__)(func)

	// time step
	class Timestep
	{
	public:
		explicit Timestep(float seconds) : seconds_(seconds) {}

		int GetMilliseconds() const { return int(seconds_ * 1000); }
		float GetSeconds() const { return seconds_; }
	private:
		float seconds_;
	};
}

#endif