# C++ 序列化：从设计到实现
## 介绍
### 概念
序列化 **（serialization）** 是将对象的状态信息转化为可以存储或传输的形式的过程。在序列化期间，对象将其当前状态写入临时或持久性存储区。以后可以通过从存储区中读取或反序列化对象的状态，重新创建该对象。<br>
### 序列化的方式
1. 文本格式：JSON,XML
2. 二进制格式：protobuf
### 二进制序列化
序列化： 将数据结构或对象转换成二进制串的过程<br>
反序列化：经在序列化过程中所产生的二进制串转换成数据结构或对象的过程<br>
 - 序列化后，数据小，传输速度快
 - 序列化、反序列化速度快
### 演示
#### 1. 基本类型序列化、反序列化
基本类型序列化
``` C++
int main()
{
    DataStream ds;
    int n =123;
    double d = 23.2;
    string s = "hellow serialization";
    ds << n <<d <<s;
    ds.save("a.out");
}
```
基本类型的反序列化
``` C++
{
    DataStream ds;
    int n;
    double d;
    string s;
    ds.load("a.load");
    ds<<d<<s<<d;
    std::cout<<n<<d<<s<<std::endl;
}
```
#### 2.复合类型数据序列化、反序列化
复合类型数据序列化
``` C++ 
int main()
{
    std::vector<int>v{3,2,1};
    std::map<string,string>m;
    m["name"] = "kitty";
    m["phone"] = "12121";
    m["gender"] = "male";
    DataStream ds;
    ds<<v<<s;
    ds.save("a.out");
}
复合类型数据反序列化
``` C++
int main()
{
    DataStreawm ds;
    ds.load("a.out");
    std::vector<int>v;
    std::map<string,string>m;
    ds>>v>>m;
    for(auto it = v.begin();it != v.end();it++)
    {
        std::cout<<*it<<std::endl;
    }
    for(auto it = m.begin();it!= m.end;it++)
    {
        std::cout<<it->first<<"="<<it->second<<std::endl;
    }
}
```
#### 3.自定义类的序列化、反序列化
自定义类的序列化
```C++
class A:public Serialization
{
public:
    A();
    ~A();
    A(const string & name,int age):m_name(name),m_age(age){}
    void show()
    {
        std::cout<<m_name<<" "<<m_age<<std::endl;
    }
    //需要序列化的字段
    SERIALIZE(m_name,m_age);
private:
    string m_name;
    int m_age;
}
int main()
{
    A a("Hell",12);
    DataStream ds;
    ds<<a;
    ds.save("a.out");
}

```
反序列化类的类型
``` C++
int main()
{
    DataStreawm ds;
    ds.load("a.out");
    std::vector<int>v;
    std::map<string,string>m;
    ds>>v>>m;
    for(auto it = v.begin();it != v.end();it++)
    {
        std::cout<<*it<<std::endl;
    }
    for(auto it = m.begin();it!= m.end;it++)
    {
        std::cout<<it->first<<"="<<it->second<<std::endl;
    }
}
```
#### 3.自定义类的序列化、反序列化
自定义类的反序列化
```C++
class A:public Serialization
{
public:
    A();
    ~A();
    A(const string & name,int age):m_name(name),m_age(age){}
    void show()
    {
        std::cout<<m_name<<" "<<m_age<<std::endl;
    }
    //需要序列化的字段
    SERIALIZE(m_name,m_age);
private:
    string m_name;
    int m_age;
}
class B:public Serialization
{
public:
    B();
    ~B();
    void add(const A & a)
    {
        m_vector.add(a);
    }
    B(const string & name,int age):m_name(name),m_age(age){}
    void show()
    {
        for(auto it = m_vector.begin();it! = m_vector.end();it++)
        {
            it->show();
        }
    }
    //需要序列化的字段
    SERIALIZE(m_vector);
private:
    std::vector<A> m_vector;
}
int main()
{
    // 序列化
    // B b;
    // b.add(A("hello",12));
    // b.add(A("liuc",21));
    // b.add(A("wang",34));
    // DataSream ds;
    // ds<<b;
    // ds.save("a.out");
    //反序列化
    B b;
    DataSream ds;
    ds.load("a.out");
    ds>>b;
    b.show();
}
```
#### Protobuf 与 srialization的区别
|             | protobuf    |    serialize   |
| :----------: | :-----------: |  :--------------: |
| 二进制格式    | 是 | 是 |
| 数据体积   | 小 | 小 |
| 编码速度 | 快 | 快|
| 数据格式类型 | 丰富 | 更加丰富 |
| 消息定义文件 | 需要 | 不需要 |
| 需要编译 | 需要 | 不需要 |
| 代码实现 | 复杂 | 简单 |

## 数据类型的定义
``` C++
enum DataType
{
    BOOL =0,
    CHAR,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    STRING,
    VECTOR,
    LIST,
    MAP,
    SET,
    CUSTOM
}
```

### 基本类型序列化+反序列化
#### 基本数据类型编码
| 字段类型 | 字段长度(字节) | 底层编码格式 |
| :---:|    :----: | :----: | 
| bool | 2 | Type(1) + Value(1)|
| char | 2 | Type(1) + Value(1)|
| int32 | 5 | Type(1) + Value(4)|
| int64 | 9 | Type(1) + Value(8)|
| float | 5 | Type(1) + Value(4)|
| double | 9 | Type(1) + Value(8)|
| stirng | 可变长度 | Type(1) +Length(5) + Value(变成)|
对于string类型，1个字节代表类型，长度用的是int32
### 复合类型序列化+ 反序列化
#### 复合数据类型编码
| 字段类型 | 字段长度(字节) | 底层编码格式 |
| :---:|    :----: | :----: | 
| vector<T> | 可变长 | Type(1) + length(5) + Value(T+T+T+...)|
| list<T> | 可变长 | Type(1) + length(5) + Value(T+T+T+...)|
| map<K,V> | 可变长 | Type(1) + length(5) + Value((k,v)+(k,v)+(k,v)+...)|
| set<T> | 可变长 | Type(1) + length(5) + Value(T+T+T+...)|
其中length代表的int32的表示的长度
#### 自定义类型序列化+ 反序列化
#### 自定义对象类型编码
| 字段类型 | 字段长度(字节) | 底层编码格式 |
| :---:|    :----: | :----: | 
| 自定义类 | 可变长 | Type(1) +Value(D1+D2+D3+...)|
Serializable 接口类
``` C++
class Serializable
{
    public:
        virtual void serializable (DataStream & stream) const =0;
        virtual bool unserializable (DataStream & stream) const =0;
}
```



