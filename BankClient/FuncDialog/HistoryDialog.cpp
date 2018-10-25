#include "HistoryDialog.h"
#include "ui_HistoryDialog.h"
#include "mark.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>

HistoryDialog::HistoryDialog(QWidget *parent,int client_sock) :
    QDialog(parent),
    ui(new Ui::HistoryDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);

    this->setWindowTitle("HistoryDialog");

    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray passwd = ui->password->text().toLatin1();
    QByteArray start_date = ui->start_date->text().toLatin1();
    QByteArray end_date = ui->end_date->text().toLatin1();

    //int len1 = year.size() + month.size() + day.size() ;
    //int len2 = year2.size() + month2.size() + day2.size();

    //char *str1 = new char[len1 + 3];
    //char *str2 = new char[len2 + 3];

    //snprintf(str1,len1+3,"%s-%s-%s",year.data(),month.data(),day.data());
    //snprintf(str2,len2+3,"%s-%s-%s",year2.data(),month2.data(),day2.data());

    qDebug() << "start.date:" << start_date.data();
    qDebug() << "end_date:" << end_date.data();

    m_pHistory = History::GetHistory();
    this->Result(m_pHistory->Function(m_iClientSfd,id.data(),passwd.data(),start_date.data(),end_date.data()));
}

void HistoryDialog::Cancel()
{
    reject();
}

void HistoryDialog::Result(int action)
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

        case HIST_SUCCESS: //查询成功
        {
            accept();
            QMessageBox::about(this,"success",m_pHistory->GetInformation().c_str());
        }break;

        case NO_HISTORY: //无记录
        {
            QMessageBox::about(this,"NULL","History is empty !!");
        }break;

    }
}


int HistoryDialog::Warning()
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
