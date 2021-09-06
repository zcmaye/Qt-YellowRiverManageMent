#ifndef ENTERING_H
#define ENTERING_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QTableView;
class QStandardItemModel;
class QComboBox;
class QDateTimeEdit;

/*@录入抽象基类*/
class AbstractEntering:public QWidget
{
    Q_OBJECT
public:
    AbstractEntering(QWidget* parent = nullptr);
    QDateTimeEdit *createDateTimeEdit() const;
public slots:
    virtual void onEntering() = 0;
};

/*@单个录入*/
class SingleEntering:public AbstractEntering
{
    Q_OBJECT
public:
    SingleEntering(QWidget* parent = nullptr);
public slots:
    void onEntering() override;
private:
    QLineEdit * qqEdit;
    QLineEdit * wecatEdit;
    QLineEdit * telputEdit;
    QPushButton * enteringBtn;
};

/*@批量录入*/
class BatchEntering:public AbstractEntering
{
    Q_OBJECT
public:
    explicit BatchEntering(QWidget *parent = nullptr);
    void onEntering() override;
    void onEntering1();
protected:
    QLineEdit*    fileNameEdit;
    QPushButton * openFileBtn;
    QPushButton * enterBtn;     //录入


    QTableView*   tableView;
    QStandardItemModel * model;

    QList<QStringList> tableStr;    //把表里面的数据存起来，准备写入数据库
};

/*@上课数据录入*/
class CourseDataEntering:public AbstractEntering
{
    Q_OBJECT
public:
    CourseDataEntering(QWidget *parent = nullptr);
    void onEntering() override;
private:
    QLineEdit*    fileNameEdit;
    QPushButton * openFileBtn;
    QPushButton * enterBtn;     //录入
    QDateTimeEdit *enteringTime;

    QTableView*   tableView;
    QStandardItemModel * model;

    QList<QStringList> tableStr;    //把表里面的数据存起来，准备写入数据库
    QComboBox* courseNameComboBox;
};


#endif // ENTERING_H
