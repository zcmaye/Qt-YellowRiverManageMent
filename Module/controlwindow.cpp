#include "controlwindow.h"
#include<QTableWidget>

ControlWindow::ControlWindow(QWidget *parent) : QStackedWidget(parent)
{
    tabWidgetEnter = new TabWidgetEnter;
    stackeWidgetSearch = new StackedWidgetSearch;
    courseDataEnter = new CourseDataEntering;
    tabWidgetCourseSearch = new TabWidgetCourseSearch;


    this->addWidget(tabWidgetEnter);
    this->addWidget(stackeWidgetSearch);
    this->addWidget(courseDataEnter);
    this->addWidget(tabWidgetCourseSearch);
}

void ControlWindow::setAdministratorPrivileges(bool isAdmin) const
{
    this->isAdmin = isAdmin;
    if(this->isAdmin)
    {
        stackeWidgetSearch->setCurrentIndex(1);
    }else
    {
        stackeWidgetSearch->setCurrentIndex(0);
    }
}

TabWidgetEnter::TabWidgetEnter(QWidget *parent):QTabWidget(parent)
{
    this->addTab(new SingleEntering,"单个录入");
    this->addTab(new BatchEntering,"批量录入");
}

StackedWidgetSearch::StackedWidgetSearch(QWidget *parent):QStackedWidget(parent)
{
    this->addWidget(new AssistantSearch);
    this->addWidget(new AdminSearch);
}



TabWidgetCourseSearch::TabWidgetCourseSearch(QWidget *parent):QTabWidget(parent)
{
    this->addTab(new CourseDataSingleSearch,"单个查询");
    this->addTab(new CourseDataSearch,"综合查询");
}
