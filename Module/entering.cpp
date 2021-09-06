#include "entering.h"
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


/*@录入抽象基类*/
AbstractEntering::AbstractEntering(QWidget *parent):QWidget(parent)
{

}

QDateTimeEdit *AbstractEntering::createDateTimeEdit() const
{
    QDateTimeEdit* newEdit = new QDateTimeEdit;
    newEdit->setDate(QDate::currentDate());
    newEdit->setDisplayFormat("yyyy-MM-dd");
    newEdit->setCalendarPopup(true);
    newEdit->setDateRange(QDate(2019,1,1),QDate::currentDate());
    newEdit->setMaximumWidth(150);;
    return newEdit;
}


/*@单个录入*/
SingleEntering::SingleEntering(QWidget *parent):AbstractEntering(parent)
{
    QGroupBox * groupBox =new QGroupBox;
    auto groupLayout = new QGridLayout;


    QLabel *qqLabel = new QLabel("QQ:");
    QLabel *wecatLabel = new QLabel("微信:");
    QLabel *telLabel = new QLabel("电话:");

     qqEdit = new QLineEdit;
     wecatEdit = new QLineEdit;
     telputEdit = new QLineEdit;
     enteringBtn = new QPushButton("录入");

    groupLayout->addWidget(qqLabel,0,0);
    groupLayout->addWidget(qqEdit,0,1);
    groupLayout->addWidget(wecatLabel,1,0);
    groupLayout->addWidget(wecatEdit,1,1);
    groupLayout->addWidget(telLabel,2,0);
    groupLayout->addWidget(telputEdit,2,1);
    groupLayout->addWidget(enteringBtn,3,1);

    groupBox->setLayout(groupLayout);


    QGridLayout *glayout = new QGridLayout;
    glayout->setRowStretch(0,1);
    glayout->setColumnStretch(0,1);

    glayout->addWidget(groupBox,1,1,2,2);

    glayout->setRowStretch(3,1);
    glayout->setColumnStretch(3,1);
    setLayout(glayout);

    setStyleSheet("QLineEdit{border-radius:5px;"
                  "border:1px solid rgb(180,180,180);"
                  "background-color: white;"
                  "height:25px;width:150px;}"
                  "QPushButton{background-color:rgba(5,186,251,255);"
                  "border-radius:5px;"
                  "height:25px;width:150px;}"
                  "QPushButton:hover{background-color:rgba(31,199,253,255);}");

    connect(enteringBtn,&QPushButton::clicked,this,&SingleEntering::onEntering);
}
void SingleEntering::onEntering()
{
    QString qq = qqEdit->text();
    QString wecat =wecatEdit->text();
    QString tel = telputEdit->text();
    if(qq.isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入要插入的学员QQ");
        return;
    }
    QSqlQuery q;
    bool isok = q.exec(QString("insert into freecustomers(qq,tel,weixin,userid,assistant,entrytime,source,images,creater,createtime) values"
                   "('%1','%3','%2',520,'顽石','666/6/6','C语言Plus群','NULL','顽石','6/6/6')").arg(qq).arg(wecat).arg(tel));
    if(isok)
        cout<<"数据插入成功...";
    else
         cout<<"数据插入失败...";
}

