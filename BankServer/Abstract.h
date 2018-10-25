#ifndef ABSTRACT_H
#define ABSTRACT_H

class AB_Function //功能函数的基类
{
public :
    virtual int Function(int cfd) = 0;
    virtual ~AB_Function();

};

class AB_Factory //生产函数的工厂的基类
{
public :
    virtual AB_Function *CreateFunc() = 0;
    virtual ~AB_Factory();
};


#endif // ABSTRACT_H
