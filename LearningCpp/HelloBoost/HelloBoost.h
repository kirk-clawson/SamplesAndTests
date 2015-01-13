#include <iostream>
#include <algorithm>

template <typename T>
T gcd(T u, T v)
{
	while (u > 0)
	{
		if (u < v)
		{
			std::swap(u, v);
		}
		u = u - v;
	}
	return v;
}