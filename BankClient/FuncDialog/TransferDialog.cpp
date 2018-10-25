#include "TransferDialog.h"
#include "ui_TransferDialog.h"
#include "mark.h"
#include <QMessageBox>
#include <QByteArray>

TransferDialog::TransferDialog(QWidget *parent,int client_sock) :
    QDialog(parent),
    ui(new Ui::TransferDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);

    this->setWindowTitle("TransferDialog");

    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

TransferDialog::~TransferDialog()
{
    delete ui;
}

void TransferDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray money = ui->money->text().toLatin1();
    QByteArray password = ui->password->text().toLatin1();
    QByteArray other_id = ui->other_id->text().toLatin1();

    m_pTransfer = Transfer::GetTransfer();
    this->Result(m_pTransfer->Function(m_iClientSfd,id.data(),password.data(),money.data(),other_id.data()));
}

void TransferDialog::Cancel()
{
    reject();
}

int TransferDialog::Warning()
{
    if(ui->id->text().length() == 0)  //无ID
    {
        QMessageBox::warning(this,"warning","Please input ID !");
        return 1;
    }

    if(ui->other_id->text().length() == 0)  //无对方ID
    {
        QMessageBox::warning(this,"warning","Please input Other ID !");
        return 1;
    }

    if(ui->password->text().length() < 6 )   //密码过短
    {
        QMessageBox::warning(this,"warning","passwd is too short");
        return 1;
    }

    if(ui->password->text().length() > 8 )   //密码过长
    {
        QMessageBox::warning(this,"warning","passwd is too long");
        return 1;
    }

    if(!(QString::compare(ui->money->text(),"0"))) //初始金额为0
    {
        QMessageBox::warning(this,"warning","Please input money");
        return 1;
    }

    return 0;
}

void TransferDialog::Result(int action)
{
    switch(action)
    {
        case NO_ACCOUNT: //无帐号
        {
            QMessageBox::critical(this,"error","No Account !!");
        }break;

        case NO_OTHER_ACCOUNT: //无对方帐号
        {
            QMessageBox::critical(this,"error","No Other Account !!");
        }break;

        case NO_MONEY: //无余额
        {
            QMessageBox::critical(this,"error"," Balance is insufficient !!");
        }break;

        case PASSWD_ERROR: //密码错误
        {
            QMessageBox::critical(this,"error","PassWord ERROR !!");
        }break;

        case TRAN_SUCCESS: //转账成功
        {
            accept();
            QMessageBox::about(this,"success","Transfer successfully");
        }break;

    }
}
