#pragma once

#include <crtdbg.h>
#include <Windows.h>
#include <iterator>
#include <stdexcept>

#define ASSERT _ASSERTE

#ifdef _DEBUG
#define VERIFY(expression) ASSERT(expression)
inline void HR(HRESULT hr) { ASSERT(S_OK == hr); }

#else
#define VERIFY(expression) (expression)
#define HR(expression) (expression)

#endif // _DEBUG

// shims for winstore types
typedef unsigned long long uint64;
typedef unsigned int uint32;

namespace Utility
{
	template <typename T>
	struct Dimensions
	{
		Dimensions() {}
		Dimensions(T width, T height) : Width(width), Height(height) {}
		T Width = T();
		T Height = T();
	};
	template<typename T>
	inline bool operator==(const Dimensions<T>& lhs, const Dimensions<T>& rhs)
	{
		return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
	}
	template<typename T>
	inline bool operator!=(const Dimensions<T>& lhs, const Dimensions<T>& rhs)
	{
		return !(lhs == rhs);
	}

	/// defines an exception based on Win32 error codes. The what() function will
	/// return a formatted string returned from FormatMessage()
	class Win32Exception : public std::runtime_error
	{
	public:
		Win32Exception() : std::runtime_error(ErrorMessage(&error_code_))
		{
		};

		virtual ~Win32Exception() {};

		/// return the actual error code
		DWORD ErrorCode() const throw() { return error_code_; };

	private:

		static std::string ErrorMessage(DWORD* error_code)
		{
			*error_code = ::GetLastError();

			std::string error_messageA;
			wchar_t* error_messageW = NULL;
			DWORD len = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				*error_code,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPWSTR>(&error_messageW),
				0,
				NULL);
			if (NULL != error_messageW)
			{
				// this may generate a C4244 warning. It is safe to ignore.
				std::copy(error_messageW,
					error_messageW + len,
					std::back_inserter(error_messageA));
				::LocalFree(error_messageW);
			}
			return error_messageA;
		};

		/// error code returned by GetLastError()
		DWORD error_code_;

	}; // class Win32Exception

} // namespace Utility