#include "DataStream.h"
#include <iostream>
using namespace zpc::serialize;

using namespace std;

int main()
{
	/*
	DataStream ds;
	ds << "djflasjfl";
	string str;
	ds >> str;
	std::cout << str << std::endl;
	*/
	DataStream ds;
	std::map<string, int>m;
	m["a"] = 2;
	m["c"] = 4;
	ds.write(m);
	std::map<string, int>mk;
	ds.read(mk);
	for (auto it = mk.begin();it!=mk.end();it++)
	{
		std::cout << it->first << "= " << it->second << std::endl;
	}



	return 0;

}