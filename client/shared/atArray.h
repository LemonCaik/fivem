#pragma once

#ifndef max
#define max std::max
#define max_defined 1
#endif

template<typename TValue>
class atArray
{
private:
	TValue* m_offset;
	uint16_t m_count;
	uint16_t m_size;

public:
	atArray()
	{
		m_offset = (TValue*)0;
		m_count = 0;
		m_size = 0;
	}

	atArray(int capacity)
	{
		m_offset = new TValue[capacity];
		m_count = 0;
		m_size = capacity;
	}

	inline uint16_t GetCount()
	{
		return m_count;
	}

	inline uint16_t GetSize()
	{
		return m_size;
	}

	TValue& Get(uint16_t offset)
	{
		if (offset >= m_count)
		{
			FatalError("atArray index out of bounds");
		}

		return m_offset[offset];
	}

	void Expand(uint16_t newSize)
	{
		if (m_size >= newSize)
		{
			return;
		}

		TValue* newOffset = (TValue*)rage::GetAllocator()->allocate(newSize * sizeof(TValue), 16, 0);

		// initialize the new entries
		std::uninitialized_fill(newOffset, newOffset + newSize, TValue());

		// copy the existing entries
		if (m_offset)
		{
			std::copy(m_offset, m_offset + m_count, newOffset);

			rage::GetAllocator()->free(m_offset);
		}

		m_offset = newOffset;
		m_size = newSize;
	}

	void Set(uint16_t offset, const TValue& value)
	{
		if (offset >= m_size)
		{
			Expand(max((int)m_size, 2) * 2);
		}

		if (offset >= m_count)
		{
			m_count = offset + 1;
		}

		m_offset[offset] = value;
	}

	void Remove(int index)
	{
		for (int i = index; i < (m_count - 1); i++)
		{
			m_offset[i] = m_offset[i + 1];
		}

		m_count--;
	}
};

#ifdef max_defined
#undef max_defined
#undef max
#endif