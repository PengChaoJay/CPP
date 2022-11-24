#pragma once
#include <string>
#include<map>
#include <functional>
#include "ClassField.h"
#include "Singleton.h"
#include "ClassMethod.h"
#include <vector>
using namespace std;
using namespace zpc::utility;
namespace  zpc {
	namespace reflect
	{
		class Object
		{
		public:
			Object() {};
			virtual ~Object() {};
			const string& get_class_name() const;
			void set_class_name(const string& className);


			int get_field_count();
			ClassField* get_field(int pos);
			ClassField* get_field(const string& fieldName);

			template<typename T>
			void get(const string& fieldName, T& value);

			template<typename T>
			void set(const string& fieldName, const T& value);

			void call(const string & methodName);

			virtual void show() = 0;
		private:
			string m_className;
		};

		typedef Object* (*create_object)(void);
		class ClassFactory
		{
			friend class Singleton<ClassFactory>;
		public:
			void register_class(const string& className, create_object method);
			Object* create_class(const string& className);

			//注册成员变量
			void register_class_field(const string& className, const string& fieldName, const string& fieldType,size_t offset);
			int get_class_field_cout(const string& className);
			ClassField* get_class_field(const string& className, int pos);
			ClassField* get_class_field(const string& className, const string& fieldName);

			// 注册成员函数

			void register_class_method(const string& className, const string& methodName, uintptr_t method);
			int get_class_method_cout(const string& className);
			ClassMethod* get_class_method(const string& className, int pos);
			ClassMethod* get_class_method(const string& className, const string& methodName);
	


		private:
			ClassFactory() {}
			~ClassFactory() {}
		private:
			std::map<string, create_object> m_classMap;
			std::map<string, std::vector<ClassField*>> m_classField;
			std::map<string, std::vector<ClassMethod*>> m_classMethod;
		};


		template<typename T>
		void Object::get(const string& fieldName, T& value)
		{
			ClassFactory* factory = Singleton<ClassFactory>::instance();
			ClassField* field = factory->get_class_field(m_className, fieldName);
			size_t offset = field->offset();
			value = *(T*)((unsigned char*)(this) + offset);

		}
		template<typename T>
		void Object::set(const string& fieldName, const T & value)
		{
			ClassFactory* factory = Singleton<ClassFactory>::instance();
			ClassField* field = factory->get_class_field(m_className, fieldName);
			size_t offset = field->offset();
			*(T*)((unsigned char*)(this) + offset) = value;
		}
	}
}