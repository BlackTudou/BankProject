#include "RegDialog.h"
#include "ui_RegDialog.h"
#include <QDebug>
#include <QMessageBox>  //标准对话框
#include <iostream>

RegDialog::RegDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("RegistDialog");

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(confirm()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));
}

RegDialog::~RegDialog()
{
    delete ui;
}


QString RegDialog::GetUserName()
{
    return ui->name->text();
}

QString RegDialog::GetPassWd()
{
    if(ui->password->text().compare(ui->password2->text()) == 0)
    {
        return ui->password->text();
    }
    else
    {
        //std::count<< "the two password are not equal" << std::endl;
    }
}

int RegDialog::confirm()
{
    if(Warning())
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void RegDialog::cancel()
{
    qDebug() << "cancel";

}

int RegDialog::Warning()
{
    if(ui->name->text().length() < 3)
    {
        QMessageBox::warning(this,"warning","username is too short");
        return 1;
    }

    if(ui->name->text().length() > 10)
    {
        QMessageBox::warning(this,"warning","username is too long");
        return 1;
    }

    if(ui->password->text().length() < 6 )
    {
        QMessageBox::warning(this,"warning","passwd is too short");
        return 1;
    }

    if(ui->password->text().length() > 8 )
    {
        QMessageBox::warning(this,"warning","passwd is too long");
        return 1;
    }

    if(ui->password->text().compare(ui->password2->text()))
    {
        QMessageBox::warning(this,"warning","the two passwords are not the same");
        return 1;
    }

    QByteArray name = ui->name->text().toLatin1();
    char *str = name.data();
    while(*str != '\0')
    {
        if(!((*str >= 'a' && *str <= 'z')||(*str >= 'A' && *str <='Z')))
        {
            QMessageBox::warning(this,"warning","usename is not all letters");
            return 1;
        }
        str++;
    }

    return 0;
}

