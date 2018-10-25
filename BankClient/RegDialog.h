#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QString>

namespace Ui {
class RegDialog;
}

class RegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegDialog(QWidget *parent = 0);
    ~RegDialog();

    QString GetUserName();
    QString GetPassWd();

    int Warning();

private:
    Ui::RegDialog *ui;

    //QLineEdit *m_pUsername;
    //QLineEdit *m_pPasswd;
    //QLineEdit *m_pPasswd2;

public slots:
    int confirm();
    void cancel();
};

#endif // REGDIALOG_H
