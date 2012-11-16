#pragma once
#include <string>

class Person
{
private:
	std::string firstName;
	std::string lastName;
	int age;
public:
	Person(std::string first, std::string last, int age);
	~Person(void);
	std::string GetName();
	int GetAge();
	void SetAge(int age);
};

