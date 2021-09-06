#include "search.h"
#include<QPushButton>
#include<QLineEdit>
#include<QComboBox>
#include<QTableView>
#include<QStandardItemModel>
#include<QLabel>
#include<QGroupBox>
#include<QLayout>
#include<QSqlQuery>
#include<QDateTimeEdit>
#include<QFileDialog>

#include"CenterModule/itemmodelfactory.h"
#include"CenterModule/Configer.h"
#include"Database/databasecontroller.h"
#include"CenterModule/mythreadsearch.h"
#include<QMetaType>
#include<QAction>
#include<QMouseEvent>
#include<QPoint>
#include<QHeaderView>
#include<QMenu>

MayeTableView::MayeTableView(QWidget *parent):QTableView(parent)
{
    contextMenu = new QMenu;
    delAct = new QAction("删除");

    this->setSortingEnabled(true);
    this->verticalHeader()->setFixedWidth(50);

    contextMenu->addAction(delAct);
    //设置右键菜单
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this,&MayeTableView::customContextMenuRequested,[=](const QPoint& pos)
    {
        QModelIndex index = this->currentIndex();
        //获取选中的行数据的助教名称 如：虎牙
        QString userName = qobject_cast<QStandardItemModel*>(model())->item(index.row(),3)->text();
        //获取当前登录的用户名称
        QString nickName = DataBaseController::SingleIstance()->userInfo().nickname;
        //只能删除自己的数据
        if(userName == nickName)
        {
            contextMenu->show();
            QPoint p = mapToGlobal(pos);
            contextMenu->move(p.x()+60,p.y() + contextMenu->height());
        }
    });
    deal();
}

void MayeTableView::deal()
{
    connect(delAct,&QAction::triggered,this,[=]()
    {
        QModelIndex index = this->currentIndex();
        if(!index.isValid())
            return;

        cout<<index.row()<<"  "<<index.column();
        //获取删除的行的所有元素，并从model中移除
        QList<QStandardItem*> item_list = qobject_cast<QStandardItemModel*>(model())->takeRow(index.row());
        //把一行中所有的数据都提取出来
        QStringList delStrData;
        for(QStandardItem*item : item_list)
        {
            delStrData<<item->text();
            delete item;
        }
        emit deleteString(delStrData);
        cout<<"hello  aaa";
    });
}


/*@Search 抽象基类*/
AbstractSearch::AbstractSearch(QWidget *parent):QWidget(parent)
{
    //屏蔽警告： Cannot queue arguments of type 'Qt::Orientation'
    //(Make sure 'Qt::Orientation' is registered using qRegisterMetaType().)
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");


    topHLayout = new QHBoxLayout;
    mainVLayout = new QVBoxLayout;

    searchBtn = new QPushButton("查询");
    tableView = new MayeTableView;
    //禁止编辑
    tableView->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
    tableView->setSelectionBehavior(QTableView::SelectRows);
    tableView->verticalHeader()->setMidLineWidth(50);

    mainVLayout->addLayout(topHLayout);
    mainVLayout->addWidget(tableView);
    this->setLayout(mainVLayout);

    multithreadingSearch();
}

void AbstractSearch::multithreadingSearch()
{
    worker = new MythreadSearch;
    thread = new QThread;
    worker->moveToThread(thread);
    //开始执行线程
    //connect(thread,&QThread::started,worker,&MythreadSearch::createModel);
    connect(thread,&QThread::started,worker,[=]()
    {
        searchBtn->setText("查询中...");
        searchBtn->setEnabled(false);
        worker->createModel();
    });
    //查询完毕返回model
    connect(worker,&MythreadSearch::searchFinished,this,[=](QStandardItemModel*model)
    {
        cout<<"查询完成";
        tableView->setModel(model);
        cout<<"查询完成 1111 rows:"<<model->rowCount();
        thread->quit();
        cout<<"查询完成 2222";
    });
    //线程执行完毕处理
    connect(thread,&QThread::finished,this,[=]()
    {
        searchBtn->setText("查询");cout<<"线程结束";
        searchBtn->setEnabled(true);
    });
}

