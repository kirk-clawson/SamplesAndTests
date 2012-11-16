#include "stdafx.h"
#include "Person.h"
#include <string>
using namespace std;

Person::Person(string first, string last, int age) : firstName(first), lastName(last), age(age)
{
}

Person::~Person(void)
{
}

string Person::GetName()
{
	return firstName + " " + lastName;
}

int Person::GetAge()
{
	return age;
}

void Person::SetAge(int age)
{
	Person::age = age;
}