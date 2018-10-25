#ifndef DYNBASE_H
#define DYNBASE_H

#include <map>
#include <string>
using namespace std;

typedef void* (*CREATE_FUNC)();

class DynObjectFactory
{
public:
    static void* CreateObject(const string& name)
    {
        map<string, CREATE_FUNC>::const_iterator it;
        it = mapCls_.find(name);
        if (it == mapCls_.end())
            return 0;
        else
            return it->second();

    }

    static void Register(const string& name, CREATE_FUNC func)
    {
        mapCls_[name] = func;//"Circle  func"
    }
private:
    static map<string, CREATE_FUNC> mapCls_;
};

// g++
__attribute ((weak)) map<string, CREATE_FUNC> DynObjectFactory::mapCls_;

class Register
{
public:
    Register(const string& name, CREATE_FUNC func)
    {
        DynObjectFactory::Register(name, func);
    }
};

#define REGISTER_CLASS(class_name) \
class class_name##Register { \
public: \
    static void* NewInstance() \
    { \
        return new class_name; \
    } \
private: \
    static Register reg_; \
}; \
Register class_name##Register::reg_(#class_name, class_name##Register::NewInstance)

#endif // DYNBASE_H
