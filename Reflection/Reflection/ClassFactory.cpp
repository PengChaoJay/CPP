#include "ClassFactory.h"
using namespace zpc::reflect;

const string& Object::get_class_name() const
{
    return m_className;
}
void Object::set_class_name(const string& className)
{
    m_className = className;
}


int Object::get_field_count()
{
    ClassFactory* factory = Singleton<ClassFactory>::instance();
    return factory->get_class_field_cout(m_className);
}


ClassField * Object::get_field(int pos)
{
    ClassFactory* factory = Singleton<ClassFactory>::instance();
    return factory->get_class_field(m_className,pos);
}

ClassField * Object::get_field(const string & fieldName)
{
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	return factory->get_class_field(m_className, fieldName);
}

void Object::call(const string& methodName)
{
    ClassMethod* method = Singleton<ClassFactory>::instance()->get_class_method(m_className, methodName);
    auto func = method->method();
	typedef std::function<void(decltype(this))> class_method;
	(*(class_method*)(func))(this);
}





void ClassFactory::register_class(const string& className, create_object method)
{
    m_classMap[className] = method;
}

Object* ClassFactory::create_class(const string& className)
{
    auto it = m_classMap.find(className);
    if (it == m_classMap.end())
    {
        return nullptr;
    }
    return it->second();
}


void ClassFactory::register_class_field(const string& className, const string& fieldName, const string& fieldType, size_t offset)
{
    m_classField[className].push_back(new ClassField(fieldName, fieldType, offset));

}
int ClassFactory::get_class_field_cout(const string& className)
{
    return m_classField[className].size();
}
ClassField* ClassFactory::get_class_field(const string& className, int pos)
{
    int size = m_classField[className].size();
    if (pos<0|| pos>=size)
    {
        return nullptr;
    }
    return m_classField[className][pos];
}
ClassField* ClassFactory::get_class_field(const string& className, const string& fieldName)
{
    auto fields = m_classField[className];
    for (auto it = fields.begin(); it != fields.end(); it++)
    {
        if ((*it)->name() == fieldName)
        {
            return (*it);
        }
    }
    return nullptr;
}


void ClassFactory::register_class_method(const string& className, const string& methodName, uintptr_t method)
{
    m_classMethod[className].push_back(new ClassMethod(methodName, method));

}

int ClassFactory::get_class_method_cout(const string& className)
{
    return m_classMethod[className].size();
}


ClassMethod* ClassFactory::get_class_method(const string& className, int pos)
{
	int size = m_classMethod[className].size();
	if (pos < 0 || pos >= size)
	{
		return nullptr;
	}
	return m_classMethod[className][pos];
}


ClassMethod* ClassFactory::get_class_method(const string& className, const string & methodName)
{
	auto methods = m_classMethod[className];
	for (auto it = methods.begin(); it != methods.end(); it++)
	{
		if ((*it)->name() == methodName)
		{
			return *it;
		}
	}
	return nullptr;
}