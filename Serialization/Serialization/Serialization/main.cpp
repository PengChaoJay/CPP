#include "DataStream.h"
#include <iostream>
using namespace zpc::serialize;

using namespace std;

int main()
{
	DataStream ds;
	ds << "djflasjfl";
	string str;
	int mk;
	ds >> str;
	std::cout << str << std::endl;
	return 0;

}