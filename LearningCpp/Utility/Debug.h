#pragma once

#include <crtdbg.h>
#include <Windows.h>

#define ASSERT _ASSERTE

#ifdef _DEBUG
#define VERIFY(expression) ASSERT(expression)
inline void HR(HRESULT hr) { ASSERT(S_OK == hr); }

#else
#define VERIFY(expression) (expression)
#define HR(expression) (expression)

#endif // _DEBUG