QDateTimeEdit *AbstractSearch::createDateTimeEdit() const
{
    QDateTimeEdit* newEdit = new QDateTimeEdit;
    newEdit->setDate(QDate::currentDate());
    newEdit->setDisplayFormat("yyyy-MM-dd");
    newEdit->setCalendarPopup(true);
    newEdit->setDateRange(QDate(2019,1,1),QDate::currentDate());
    newEdit->setMaximumWidth(150);;
    return newEdit;
}


/*@助教查询流量*/
AssistantSearch::AssistantSearch(QWidget *parent):AbstractSearch(parent)
{
    searchEdit = new QLineEdit;
    begDateEdit = createDateTimeEdit();
    endDateEdit = createDateTimeEdit();

   //设置顶部
   topHLayout->addWidget(new QLabel("QQ:"));
   topHLayout->addWidget(searchEdit);
   topHLayout->addWidget(begDateEdit);
   topHLayout->addWidget(endDateEdit);
   topHLayout->addWidget(searchBtn);

   connect(searchBtn,&QPushButton::clicked,this,&AssistantSearch::onSearch);

   connect(tableView,&MayeTableView::deleteString,this,[=](const QStringList& data)
   {
        cout<<data;
        QString sql("DELETE FROM freecustomers WHERE qq='%1' AND assistant='%2' AND entrytime='%3'");
        sql = sql.arg(data[0]).arg(data[3]).arg(data[4]);
        cout<<sql;
        QSqlQuery query;
        if(!query.exec(sql))
        {
            QMessageBox::warning(this,"提示","删除失败~");
            return;
        }
        QMessageBox::warning(this,"提示","删除成功~");
   });
}
void AssistantSearch::onSearch()
{
    QString sql("select qq,tel,weixin,assistant,entrytime,source,creater,createtime from freecustomers ");
    //按照日期查找
    if(searchEdit->text().isEmpty())
    {
        QString assNickname = DataBaseController::SingleIstance()->userInfo().nickname;
        sql += QString(" where assistant='%1' and entrytime>='%2' and entrytime<='%3'")
                .arg(assNickname).arg(begDateEdit->text()).arg(endDateEdit->text());
    }
    //按照QQ查找 所有用户的记录
    else
    {
         sql += QString(" where qq='%1'").arg(searchEdit->text());
    }
    worker->setSql(sql);
    thread->start();
    cout<<"------查询开始" <<"  "<<sql;
}

/*@管理员查询流量*/
AdminSearch::AdminSearch(QWidget *parent):AssistantSearch(parent)
{
    assistantChoose = new QComboBox;
    assistantChoose->addItem("选择助教");
    //把所有助教的昵称都加入选择框
    QSqlQuery query;
    query.exec("select CONCAT(nickname) from user");
    while(query.next())
    {
        assistantChoose->addItem(query.value(0).toString());
    }

    topHLayout->insertWidget(topHLayout->count()-1,assistantChoose);


    connect(searchBtn,&QPushButton::clicked,this,&AdminSearch::onSearch);
}
void AdminSearch::onSearch()
{
    QString userCondition;
    if(assistantChoose->currentIndex() == 0)    //无选择
    {
       userCondition = " ";
    }else
    {
        userCondition = QString(" and assistant='%1'").arg(assistantChoose->currentText());
    }

    QString sql("select qq,tel,weixin,assistant,entrytime,source,creater,createtime from freecustomers");
    //按照日期查找
    if(searchEdit->text().isEmpty())
    {
        sql += QString(" where entrytime>='%1' and entrytime<='%2' %3")
                .arg(begDateEdit->text()).arg(endDateEdit->text()).arg(userCondition);
    }
    //按照QQ查找 所有用户的记录
    else
    {
        sql += QString(" where qq='%1' %2").arg(searchEdit->text()).arg(userCondition);
    }

    worker->setSql(sql);
    thread->start();
}

/*@听课数据单个查询*/
CourseDataSingleSearch::CourseDataSingleSearch(QWidget *parent):AbstractSearch(parent)
{
    searchEdit = new QLineEdit;

    topHLayout->addWidget(new QLabel("QQ/微信:  "));
    topHLayout->addWidget(searchEdit);
    topHLayout->addWidget(searchBtn);
    topHLayout->addStretch();


    connect(searchBtn,&QPushButton::clicked,this,&CourseDataSingleSearch::onSearch);
}

