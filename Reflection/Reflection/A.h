#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "ClassRegister.h"
using namespace zpc::reflect;


class A:public Object
{
public:
	A(){}
	~A() {}

	void show()
	{
		std::cout << "A"  << std::endl;
	}
	void f1()
	{
		std::cout << "f1" << std::endl;
	}
	void f2()
	{
		std::cout << "f2" << std::endl;
	}

public:
	string m_name;
	int m_age;
};

REGISTER_CLASS(A);
REGISTER_CLASS_FIELD(A, m_name, string);
REGISTER_CLASS_FIELD(A, m_age, int);
REGISTER_CLASS_METHOD(A,f1);
REGISTER_CLASS_METHOD(A,f2);