/*@批量录入*/
BatchEntering::BatchEntering(QWidget *parent):AbstractEntering(parent)
{
    fileNameEdit = new QLineEdit;   //显示文件路径
    openFileBtn = new QPushButton("选择文件");  //选择文件
    enterBtn = new QPushButton("确定录入");
    tableView = new QTableView;                 //预览数据

    model = new QStandardItemModel;
    fileNameEdit->setFixedSize(150,25);


    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(fileNameEdit);
    hlayout->addWidget(openFileBtn);
    hlayout->addWidget(enterBtn);
    hlayout->addStretch();

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addWidget(tableView);

    setLayout(vlayout);

    connect(openFileBtn,&QPushButton::clicked,this,&BatchEntering::onEntering);
}
void BatchEntering::onEntering()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择数据文件","./","CSV(*.csv)");
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"waring","请选择有效的文件");
        return;
    }
    fileNameEdit->setText(fileName);

    //避免用户一直选择文件,先清空一下
     tableStr.clear();
     model->clear();

    //读取文件，并显示到tableView上
    QFile read(fileName);
    if(!read.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout<<fileName<< "打开失败";
        QMessageBox::warning(this,"waring","文件有误打开失败");
        return;
    }
    QTextStream stream(&read);
    //设置表头
    QStringList headerField = stream.readLine().split(",");
    model->setHorizontalHeaderLabels(headerField);

    //设置内容
    while (!stream.atEnd())
    {
        //读出一行
        QString line = stream.readLine();
        //跳过空行(表里面的数据只有逗号的时候就是空行，逗号个数等于表头数据个数减1)
        if(line.size() == headerField.size()-1 || line.isEmpty())
        {
            cout<<line<<" "<<line.size()<<" "<<headerField.size();
            continue;
        }
        QStringList str_list =  line.split(",");
        //把最后的空格删掉，用当前系统时间替换
        str_list.replace(str_list.size()-1,QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss"));

        //把数据暂时存起来
        tableStr<<str_list;

        //放入model
        QList<QStandardItem*> items;
        for(auto&single_DataStr:str_list)
        {
             items << new QStandardItem(single_DataStr);
        }
        model->appendRow(items);
    }
    tableView->setModel(model);

    connect(enterBtn,&QPushButton::clicked,this,[=]()
    {
        if(fileNameEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"警告","请选择要导入的文件");
            return;
        }
        QSqlQuery q;
        int userId = DataBaseController::SingleIstance()->userInfo().id;
#if 0
        //model->rowCount会取走一行元素
        while(model->rowCount())
        {
             QStringList datas;
             QList<QStandardItem*> items = model->takeRow(0);
             for(QStandardItem* item : items)
             {
                   datas<<item->text();
             }
            QString sql = QString("insert into freecustomers(qq,tel,weixin,userid,assistant,entrytime,source,images,creater,createtime) ");
            sql += QString(" values('%1','','%2',%3,'%4','%5','%6','%7','%8','%9')")
                    .arg(datas[0]).arg(datas[3]).arg(userId).arg(datas[1]).arg(datas[2]).arg(datas[4]).arg(datas[5]).arg(datas[6]).arg(datas[7]);
            static int cnt = 0;
            cout<<datas.size()<<"  "<<datas<<"  cnt:"<<cnt++<<"       "<<model->rowCount();
            q.exec(sql);
        }
        cout<<"录入完成";
#else
        int failedNum = 0;      //插入失败的个数
        for(auto&data:tableStr)
        {
            QString sql = QString("insert into freecustomers(qq,tel,weixin,userid,assistant,entrytime,source,images,creater,createtime) "
                                  " values('%1','','%2',%3,'%4','%5','%6','%7','%8','%9')")
                    .arg(data[0]).arg(data[3]).arg(userId).arg(data[1]).arg(data[2]).arg(data[4]).arg(data[5]).arg(data[6]).arg(data[7]);
           if(!q.exec(sql))
           {
                failedNum++;
           }
        }
        if(failedNum !=0)
        {
            QMessageBox::warning(this,"waring",QString("共%1条数据,%2失败/%3成功").arg(tableStr.size()).arg(failedNum).arg(tableStr.size() - failedNum));
        }else
        {
            QMessageBox::warning(this,"录入成功~",QString("%1条数据全部录入成功").arg(tableStr.size()));
        }
#endif
    });
}
void BatchEntering::onEntering1()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择数据文件","./","CSV(*.csv)");
    fileNameEdit->setText(fileName);

    //读取文件，并显示到tableView上
    QFile read(fileName);
    if(!read.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout<<fileName<< "打开失败";
    }
    QTextStream stream(&read);
    //设置表头
    QStringList headerField = stream.readLine().split(",");
    model->setHorizontalHeaderLabels(headerField);

    //设置内容
    while (!stream.atEnd())
    {
        //读出一行
        QString line = stream.readLine();
        //跳过空行(表里面的数据只有逗号的时候就是空行，逗号个数等于表头数据个数减1)
        if(line.size() == headerField.size()-1 || line.isEmpty())
        {
            cout<<line<<" "<<line.size()<<" "<<headerField.size();
            continue;
        }
        //把这行数据分隔并存起来
        tableStr.push_back(line.split(","));
        tableStr.back().pop_back(); //把最后的空格删掉，用当前系统时间替换
        tableStr.back()<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    }
    //构造model
    for(auto&line_DataStr:tableStr)
    {
        QList<QStandardItem*> items;
        for(auto&single_DataStr:line_DataStr)
        {
             items << new QStandardItem(single_DataStr);
        }
        model->appendRow(items);
    }
    tableView->setModel(model);

    connect(enterBtn,&QPushButton::clicked,this,[=]()
    {
        if(fileNameEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"警告","请选择要导入的文件");
            return;
        }
        QSqlQuery q;
        QString dateTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
        int userId = DataBaseController::SingleIstance()->userInfo().id;
        for(auto&data:tableStr)
        {
            QString sql = QString("insert into freecustomers(qq,tel,weixin,userid,assistant,entrytime,source,images,creater,createtime)"
                                  "values('%1','','%2',%3,'%4','%5','%6','image','%7','%8')")
                    .arg(data[0]).arg(data[3]).arg(userId).arg(data[1]).arg(data[2]).arg(data[4]).arg(data[6]).arg(dateTime);
           cout<<data[2]<<"   "<<dateTime;
           q.exec(sql);
        }
    });
}

