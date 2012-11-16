// Vectors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Person.h"
#include <vector>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Person p1("Kirk", "Clawson", 43);
	Person p2("Homer", "Simpson", 37);
	Person p3("Peter", "Griffin", 42);

	vector<Person> people;
	people.push_back(p1);
	people.push_back(p2);
	people.push_back(p3);

	for (int i = 0; i < people.size(); i++)
	{
		cout << "Person: " + people[i].GetName() << endl;
	}

	return 0;
}

