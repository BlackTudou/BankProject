TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    Abstract.cpp \
    FuncFactory.cpp \
    My_Sql.cpp \
    MyTools.cpp \
    network.cpp \
    Pass.cpp \
    Reg.cpp \
    Selectfunc.cpp \
    MySQLInterface.cpp \
    Func/Open.cpp \
    Func/Deposit.cpp \
    Func/Withdraw.cpp \
    Func/Transfer.cpp \
    Func/Inquiry.cpp \
    Func/ChPasswd.cpp \
    Func/Destroy.cpp \
    Func/History.cpp

HEADERS += \
    Abstract.h \
    DynBase.h \
    FuncFactory.h \
    mark.h \
    My_Sql.h \
    MyTools.h \
    network.h \
    Pass.h \
    Reg.h \
    Selectfunc.h \
    Func/Func.h \
    MySQLInterface.h \
    Func/Open.h \
    Func/Deposit.h \
    Func/Withdraw.h \
    Func/Transfer.h \
    Func/Inquiry.h \
    Func/ChPasswd.h \
    Func/Destroy.h \
    Func/History.h


QMAKE_CXXFLAGS += -std=c++0x


LIBS += -L/usr/lib -lpthread
LIBS += -L/usr/lib -lmysqlclient

