#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QStackedWidget>
#include<QTableWidget>
#include"Module/entering.h"
#include"Module/search.h"

class TabWidgetEnter;
class StackedWidgetSearch;
class CourseDataEntering;
class CourseDataSearch;
class TabWidgetCourseSearch;

class ControlWindow : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ControlWindow(QWidget *parent = nullptr);
    //设置管理员权限
    void setAdministratorPrivileges(bool isAdmin)const;

signals:
private:
    mutable bool isAdmin;

    TabWidgetEnter* tabWidgetEnter;
    StackedWidgetSearch* stackeWidgetSearch;
    CourseDataEntering* courseDataEnter;
    TabWidgetCourseSearch*tabWidgetCourseSearch;
};

class StackedWidgetSearch: public QStackedWidget
{
    Q_OBJECT
public:
    StackedWidgetSearch(QWidget *parent = nullptr);
private:
    AssistantSearch *selfSearch;
    AdminSearch* adminSearch;
};

//录入标签窗口
class TabWidgetEnter : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetEnter(QWidget *parent = nullptr);
private:

};

class TabWidgetCourseSearch:public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetCourseSearch(QWidget *parent = nullptr);
private:
};


#endif // CONTROLWINDOW_H
