#include "WithdrawDialog.h"
#include "ui_WithdrawDialog.h"
#include "mark.h"
#include <QMessageBox>
#include <QByteArray>

WithdrawDialog::WithdrawDialog(QWidget *parent, int client_sock) :
    QDialog(parent),
    ui(new Ui::WithdrawDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);

    this->setWindowTitle("WithdrawDialog");

    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

WithdrawDialog::~WithdrawDialog()
{
    delete ui;
}

void WithdrawDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray money = ui->money->text().toLatin1();
    QByteArray password = ui->password->text().toLatin1();

    m_pWithdraw = Withdraw::GetWithdraw();
    this->Result(m_pWithdraw->Function(m_iClientSfd,id.data(),password.data(),money.data()));
}

void WithdrawDialog::Cancel()
{
    reject();
}

int WithdrawDialog::Warning()
{
    if(ui->id->text().length() == 0)  //无ID
    {
        QMessageBox::warning(this,"warning","Please input ID !");
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
        QMessageBox::warning(this,"warning","No money");
        return 1;
    }

    return 0;
}

void WithdrawDialog::Result(int action)
{
    switch(action)
    {
        case NO_ACCOUNT: //无帐号
        {
            QMessageBox::critical(this,"error","No Account !!");
        }break;

        case NO_MONEY: //无余额
        {
            QMessageBox::critical(this,"error","No Money !!");
        }break;

        case PASSWD_ERROR: //密码错误
        {
            QMessageBox::critical(this,"error","PassWord ERROR !!");
        }break;

        case DRAW_SUCCESS: //取款成功
        {
            accept();
            QMessageBox::about(this,"success","Draw successfully");
        }break;

    }
}

