# CPPReflection
# 介绍
## 概念
反射机制允许程序在运行时借助Reflection API 取得任何类的内部信息，并能直接操作对象的内部属性和方法。（比如UE中的反射的实现）<br>


## 问题
1. C++不支持反射
2. 很多业务场景需要依赖反射机制，比如：RPC，WEB MVC，对象序列化等。
## 场景：
RPC，远程过程调用，比如客户端向服务端发送请求，服务端处理之后给客户端发送处理结果，客户端如何获得对象，获得对象里面的方法，变量，需要使用反射。
## 目标
1. 类对象的反射
2. 类成员数据的反射
3. 类成员函数的反射

## 演示
``` C++
class A: public objdect
{
public:
    A(){}
    ~A(){}
    void f1()
    {
        std::cout<<"f1"<<std::endl;
    }
    void f2()
    {
        std::cout<<"f2"<<std::endl;
    }
public:
    string m_name;
    int m_age;
};


REGISTER_CLASS(A);
REGISTER_CLASS_FIELD(A,m_name,string);
REGISTER_CLASS_FIELD(A,m_age,int);
REGISTER_CLASS_METHOD(A,f1）;
REGISTER_CLASS_METHOD(A,f2）;

```

# 三种类型的宏
1. 类对象的反射:
REGISTER_CLASS(A);
2. 类成员变量的反射
REGISTER_CLASS_FIELD(A,m_name,string)
3. 类成员函数的反射
REGISTER_CLASS_METHOD(A,f1）

## 反射用法：
``` C++
int main()
{
    ClasFactory * factory = Singleton<ClassFactory> :: instance();
    Object * a = factory ->create_class("A");
    
    // 类成员数据的反射
    string name;
    a->get_field_value("m_name",name);
    a->set_field_value("m_name","kitty");
    
    int age;
    a->get_field_value("m_age",age);
    a->set_field_value("m_age",30);
    // 类成员函数的反射
    a->call("f1");
    a->call("f2");
}
```

# 基本框架
## 单例模式
```C++
#pragma once
namespace zpc
{
        namespace utility
        {
                template <typename T>
                class Singleton
                {
                public:
                        static T* instance()
                        {
                                if (m_instance == nullptr)
                                {
                                        m_instance = new T();
                                }
                                return m_instance;

                        }
                private:
                        static T* m_instance;
                private:
                        Singleton() {}
                        ~Singleton() {}
                        Singleton(const Singleton<T>&) = delete;
                        Singleton <T>& operator = (const Singleton<T>&) = delete;
                };
                template<typename T>
                T* Singleton<T>::m_instance = nullptr;
        }
}
```
创建的单例模式，保证创建的工厂只有一个，实现全局唯一的工厂类。
## 工厂模式
```C++
#pragma once
#include <string>
#include<map>
#include "Singleton.h"
using namespace std;
using namespace zpc::utility;

class Object
{
public:
        Object() {};
        virtual ~Object() {};
        virtual void show() = 0;
};
namespace  zpc {
        namespace reflect
        {
                typedef Object* (*create_object)(void);
                class ClassFactory
                {
                        friend class Singleton<ClassFactory>;
                public:
                        void register_class(const string& className, create_object method);
                        Object* create_class(const string& className);

                private:
                        ClassFactory() {}
                        ~ClassFactory() {}
                private:
                        std::map<string, create_object> m_classMap;
                };

        }
}

```
### 辅助类
``` C++
#pragma once
#include "ClassFactory.h"
namespace zpc {
        namespace reflect
        {
                class ClassRegister
                {
                public:
                        ClassRegister(const string& className, create_object method)
                        {
                                Singleton<ClassFactory>::instance()->register_class(className, method);
                        }
                };
#define REGISTER_CLASS(className)                        \
        Object* createObject##className()                        \
                {                                                                        \
                        Object* obj = new className();        \
                        return obj;                                                \
                }                                                                        \
        ClassRegister ClassRegister##className(#className,createObject##className)
        }
```


### 类成员的反射
**关键问题**：如何获取类成员数据的内存地址偏移（offset)

1. 黑科技1：
``` C++
#define OFFSET(className,fieldName)\
    ((size_t)(&((className*)0)->fieldName))
        auto offset = (size_t) & ((Test *)0)->m_age;

        Test t("kitty", 18);
        int age = *(int*)((size_t)&t + offset);
        std::cout << age << std::endl;
```
标准库中存在着

**#include <stddef.h>** 
offsetof(s,m) 类似的写法
有些编译器编译不通过

2. 黑科技2：
``` C++
Test t;
size_t offset = (size_t)(&t.member) - (size_t)(&t);
```

### 类成员函数的反射
类成员函数指针：
``` C++ 
typedef std::function<void(Test*)>test_method;
method method = &Test::f1;

Test *t = new Test();
method(t);

uintptr_t ptr = (uintptr_t)& method;
(*(test_method*)(ptr))(t)
function<void(delctype(this))
```
