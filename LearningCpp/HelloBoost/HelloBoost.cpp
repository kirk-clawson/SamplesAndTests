// HelloBoost.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HelloBoost.h"

using std::cout;
using std::endl;

int _tmain(int argc, _TCHAR* argv[])
{
	float x = 3.3;
	float y = 15.6;

	cout << "X: " << x << endl;
	cout << "Y: " << y << endl;
	cout << "GCD: " << gcd(x, y) << endl;

	getchar();
	return 0;
}

