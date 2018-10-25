#include "DestroyDialog.h"
#include "ui_DestroyDialog.h"
#include <QMessageBox>
#include "mark.h"

DestroyDialog::DestroyDialog(QWidget *parent,int client_sock) :
    QDialog(parent),
    ui(new Ui::DestroyDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);

    this->setWindowTitle("DestroyDialog");
    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

DestroyDialog::~DestroyDialog()
{
    delete ui;
}

void DestroyDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray passwd = ui->password->text().toLatin1();

    m_pDestroy = Destroy::GetDestroy();
    this->Result(m_pDestroy->Function(m_iClientSfd,id.data(),passwd.data()));
}

void DestroyDialog::Cancel()
{
    reject();
}

void DestroyDialog::Result(int action)
{
    switch(action)
    {
        case NO_ACCOUNT: //无帐号
        {
            QMessageBox::critical(this,"error","No Account !!");
        }break;

        case PASSWD_ERROR: //密码错误
        {
            QMessageBox::critical(this,"error","PassWord ERROR !!");
        }break;

        case DEST_SUCCESS: //销户成功
        {
            accept();
            QMessageBox::about(this,"success","Dest successfully");
        }break;

    }
}


int DestroyDialog::Warning()
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

    return 0;
}

