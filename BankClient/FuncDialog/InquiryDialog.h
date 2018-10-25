#ifndef INQUIRYDIALOG_H
#define INQUIRYDIALOG_H

#include <QDialog>
#include "Func/Inquiry.h"

namespace Ui {
class InquiryDialog;
}

class InquiryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InquiryDialog(QWidget *parent = 0,int client_sock = 0);
    ~InquiryDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::InquiryDialog *ui;

    int m_iClientSfd; //连接服务器的套接字
    Inquiry * m_pInquiry;  //查询余额操作

private:
    void Result(int action);
    int Warning();
};

#endif // INQUIRYDIALOG_H
