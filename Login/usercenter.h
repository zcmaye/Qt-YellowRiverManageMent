#ifndef USERCENTER_H
#define USERCENTER_H

#include <QDialog>
class QLabel;
class QPushButton;
class QLineEdit;

class UserCenter : public QDialog
{
    Q_OBJECT
public:
    explicit UserCenter(QWidget *parent = nullptr);
    void connectSignalsAndSlots();
signals:
private:
    QLineEdit* oldPasswdEdit;
    QLineEdit* newPasswdEdit;
    QLineEdit* newPasswdAgainEdit;
    QPushButton* modifyBtn;

};

#endif // USERCENTER_H
