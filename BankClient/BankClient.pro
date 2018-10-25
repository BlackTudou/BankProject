#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T20:44:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BankClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sock.cpp \
    RegDialog.cpp \
    transmit.cpp \
    LoginDialog.cpp \
    MD5.cpp \
    FuncDialog/OpenDialog.cpp \
    Func/Open.cpp \
    FuncDialog/DepositDialog.cpp \
    Func/Deposit.cpp \
    FuncDialog/WithdrawDialog.cpp \
    Func/Withdraw.cpp \
    Func/Transfer.cpp \
    FuncDialog/TransferDialog.cpp \
    FuncDialog/InquiryDialog.cpp \
    Func/Inquiry.cpp \
    FuncDialog/ChpasswdDialog.cpp \
    Func/Chpasswd.cpp \
    FuncDialog/DestroyDialog.cpp \
    Func/Destroy.cpp \
    Func/History.cpp \
    FuncDialog/HistoryDialog.cpp

HEADERS  += mainwindow.h \
    sock.h \
    RegDialog.h \
    transmit.h \
    mark.h \
    LoginDialog.h \
    MD5.h \
    FuncDialog/OpenDialog.h \
    Func/Open.h \
    FuncDialog/AllDialog.h \
    FuncDialog/DepositDialog.h \
    Func/Deposit.h \
    FuncDialog/WithdrawDialog.h \
    Func/Withdraw.h \
    Func/Transfer.h \
    FuncDialog/TransferDialog.h \
    FuncDialog/InquiryDialog.h \
    Func/Inquiry.h \
    FuncDialog/ChpasswdDialog.h \
    Func/Chpasswd.h \
    FuncDialog/DestroyDialog.h \
    Func/Destroy.h \
    Func/History.h \
    FuncDialog/HistoryDialog.h

FORMS    += mainwindow.ui \
    RegDialog.ui \
    LoginDialog.ui \
    FuncDialog/DepositDialog.ui \
    FuncDialog/OpenDialog.ui \
    FuncDialog/WithdrawDialog.ui \
    FuncDialog/TransferDialog.ui \
    FuncDialog/InquiryDialog.ui \
    FuncDialog/ChpasswdDialog.ui \
    FuncDialog/DestroyDialog.ui \
    FuncDialog/HistoryDialog.ui

CONFIG += C++11
