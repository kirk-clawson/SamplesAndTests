#pragma once

#include <algorithm>

template <typename Traits>
class SafeHandle
{
	typedef Pointer Traits::Pointer;

	Pointer m_value;

	void Close() throw()
	{
		if (*this)
		{
			Traits::Close(m_value);
		}
	}
public:

	SafeHandle(SafeHandle const &) = delete;
	SafeHandle& operator=(SafeHandle const &) = delete;

	explicit SafeHandle(Pointer value = Traits::Invalid()) throw() :
		m_value(value)
	{
	}

	SafeHandle(SafeHandle && other) throw() :
		m_value(other.Detach())
	{
	}

	SafeHandle& operator=(SafeHandle && other) throw()
	{
		if (this != other)
		{
			Attach(other.Detach());
		}
		return *this;
	}

	~SafeHandle() throw()
	{
		Close();
	}

	explicit operator bool() const throw()
	{
		return m_value != Traits::Invalid();
	}

	Pointer Get() const throw()
	{
		return m_value;
	}

	Pointer * GetAddressOf() throw()
	{
		ASSERT(!*this);
		return &m_value;
	}

	Pointer Detach() throw()
	{
		Pointer temp = m_value;
		m_value = Traits::Invalid();
		return temp;
	}

	bool Attach(Pointer value = Traits::Invalid()) throw()
	{
		if (m_value != value)
		{
			Close();
			m_value = value;
		}

		return static_cast<bool>(*this);
	}

	void Swap(SafeHandle & other) throw()
	{
		std::swap(m_value, other.m_value);
	}
};

template <typename Traits>
void swap(SafeHandle<Traits> & left, SafeHandle<Traits> & right) throw()
{
	left.Swap(right);
}