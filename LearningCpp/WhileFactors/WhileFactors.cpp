// WhileFactors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int input;
	cout << "Enter a number" << endl;
	cin >> input;

	bool isPrime = true;
	int candidate = 2;
	while (candidate <= input/candidate)
	{
		int factor = input/candidate;
		if (factor * candidate == input)
		{
			cout << "factor found: " << factor << endl;
			isPrime = false;
		}
		candidate = candidate + 1;
	}

	cout << input << " is ";
	if (isPrime)
	{
		cout << "prime" << endl;
	}
	else
	{
		cout << "not prime" << endl;
	}

	return 0;
}

