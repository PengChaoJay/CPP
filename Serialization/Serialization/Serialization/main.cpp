#include "DataStream.h"
#include "Serializable.h"
#include <iostream>
using namespace zpc::serialize;

class A :public Serializable
{
public:
	A() {}
	A(const string& name, int age,const string & gender) :m_name(name), m_age(age),m_gender(gender)  {}
	~A() {}

	void show()
	{
		std::cout << m_name << m_age <<m_gender;
	}
	SERIALIZE(m_name,m_age,m_gender)
private:
	string m_name;
	int m_age;
	string m_gender;
};






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
	/*
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
	*/
	//DataStream ds;
	//A a("kee", 32,"famel");
	//ds << a;
	//
	//A b;
	//ds >> b;
	//b.show();

	A a("kitty", 18,"ddf");

	DataStream ds;
	ds << a;
	ds.save("a.out");
	ds.load("a.out");
	A b;
	ds >> b;
	b.show();




	return 0;

}