#ifndef FUNCFACTORY_H
#define FUNCFACTORY_H

#include "Abstract.h"
#include "Func/Func.h"

class pass : public AB_Factory
{
public:
    AB_Function *CreateFunc();
};

class regi : public AB_Factory
{
public:
    AB_Function *CreateFunc();
};

class open : public AB_Factory
{
public :
    AB_Function * CreateFunc();
};

class dpst : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class draw : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class tran : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class inqu : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class chpw : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class dest : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};

class hist : public AB_Factory
{
public:
   AB_Function * CreateFunc();
};


#endif // FUNCFACTORY_H
