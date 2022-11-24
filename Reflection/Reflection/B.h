#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "ClassRegister.h"
using namespace zpc::reflect;


class B :public Object
{
public:
	B() {}
	~B() {}

	void show()
	{
		std::cout << "B" << std::endl;
	}

public:
	string m_name;
};

REGISTER_CLASS(B);