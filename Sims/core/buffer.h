/********************************************************************
	created:	2016/11/29
	created:	29:11:2016   10:21
	filename: 	D:\Code\Sims\Sims\core\buffer.h
	file path:	D:\Code\Sims\Sims\core
	file base:	buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Buffer
*********************************************************************/
#pragma once

#include "core.h"

namespace sims
{	
	// T must be POD type 
	template<typename T>
	class TBuffer
	{
	public:
		explicit TBuffer(uint32 size = 0)
			: size_(size)
			, capacity_(size)
			, data_(size > 0 ? new T[size] : nullptr)
		{
		}
		TBuffer(uint32 size, uint32 capacity)
			: size_(size)
			, capacity_(capacity)
		{
			ASSERT(size <= capacity);
			data_ = (capacity != 0 ? new T[capacity] : nullptr);
		}
		TBuffer(const T* data, uint32 size)
			: size_(size)
			, capacity_(size)
		{
			ASSERT(data != nullptr && size > 0);
			if (size > 0)
			{
				data_ = new T[size];
				memcpy(data_, data, size * sizeof(T));
			}
			else data_ = nullptr;
		}
		~TBuffer()
		{
			Clear();
		}

		TBuffer(const TBuffer& buffer)
			: size_(buffer.size_)
			, capacity_(buffer.capacity_)
		{
			if (capacity_ > 0)
			{
				data_ = new T[capacity_];
				memcpy(data_, buffer.data_, size_ * sizeof(T));
			}
			else data_ = nullptr;
		}

		TBuffer(TBuffer&& buffer)
			: size_(buffer.size_)
			, capacity_(buffer.capacity_)
			, data_(buffer.data_)
		{
			buffer.size_ = buffer.capacity_= 0;
			buffer.data_ = nullptr;
		}

		TBuffer& operator=(const TBuffer& buffer)
		{
			if (&buffer != this)
			{
				if (buffer.size_ > capacity_)
				{
					SAFE_DELETEARRAY(data_);
					capacity_ = buffer.capacity_;
					data_ = new T[capacity_];
				}

				if (buffer.size_ > 0)
					memcpy(data_, buffer.data_, size_ * sizeof(T));

				size_ = buffer.size_;
			}
			return *this;
		}

		TBuffer& operator=(TBuffer&& buffer)
		{
			if (&buffer != this)
			{
				Clear();
				data_ = buffer.data_;
				size_ = buffer.size_;
				capacity_ = buffer.capacity_;
				buffer.data_ = nullptr;
			}
			return *this;
		}

		uint32 GetSize() const { return size_; }

		const T* GetData() const { return data_; }
		T* GetData() { return data_; }

		T& operator[](uint32 index)
		{
			return data_[index];
		}
		const T& operator[](uint32 index) const
		{
			return data_[index];
		}

		void Clear()
		{
			SAFE_DELETEARRAY(data_);
			size_ = capacity_ = 0;
		}

		void Resize(uint32 size)
		{
			if (size > capacity_)
			{
				T* data = data_;
				capacity_ = size;
				data_ = new T[capacity_];
				if (size_ > 0)
					memcpy(data_, data, size_ * sizeof(T));
				SAFE_DELETEARRAY(data);
			}
			size_ = size;
		}

		template<typename Pred>
		int32 FindIf(Pred P)
		{
			ASSERT(data_ && size_ > 0);

			for (uint32 index = 0; index < size_; ++index)
			{
				if (P(data_[index]))
				{
					return (int32)index;
				}
			}
			return INDEX_NONE;
		}

		template<typename Pred>
		void RemoveIf(Pred P)
		{
			ASSERT(data_ && size_ > 0);

			int32 first = FindIf(P);
			if (first != INDEX_NONE)
			{
				for (int32 index = first; index < (int32)size_; ++index)
				{
					if (!P(data_[index]))
					{
						data_[first] = data_[index];
						++first;
					}
				}
				size_ = first;
			}
		}
	private:
		uint32 size_;
		uint32 capacity_;
		T* data_;
	};

	typedef TBuffer<char> Buffer;
}