/*@上课数据录入*/
CourseDataEntering::CourseDataEntering(QWidget *parent):AbstractEntering(parent)
{
    delete this->layout();  //先删除从父类继承过来的布局
    courseNameComboBox = new QComboBox;
    QSqlQuery query;
    query.exec("select coursename from course");
    while(query.next())
    {
        courseNameComboBox->addItem(query.value(0).toString());
    }

    fileNameEdit = new QLineEdit;   //显示文件路径
    openFileBtn = new QPushButton("选择文件");  //选择文件
    enterBtn = new QPushButton("确定录入");
    tableView = new QTableView;                 //预览数据
    enteringTime = new QDateTimeEdit;
    enteringTime->setDate(QDate::currentDate());
    enteringTime->setDisplayFormat("yyyy-MM-dd");
    enteringTime->setCurrentSection(QDateTimeEdit::DaySection);


    model = new QStandardItemModel;
    fileNameEdit->setFixedSize(150,25);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(courseNameComboBox);
    hlayout->addWidget(enteringTime);
    hlayout->addWidget(fileNameEdit);
    hlayout->addWidget(openFileBtn);
    hlayout->addWidget(enterBtn);
    hlayout->addStretch();

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addWidget(tableView);

    setLayout(vlayout);

    connect(openFileBtn,&QPushButton::clicked,this,&CourseDataEntering::onEntering);
    connect(enterBtn,&QPushButton::clicked,this,[=]()
    {
        if(fileNameEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"警告","请选择要导入的文件");
            return;
        }
        QSqlQuery q;
        for(auto&data:tableStr)
        {
            QString sql = QString("insert into lectures(nickname,qq,weixin,lectures_date,course,lecture_length,lecture_interval)"
                                  "values('%1','%2','%3','%4','%5','%6','%7')")
                    .arg(data[0]).arg(data[1]).arg(data[2]).arg(enteringTime->text()).arg(courseNameComboBox->currentText()).arg(data[3]).arg(data[4]);
           q.exec(sql);
        }
    });
}
void CourseDataEntering::onEntering()
{
    QString fileName = QFileDialog::getOpenFileName(this,"浏览文件","./","CSV(*.csv)");
    fileNameEdit->setText(fileName);

    //避免用户一直选择文件,先清空一下
     tableStr.clear();
     model->clear();

    QFile read(fileName);
    if(!read.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout<<fileName<<" open failed";
        return;
    }
    QTextStream stream(&read);
    QStringList headerFiled =  stream.readLine().split(",");
    headerFiled.insert(2,"微信");
    model->setHorizontalHeaderLabels(headerFiled);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        //因为上面自己插入了一个微信字段，所以字段要减2
        if(line.size() == headerFiled.size()-2 || line.isEmpty())
        {
            break;
        }

        QStringList line_datas = line.split(",");
        bool isok = false;
        line_datas.at(1).toLongLong(&isok);
        if(isok)
        {
            line_datas.insert(2,"");
        }else
        {
            line_datas.insert(1,"");
        }
        tableStr<<line_datas;
    }

    for(auto &line_str:tableStr)
    {
        QList<QStandardItem*> items;
        for(auto&val :line_str)
        {
            items<<new QStandardItem(val);
        }
        model->appendRow(items);
    }
    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
}
