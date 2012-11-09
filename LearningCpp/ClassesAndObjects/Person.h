#pragma once
#include <string>


class Person
{
private:
	std::string firstname;
	std::string lastname;
	int age;
public:
	Person(std::string first, std::string last, int age);
	~Person(void);
};

