#include "DataStream.h"
#include "Serializable.h"
#include <iostream>
using namespace zpc::serialize;

class A :public Serializable
{
public:
	A() {}
	A(const string& name, int age) :m_name(name), m_age(age) {}
	~A() {}
	void serializ(DataStream& stream) const
	{
		char type = DataStream::DataType::CUSTOM;
		stream.write((char*)&type, sizeof(char));
		stream.write(m_name);
		stream.write(m_age);

		string mk;
		stream << mk;
		std::cout << mk;
		stream << m_age;
	}
	bool unserializ(DataStream & stream)
	{
		char type;
		stream.read((char*)&type, sizeof(char));
		if ((DataStream::DataType::CUSTOM)&type!= DataStream::DataType::CUSTOM)
		{
			return false;
		}
		stream.read(m_name);
		stream.read(m_age);
		return true;
	}
	void show()
	{
		std::cout << m_name << m_age;
	}
private:
	string m_name;
	int m_age;
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
	DataStream ds;
	A a("kee", 32);
	ds << a;
	
	A b;
	ds >> b;
	b.show();
	return 0;

}