void CourseDataSingleSearch::onSearch()
{
    /*
     * SELECT f.qq,l.nickname,f.assistant,f.weixin,f.tel,l.lectures_date,l.course,l.lecture_length,l.lecture_interval
FROM freecustomers f LEFT OUTER JOIN lectures l on f.qq=l.qq
where f.createtime>'2021-07-08' and f.assistant='叶修语' and l.lectures_date='2021-07-08';
     * /
    //DISTINCT lectures_date as aa,
//    QString search = QString("SELECT qq,nickname,assistant,weixin,tel,lectures_date,course,lecture_length,lecture_interval from (SELECT f.assistant,l.qq,l.nickname,l.weixin,f.tel,l.lectures_date,l.course,l.lecture_length,l.lecture_interval from lectures l,freecustomers f"
//                             " WHERE l.qq=f.qq ) as t WHERE qq='%1' OR weixin='%2' OR nickname LIKE '%%3%'")
//            .arg(searchEdit->text()).arg(searchEdit->text()).arg(searchEdit->text());

*/
    QString sql("SELECT DISTINCT f.qq,l.nickname,f.assistant,f.weixin,f.tel,l.lectures_date,l.course,l.lecture_length,l.lecture_interval FROM freecustomers f INNER JOIN lectures l on f.qq=l.qq ");
    sql += QString("WHERE (f.qq='%1' OR f.weixin='%1' OR l.nickname LIKE '%%1%')")
            .arg(searchEdit->text());

    worker->setSql(sql);
    thread->start();
}

/*@听课数据综合查询*/
CourseDataSearch::CourseDataSearch(QWidget *parent):AbstractSearch(parent)
{
    begEnteringDateEdit = createDateTimeEdit();
    endEnteringDateEdit = createDateTimeEdit();

    begLecturesDateEdit = createDateTimeEdit();
    endLecturesDateEdit = createDateTimeEdit();

    assistantChoose = new QComboBox;
    assistantChoose->addItem("选择助教");
    QSqlQuery query;
    query.exec("select nickname from user");
    while(query.next())
    {
        assistantChoose->addItem(query.value(0).toString());
    }

    topHLayout->addWidget(assistantChoose);
    topHLayout->addWidget(new QLabel("录入时间:"));
    topHLayout->addWidget(begEnteringDateEdit);
    topHLayout->addWidget(endEnteringDateEdit);
    topHLayout->addWidget(new QLabel("听课时间"));
    topHLayout->addWidget(begLecturesDateEdit);
    topHLayout->addWidget(endLecturesDateEdit);
    topHLayout->addWidget(searchBtn);
    topHLayout->addStretch();


    connect(searchBtn,&QPushButton::clicked,this,&CourseDataSearch::onSearch);
}

void CourseDataSearch::onSearch()
{
    QString sql("SELECT DISTINCT l.qq,F.assistant,l.nickname,l.weixin,f.tel,l.lectures_date,l.course,l.lecture_length,l.lecture_interval "
                " from lectures l,freecustomers f WHERE l.qq=f.qq ");
    sql += QString(" and f.entrytime>='%1' and f.entrytime<='%2' and l.lectures_date>='%3' and l.lectures_date<='%4' ")
            .arg(begEnteringDateEdit->text()).arg(endEnteringDateEdit->text()).arg(begLecturesDateEdit->text()).arg(endLecturesDateEdit->text());
    if(assistantChoose->currentIndex()!=0)
    {

         sql+=QString(" AND f.assistant='%1'").arg(assistantChoose->currentText());
    }

//    //多表联合查询
//   QString search = QString("SELECT f.assistant,l.qq,l.nickname,l.weixin,f.tel,l.lectures_date,l.course,l.lecture_length,l.lecture_interval from lectures l,freecustomers f "
//                            "WHERE l.qq=f.qq and f.entrytime>='%1' and f.entrytime<='%2'"
//                            "and l.lectures_date>='%3' and l.lectures_date<='%4' "
//                            "and f.assistant='%5';").arg(begEnteringDateEdit->text()).arg(endEnteringDateEdit->text())
//           .arg(begLecturesDateEdit->text()).arg(endLecturesDateEdit->text()).arg(assistantChoose->currentText());
   //QMessageBox::warning(this,"aa",search);
    cout<<sql;
   QSqlQuery query;
    if(query.exec(sql))
    {
        tableView->setModel(ItemModelFactory::create()->createTableModel(query));
    }
}


