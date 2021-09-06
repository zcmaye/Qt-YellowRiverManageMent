#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include<QSqlQuery>
#include<QTableView>
class QLineEdit;
class QPushButton;
class QTableView;
class QStandardItemModel;
class QComboBox;
class QDateTimeEdit;
class QHBoxLayout;
class QVBoxLayout;
class QSqlQuery;
class QMenu;


class MythreadSearch;
class ItemModelFactory;

class MayeTableView:public QTableView
{
    Q_OBJECT
public:
    MayeTableView(QWidget*parent = nullptr);
    void deal();
signals:
    void deleteString(const QStringList& datas);
private:
    QMenu* contextMenu;
    QAction* delAct;
};


/*@Search 抽象基类*/
class AbstractSearch:public QWidget
{
    Q_OBJECT
public:
    AbstractSearch(QWidget* parent = nullptr);
    void multithreadingSearch();
public slots:
    virtual void onSearch() = 0;
    QDateTimeEdit* createDateTimeEdit()const;
protected:
    //顶部布局
    QHBoxLayout* topHLayout;
    //整体布局
    QVBoxLayout* mainVLayout;

    //QLineEdit* searchEdit;
    QPushButton *searchBtn;
    //QDateTimeEdit * begdateTimeEdit;
    //QDateTimeEdit * enddateTimeEdit;
    QStandardItemModel* model;
    MayeTableView * tableView;

protected:  //多线程相关
    MythreadSearch* worker;
    QThread* thread;
};

/*@助教查询-助教流量*/
class AssistantSearch:public AbstractSearch
{
    Q_OBJECT
public:
    AssistantSearch(QWidget* parent = nullptr);
public slots:
    void onSearch() override;
protected:
    QLineEdit* searchEdit;
    QDateTimeEdit *begDateEdit;
    QDateTimeEdit* endDateEdit;
};

/*@管理员查询-助教流量*/
class AdminSearch:public AssistantSearch
{
    Q_OBJECT
public:
    AdminSearch(QWidget* parent = nullptr);
public slots:
    void onSearch() override;
private:
    QComboBox* assistantChoose;
};

/*@听课数据单个QQ查询*/
class CourseDataSingleSearch:public AbstractSearch
{
    Q_OBJECT
public:
    CourseDataSingleSearch(QWidget* parent = nullptr);
public slots:
    void onSearch() override;
private:
    QLineEdit*searchEdit;
};

/*@听课数据综合查询*/
class CourseDataSearch:public AbstractSearch
{
    Q_OBJECT
public:
    CourseDataSearch(QWidget* parent = nullptr);
public slots:
    void onSearch() override;
private:
    QDateTimeEdit* begEnteringDateEdit;        //听课起始时间
    QDateTimeEdit* endEnteringDateEdit;        //听课结束时间

    QDateTimeEdit* begLecturesDateEdit;        //听课起始时间
    QDateTimeEdit* endLecturesDateEdit;        //听课结束时间
    QComboBox* assistantChoose;
};

#endif // SEARCH_H
