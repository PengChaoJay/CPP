
#include  <iostream>
using namespace std;
#include "A.h"
#include "B.h"

//class Test
//{
//public:
//	Test(const string& name, int age) :m_name(name), m_age(age) {}
//	~Test() {}
//public:
//	string m_name;
//	int m_age;
//};

// 宏的写法
//#define OFFSET(className,fieldName) \
//    ((size_t)(&((className*)0)->fieldName))

/*

	ClassFactory* factory = Singleton<ClassFactory>::instance();
	Object* a =factory->create_class("A");
	a->show();

	Object* b = factory->create_class("B");
	b->show();
	// auto offset = (size_t) & ((Test *)0)->m_age;
	// auto offset = OFFSET(Test, m_age);
Test t("kitty", 18);
// 获得是一个偏移量
auto offset = (size_t)(&t.m_name) - (size_t)(&t);
std::cout << (size_t)(&t) << std::endl;
// 原始地址加上一偏移量获得真正的地址空间
string age = *(string*)((size_t)&t + offset);
std::cout << age << std::endl;


*/

int main()
{
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	Object* a = factory->create_class("A");
	a->show();

	a->set("m_age", 18);
	int age;
	a -> get("m_age", age);
	std::cout << age << std::endl;
	const string name = "lili";
	a->set("m_name", name);
	string str;
	a->get("m_name", str);
	std::cout << str << std::endl;


	a->call("f1");
	a->call("f2");
	


	return 0;
}