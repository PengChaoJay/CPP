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
				std::cout << "reginster" << className <<"  " << method << std::endl;
				Singleton<ClassFactory>::instance()->register_class(className, method);
			}

			ClassRegister(const string& className, const string& fieldName, const string & fieldType, size_t offset)
			{
				std::cout << "reginster" << className << "  " << fieldName << std::endl;
				Singleton<ClassFactory>::instance()->register_class_field(className, fieldName, fieldType, offset);
			}

			ClassRegister(const string& className, const string& methodName, uintptr_t method)
			{
				std::cout << "reginster" << className << "  dfa" << methodName << std::endl;
				Singleton<ClassFactory>::instance()->register_class_method(className, methodName, method);
			}


		};
#define REGISTER_CLASS(className)			\
	Object* createObject##className()		\
		{									\
			Object* obj = new className();	\
			obj->set_class_name(#className); \
			return obj;						\
		}									\
	ClassRegister ClassRegister##className(#className,createObject##className)
	}


#define REGISTER_CLASS_FIELD(className,fieldName,fieldType) \
	className className##fieldName; \
	ClassRegister ClassRegister##className##fieldName(#className,#fieldName,#fieldType,(size_t)(&className##fieldName.fieldName)-(size_t)(&className##fieldName))

#define REGISTER_CLASS_METHOD(className, methodName) \
    std::function<void(className *)> className##methodName##method = &className::methodName; \
    ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName##method))
}



