#include "DepositDialog.h"
#include "ui_DepositDialog.h"
#include "mark.h"
#include <QMessageBox>
#include <QByteArray>

DepositDialog::DepositDialog(QWidget *parent, int client_sock) :
    QDialog(parent),
    ui(new Ui::DepositDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);
    this->setWindowTitle("DepositDialog");

    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

DepositDialog::~DepositDialog()
{
    delete ui;
}

void DepositDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray money = ui->money->text().toLatin1();

    m_pDeposit = Deposit::GetDeposit();
    this->Result(m_pDeposit->Function(m_iClientSfd,id.data(),money.data()));
}

void DepositDialog::Cancel()
{
    reject();
}

int DepositDialog::Warning()
{
    if(ui->id->text().length() == 0)
    {
        QMessageBox::warning(this,"warning","Please input ID!");
        return 1;
    }

    if(!(QString::compare(ui->money->text(),"0"))) //初始金额为0
    {
        QMessageBox::warning(this,"warning","No money");
        return 1;
    }

    return 0;
}

void DepositDialog::Result(int action)
{
    switch(action)
    {
        case NO_ACCOUNT: //无帐号
        {
            QMessageBox::critical(this,"error","No Account !!");
        }break;

        case DPST_SUCCESS: //存款成功
        {
            accept();
            QMessageBox::about(this,"success","Save successfully");
        }break;

    }
}
