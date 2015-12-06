#include "widget.h"
#include "ui_widget.h"
#include "QDebug"
#include "QFile"
#include "QFileDialog"
#include "QTextCodec"
#include "QMessageBox"
#include "windows.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    strList << "单词" << "是否阅读";
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(strList);
    voiceObj = new QAxObject("sapi.spvoice",this);
}
void Widget::AddWord()
{
    Add(ui->qleAddWord->text());
}
void Widget::Add(const QString &add)
{
    ++words;
    ui->tableWidget->setRowCount(words);
    ui->tableWidget->setItem(words - 1,0,new QTableWidgetItem(add));
    ui->tableWidget->setItem(words - 1,1,new QTableWidgetItem("否"));
}

void Widget::DeleteWord()
{
    if (words == 0)
        return;
    else
    {
        --words;
        ui->tableWidget->removeRow(words);
    }
}
void Widget::ReadWord()
{
    voiceObj->setProperty("rate(int)",ui->spSpeedVoice->value());
    ui->tableWidget->setEnabled(false);
    ui->btnAddWord->setEnabled(false);
    ui->btnDeleteWord->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    reading = new QTimer(this);
    connect(reading,SIGNAL(timeout()),this,SLOT(Reading()));
    reading->start(ui->spEveryWord->value() * 1000);
}
void Widget::Reading()
{
    bool read = false;
    for(int i=0;i<words;i++)
    {
        if(QString::compare(ui->tableWidget->item(i,1)->text(),"否") == 0)
        {
            if(i>0)
                voiceObj->dynamicCall("speak(QString)",QString("Next Word."));
            for(int j = 0;j<ui->spReadTimes->value();j++)
            {
                voiceObj->dynamicCall("speak(QString)",ui->tableWidget->item(i,0)->text());
                Sleep(ui->spEveryWord->value());
            }
            ui->tableWidget->setItem(i,1,new QTableWidgetItem("是"));
            read = true;
            return;
        }
    }
    if(!read)
        StopRead();
}
void Widget::AddByTxt()
{
    QString q_filename = QFileDialog::getOpenFileName(this,"Open","","Text Files (*.txt)");
    if(!q_filename.isNull())
    {
       QFile files(q_filename);
       if(files.open(QIODevice::ReadWrite))
       {
           QTextCodec * gbk_codec = QTextCodec::codecForName("gbk");
           QString gbk_string = gbk_codec->toUnicode(files.readAll());
           QStringList strList = gbk_string.split("|");
           for(int i=0;i<strList.count();i++)
                Add(strList.at(i));
       }
    }
}
void Widget::StopRead()
{
    int resultWord = 0;
    reading->stop();
    ui->tableWidget->setEnabled(true);
    ui->btnAddWord->setEnabled(true);
    ui->btnDeleteWord->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    for(int i = 0;i<words;i++)
        if(QString::compare(ui->tableWidget->item(i,1)->text(),"是") == 0)
            ++resultWord;
    QMessageBox::information(this,"Result",QString("本次听写过的单词数为：%1个").arg(QString::number(resultWord)));
}
Widget::~Widget()
{
    delete ui;
}
