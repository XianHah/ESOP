#include "OpsViewer.h"
#include "ui_OpsViewer.h"
#include <qevent.h>
#include <QDebug>
#include <qpainter.h>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>
#include "analysis.h"
#include <algorithm>
#include "LoginServer.h"
#include "Warning.h"

extern QWebSocket *ClientSocket;
extern QString OpsUser;
extern QString MACAddr;
extern QString NoticeScrollTimei;
mutex o_mutex;
QString NoticeText;
int nPos = 0;
bool IsUserConfirm;
bool FirstSendJson = true;

OpsViewer::OpsViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpsViewer)
{
    ui->setupUi(this);
    //ui->BaseWidget->setStyleSheet("border-image: url(:/image/1.png);");
    ui->AllWidget->setStyleSheet("border-image: url();");
    ui->label->setStyleSheet(QString("border-image: url();color:red;"));
    ui->label_2->setStyleSheet(QString("border-image: url();color:blue;"));
    ui->label_3->setStyleSheet(QString("border-image: url();color:blue;"));
    ui->label_4->setStyleSheet(QString("border-image: url();color:red;"));
    ui->label_5->setStyleSheet(QString("border-image: url();color:red;"));
    ui->label->setText(QString::fromLocal8Bit("几米，让连接更简单!"));

    QScreen * screen  =  QGuiApplication::primaryScreen();
    QRect rect =  screen->availableGeometry();
    //this->showMaximized();
    //this->setGeometry(30 ,30 ,rect.width()-400, rect.height()-200);

    setMouseTracking(true);
    ui->AllWidget->setMouseTracking(true);
    ui->LastWidget->setMouseTracking(true);
    ui->StopWidget->setMouseTracking(true);
    ui->StartWidget->setMouseTracking(true);
    ui->NextWidget->setMouseTracking(true);
    ui->splitter->setMouseTracking(true);
    ui->BaseWidget->setMouseTracking(true);
    ui->LogOutWidget->setMouseTracking(true);
    ui->ConfirmWidget->setMouseTracking(true);
    ui->OpenCamWidget->setMouseTracking(true);

    ui->LastWidget->setImage(":/image/last.png");
    ui->NextWidget->setImage(":/image/next.png");
    ui->StopWidget->setImage(":/image/stop.png");
    ui->StartWidget->setImage(":/image/start.png");
    ui->LogOutWidget->setImage(":/image/logout.png");
    ui->ConfirmWidget->setImage(":/image/confirm.png");
    ui->OpenCamWidget->setImage(":/image/OpenCam.png");

    ShowWhatThis = new QLabel(this);
    ShowWhatThis->setMouseTracking(true);
    ShowWhatThis->setStyleSheet("border-image: url();");

    ui->ConfirmWidget->hide();


    for(int i = 0;i<=10;i++)
    {
        QSplitterHandle *splitterHandle = ui->splitter->handle(i);
        if(splitterHandle)
        {
            splitterHandle->setDisabled(true);
            splitterHandle->setMouseTracking(true);
        }
        else
        {
            break;
        }
    }

    //network part
    //Manager = new QNetworkAccessManager;
    connect(ClientSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(RecvMessage(QString)));
    //connect(this,SIGNAL(StartRequestSignal(QUrl)),this,SLOT(StartRequest(QUrl)));

    OldPoint = this->pos();
    OldSize = this->size();

    connect(&ChangePicTimer,SIGNAL(timeout()),this,SLOT(ChangePic()));
    connect(&ChangeNoticeTimer,SIGNAL(timeout()),this,SLOT(ChangNotice()));
    connect(&ScollTimer,SIGNAL(timeout()),this,SLOT(ScollPlayNotice()));
    connect(&ConfirmTimer,SIGNAL(timeout()),this,SLOT(ConfirmSlot()));

    connect(this,SIGNAL(GetShowMessage()),this,SLOT(GetShowMessageSlot()));
    connect(this,SIGNAL(SendConfirmJson()),this,SLOT(SendConfirmJsonSlot()));

    connect(&RunCounter,SIGNAL(SendNum(int)),this,SLOT(GetCountNum(int)));
    connect(&RunCounter,SIGNAL(OpenCamFail(QString)),this,SLOT(OpenCamFailSlot(QString)));
    connect(&RunCounter,SIGNAL(OpenCamSucceed()),this,SLOT(OpenCamSucceedlSlot()));

    connect(&SendCountTimer,SIGNAL(timeout()),this,SLOT(SendCountNumToServer()));

    connect(&AllWidgetHideTimer,SIGNAL(timeout()),this,SLOT(AllWidgetHideSlot()));


    QSettings *setting = new QSettings("./Setting.ini",QSettings::IniFormat);
    SendCountTime = setting->value("COUNTER/SendCountTime").toInt();
}

OpsViewer::~OpsViewer()
{
    delete ui;
}

void OpsViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x()>=ui->AllWidget->x()+11&&
            event->x()<=ui->AllWidget->width()+ui->AllWidget->x()+11&&
            event->y()>=ui->AllWidget->y()+11&&
            event->y()<=ui->AllWidget->y()+ui->AllWidget->height()+10
            )
    {
        if(!ui->AllWidget->isVisible())
        {
            ui->AllWidget->show();
            if(AllWidgetHideTimer.isActive())
            {
                AllWidgetHideTimer.stop();
            }
            AllWidgetHideTimer.setSingleShot(true);
            AllWidgetHideTimer.start(6000);
        }
        if(event->x()>=ui->AllWidget->x()+ui->LastWidget->x()+ui->splitter->x()+11&&
                event->x()<=ui->AllWidget->x()+ui->LastWidget->x()+ui->LastWidget->width()+ui->splitter->x()+11&&
                event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->LastWidget->y()+11&&
                event->y()<=ui->AllWidget->y()+ui->LastWidget->height()+ui->splitter->y()+10) //上一个
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("上一张"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();

        }
        else if (event->x()>=ui->AllWidget->x()+ui->StopWidget->x()+ui->splitter->x()+11&&
                 event->x()<=ui->AllWidget->x()+ui->StopWidget->x()+ui->StopWidget->width()+ui->splitter->x()+11&&
                 event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->StopWidget->y()+11&&
                 event->y()<=ui->AllWidget->y()+ui->StopWidget->height()+ui->splitter->y()+ui->StopWidget->y()+10) //停止
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("停止播放"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();
        }
        else if (event->x()>=ui->AllWidget->x()+ui->StartWidget->x()+ui->splitter->x()+11&&
                 event->x()<=ui->AllWidget->x()+ui->StartWidget->x()+ui->StartWidget->width()+ui->splitter->x()+11&&
                 event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->StartWidget->y()+11&&
                 event->y()<=ui->AllWidget->y()+ui->StartWidget->height()+ui->splitter->y()+ui->StartWidget->y()+10) //开始
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("继续播放"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();
        }
        else if(event->x()>=ui->AllWidget->x()+ui->NextWidget->x()+ui->splitter->x()+11&&
                event->x()<=ui->AllWidget->x()+ui->NextWidget->x()+ui->NextWidget->width()+ui->splitter->x()+11&&
                event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->NextWidget->y()+11&&
                event->y()<=ui->AllWidget->y()+ui->NextWidget->height()+ui->splitter->y()+ui->NextWidget->y()+10) //下一个
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("下一张"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();
        }
        else if (event->x()>=ui->AllWidget->x()+ui->LogOutWidget->x()+ui->splitter->x()+11&&
                 event->x()<=ui->AllWidget->x()+ui->LogOutWidget->x()+ui->LogOutWidget->width()+ui->splitter->x()+11&&
                 event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->LogOutWidget->y()+11&&
                 event->y()<=ui->AllWidget->y()+ui->LogOutWidget->height()+ui->splitter->y()+ui->LogOutWidget->y()+10)
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("退出"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();
        }
        else if (event->x()>=ui->AllWidget->x()+ui->ConfirmWidget->x()+ui->splitter->x()+11&&
                 event->x()<=ui->AllWidget->x()+ui->ConfirmWidget->x()+ui->ConfirmWidget->width()+ui->splitter->x()+11&&
                 event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->ConfirmWidget->y()+11&&
                 event->y()<=ui->AllWidget->y()+ui->ConfirmWidget->height()+ui->splitter->y()+ui->ConfirmWidget->y()+10)
        {
            if(ui->ConfirmWidget->isVisible())
            {
                ShowWhatThis->setText(QString::fromLocal8Bit("已查看"));
                ShowWhatThis->move(event->x()+10,event->y()+10);
                ShowWhatThis->show();
            }
            else
            {
                ShowWhatThis->setText("");
                ShowWhatThis->hide();
            }
        }
        else if (event->x()>=ui->AllWidget->x()+ui->OpenCamWidget->x()+ui->splitter->x()+11&&
                 event->x()<=ui->AllWidget->x()+ui->OpenCamWidget->x()+ui->OpenCamWidget->width()+ui->splitter->x()+11&&
                 event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->OpenCamWidget->y()+11&&
                 event->y()<=ui->AllWidget->y()+ui->OpenCamWidget->height()+ui->splitter->y()+ui->OpenCamWidget->y()+10)
        {
            ShowWhatThis->setText(QString::fromLocal8Bit("打开摄像头"));
            ShowWhatThis->move(event->x()+10,event->y()+10);
            ShowWhatThis->show();
        }
        else
        {
            ShowWhatThis->setText("");
            ShowWhatThis->hide();
        }
    }
    else
    {
        ui->AllWidget->hide();
        ShowWhatThis->hide();
    }
}

void OpsViewer::mousePressEvent(QMouseEvent *event)
{
    if(!ui->AllWidget->isVisible())
    {
        ui->AllWidget->show();
    }
    if(AllWidgetHideTimer.isActive())
    {
        AllWidgetHideTimer.stop();
    }
    AllWidgetHideTimer.setSingleShot(true);
    AllWidgetHideTimer.start(6000);
    if(event->x()>=ui->AllWidget->x()+ui->LastWidget->x()+ui->splitter->x()+11&&
            event->x()<=ui->AllWidget->x()+ui->LastWidget->x()+ui->LastWidget->width()+ui->splitter->x()+11&&
            event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->LastWidget->y()+11&&
            event->y()<=ui->AllWidget->y()+ui->LastWidget->height()+ui->splitter->y()+ui->LastWidget->y()+10) //上一个
    {
        //qDebug()<<"last";
        if(FileNameVec.size()!=0)
        {
            ChangePicTimer.stop();
            if(PicIndex<=2)
            {
                if(IsLastClick)
                {
                    PicIndex-=1;
                    if(PicIndex<0)
                        PicIndex = 0;
                }
                else
                {
                    PicIndex = 0;
                }
            }
            else if(PicIndex>2)
            {
                if(IsLastClick)
                {
                    PicIndex-=1;
                    if(PicIndex<0)
                        PicIndex = 0;
                }
                else
                {
                    PicIndex = PicIndex-2;
                }
            }
            else
                PicIndex = 0;
            ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
            ui->AllWidget->setStyleSheet("border-image: url();");
            ui->ProWidget->setStyleSheet("border-image: url();");
            ShowWhatThis->setStyleSheet("border-image: url();");
            ui->label->setStyleSheet(QString("border-image: url();color:red;"));
            IsLastClick = true;
            ChangePicTimer.start(SwitchTime);
        }
    }
    else if (event->x()>=ui->AllWidget->x()+ui->StopWidget->x()+ui->splitter->x()+11&&
             event->x()<=ui->AllWidget->x()+ui->StopWidget->x()+ui->StopWidget->width()+ui->splitter->x()+11&&
             event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->StopWidget->y()+11&&
             event->y()<=ui->AllWidget->y()+ui->StopWidget->height()+ui->splitter->y()+ui->StopWidget->y()+10) //停止
    {
        //qDebug()<<"stop";
        if(ChangePicTimer.isActive())
            ChangePicTimer.stop();
        //ClientSocket->sendTextMessage("123");
    }
    else if (event->x()>=ui->AllWidget->x()+ui->StartWidget->x()+ui->splitter->x()+11&&
             event->x()<=ui->AllWidget->x()+ui->StartWidget->x()+ui->StartWidget->width()+ui->splitter->x()+11&&
             event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->StartWidget->y()+11&&
             event->y()<=ui->AllWidget->y()+ui->StartWidget->height()+ui->splitter->y()+ui->StartWidget->y()+10) //开始
    {
        //qDebug()<<"start";
        if(!ChangePicTimer.isActive())
            ChangePicTimer.start(SwitchTime);
    }
    else if(event->x()>=ui->AllWidget->x()+ui->NextWidget->x()+ui->splitter->x()+11&&
            event->x()<=ui->AllWidget->x()+ui->NextWidget->x()+ui->NextWidget->width()+ui->splitter->x()+11&&
            event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->NextWidget->y()+11&&
            event->y()<=ui->AllWidget->y()+ui->NextWidget->height()+ui->splitter->y()+ui->NextWidget->y()+10) //下一个
    {
        //qDebug()<<"next";
        ChangePicTimer.stop();
        if(FileNameVec.size()!=0)
        {
            if(PicIndex<FileNameVec.size())
            {
                if(IsLastClick)
                {
                    if(FileNameVec.size()!=1)
                    {
                        PicIndex+=1;
                        ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
                        ui->AllWidget->setStyleSheet("border-image: url();");
                        ui->ProWidget->setStyleSheet("border-image: url();");
                        ShowWhatThis->setStyleSheet("border-image: url();");
                        ui->label->setStyleSheet(QString("border-image: url();color:red;"));
                        PicIndex+=1;
                    }
                    else
                    {
                        ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[0]));
                        ui->AllWidget->setStyleSheet("border-image: url();");
                        ui->ProWidget->setStyleSheet("border-image: url();");
                        ShowWhatThis->setStyleSheet("border-image: url();");
                        ui->label->setStyleSheet(QString("border-image: url();color:red;"));
                    }
                }
                else
                {
                    ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
                    ui->AllWidget->setStyleSheet("border-image: url();");
                    ui->ProWidget->setStyleSheet("border-image: url();");
                    ShowWhatThis->setStyleSheet("border-image: url();");
                    ui->label->setStyleSheet(QString("border-image: url();color:red;"));
                    PicIndex+=1;
                }
                if(PicIndex>=FileNameVec.size())
                {
                    if(!IsUserConfirm)
                    {
                        ui->ConfirmWidget->show();
                        ui->ConfirmWidget->setMouseTracking(true);
                    }
                }
            }
            else
            {
                ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[FileNameVec.size()-1]));
                ui->AllWidget->setStyleSheet("border-image: url();");
                ui->ProWidget->setStyleSheet("border-image: url();");
                ShowWhatThis->setStyleSheet("border-image: url();");
                ui->label->setStyleSheet(QString("border-image: url();color:red;"));
                //PicIndex = (int)FileNameVec.size()-1;
            }
            IsLastClick = false;
            ChangePicTimer.start(SwitchTime);
        }
    }
    else if (event->x()>=ui->AllWidget->x()+ui->LogOutWidget->x()+ui->splitter->x()+11&&
             event->x()<=ui->AllWidget->x()+ui->LogOutWidget->x()+ui->LogOutWidget->width()+ui->splitter->x()+11&&
             event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->LogOutWidget->y()+38&&
             event->y()<=ui->AllWidget->y()+ui->LogOutWidget->height()+ui->splitter->y()+ui->LogOutWidget->y()+38)
    {
        Analysis ana;
        QString JsonStr = ana.SendLogoutJson(OpsUser);
        ClientSocket->sendTextMessage(JsonStr);
    }
    else if (event->x()>=ui->AllWidget->x()+ui->ConfirmWidget->x()+ui->splitter->x()+11&&
             event->x()<=ui->AllWidget->x()+ui->ConfirmWidget->x()+ui->ConfirmWidget->width()+ui->splitter->x()+11&&
             event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->ConfirmWidget->y()+38&&
             event->y()<=ui->AllWidget->y()+ui->ConfirmWidget->height()+ui->splitter->y()+ui->ConfirmWidget->y()+38)
    {
        if(ui->ConfirmWidget->isVisible())
        {
            ChangePicTimer.stop();
            Analysis ana;
            QString JsonStr = ana.SendConfirmJson(OpsUser,false);
            ClientSocket->sendTextMessage(JsonStr);
            ChangePicTimer.start(SwitchTime);
            ui->ConfirmWidget->hide();
            ui->ConfirmWidget->setMouseTracking(false);
            IsUserConfirm = true;
            FirstSendJson = false;
        }
    }
    else if (event->x()>=ui->AllWidget->x()+ui->OpenCamWidget->x()+ui->splitter->x()+11&&
             event->x()<=ui->AllWidget->x()+ui->OpenCamWidget->x()+ui->OpenCamWidget->width()+ui->splitter->x()+11&&
             event->y()>=ui->AllWidget->y()+ui->splitter->y()+ui->OpenCamWidget->y()+11&&
             event->y()<=ui->AllWidget->y()+ui->OpenCamWidget->height()+ui->splitter->y()+ui->OpenCamWidget->y()+10)
    {
        if(RunCounter.ThreadRunFlag!=true)
        {
            RunCounter.ThreadRunFlag = true;
            RunCounter.start();
        }
    }
    else
    {

    }
}

void OpsViewer::SendConfirmJsonSlot()
{
    Analysis ana;
    QString JsonStr = ana.SendConfirmJson(OpsUser,false);
    ClientSocket->sendTextMessage(JsonStr);
    ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[0]));
    ui->AllWidget->setStyleSheet("border-image: url();");
    ui->ProWidget->setStyleSheet("border-image: url();");
    ShowWhatThis->setStyleSheet("border-image: url();");
    ui->label->setStyleSheet(QString("border-image: url();color:red;"));
}

void OpsViewer::GetCountNum(int i)
{
    ui->label_5->setText(QString::fromLocal8Bit("当前产量：")+QString::number(i)+" PCS");
    CountNum = i;
}

void OpsViewer::SendCountNumToServer()
{
    Productiveapacity = CountNum - TempCountNum;
    TempCountNum = CountNum;
    Analysis ana;
    QString JsonStr;
    JsonStr = ana.SendCountNumJson(OpsUser,Productiveapacity,StartCountTime);
    ClientSocket->sendTextMessage(JsonStr);
    ui->label_4->setText(QString::fromLocal8Bit("当前产能：")+QString::number(Productiveapacity)+" PCS/h");
}

void OpsViewer::OpenCamFailSlot(QString str)
{
    QMessageBox::warning(this,QString::fromLocal8Bit("警告"),str);
}

void OpsViewer::OpenCamSucceedlSlot()
{
    StartCountTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    SendCountTimer.start(SendCountTime);
}

void OpsViewer::AllWidgetHideSlot()
{
    if(ui->AllWidget->isVisible())
    {
        ui->AllWidget->hide();
    }
}

void OpsViewer::paintEvent(QPaintEvent *event)
{
    NewPoint = this->pos();
    NewSize = this->size();
    if(NewPoint == OldPoint&&NewSize == OldSize)
    {

    }
    else
    {
        if(!ui->AllWidget->isVisible())
        {
            ui->AllWidget->show();
            if(AllWidgetHideTimer.isActive())
            {
                AllWidgetHideTimer.stop();
            }
            AllWidgetHideTimer.setSingleShot(true);
            AllWidgetHideTimer.start(6000);
        }
        OldPoint = NewPoint;
        OldSize = NewSize;
    }

}

void OpsViewer::RecvMessage(QString message)
{
    Analysis ana;
    ana.GetJsonID(message);
    JsonId = ana.Id;
    if(ana.Type=="show")  //展示信息
    {
        ana.AnalysisReceiveDisplayInformation(message);
        PictureUrl.assign(ana.QlistPicturePath.begin(),ana.QlistPicturePath.end());
        NoticeContent.assign(ana.QlistNoticeContent.begin(),ana.QlistNoticeContent.end());
        NoticeStartTime.assign(ana.QlistNoticeStartTime.begin(),ana.QlistNoticeStartTime.end());
        NoticeEndTime.assign(ana.QlistNoticeEndTime.begin(),ana.QlistNoticeEndTime.end());
        SwitchTime = ana.SwitchInterval*1000;
        ConfirmTime = ana.Confirminterval*60*1000;
        emit GetShowMessage();
    }
    else if(ana.Type=="cancelPicture") //取消展示图片
    {
        ui->ConfirmWidget->hide();
        ui->ConfirmWidget->setMouseTracking(false);
        ChangePicTimer.stop();
        ConfirmTimer.stop();
        PictureUrl.clear();
        //SwitchTime = 0;
        FileNameVec.clear();
        ui->BaseWidget->setStyleSheet("border-image: url();");
        QString JsonStr = ana.SendCancelDisplayInformationJsonData(true,QString::fromLocal8Bit("取消图片成功"),JsonId,"cancelPicture");
        ClientSocket->sendTextMessage(JsonStr);
    }
    else if(ana.Type=="cancelShow") //取消展示全部
    {
        ui->ConfirmWidget->hide();
        ui->ConfirmWidget->setMouseTracking(false);
        ChangePicTimer.stop();
        ConfirmTimer.stop();
        PictureUrl.clear();
        //SwitchTime = 0;
        FileNameVec.clear();
        ui->BaseWidget->setStyleSheet("border-image: url();");
        ChangeNoticeTimer.stop();
        ScollTimer.stop();
        NoticeContent.clear();
        NoticeStartTime.clear();
        NoticeEndTime.clear();
        HistoryNoticeContent.clear();
        NoticeShowVec.clear();
        for(int i = 0;i<(int)NoticeStartTimerID.size();i++)
        {
            if(NoticeStartTimerID[i]!=-1)
                killTimer(NoticeStartTimerID[i]);
            if(NoticeEndTimerID[i]!=-1)
                killTimer(NoticeEndTimerID[i]);
        }
        NoticeStartTimerID.clear();
        NoticeEndTimerID.clear();
        ui->label->setText( QString::fromLocal8Bit("几米，让连接更简单!"));
        QString JsonStr = ana.SendCancelDisplayInformationJsonData(true,QString::fromLocal8Bit("取消展示成功"),JsonId,"cancelShow");
        ClientSocket->sendTextMessage(JsonStr);
    }
    else if(ana.Type=="cancelNotice") //取消展示通知
    {
        ChangeNoticeTimer.stop();
        ScollTimer.stop();
        NoticeContent.clear();
        NoticeStartTime.clear();
        NoticeEndTime.clear();
        HistoryNoticeContent.clear();
        NoticeShowVec.clear();
        for(int i = 0;i<(int)NoticeStartTimerID.size();i++)
        {
            if(NoticeStartTimerID[i]!=-1)
                killTimer(NoticeStartTimerID[i]);
            if(NoticeEndTimerID[i]!=-1)
                killTimer(NoticeEndTimerID[i]);
        }
        NoticeStartTimerID.clear();
        NoticeEndTimerID.clear();
        ui->label->setText( QString::fromLocal8Bit("几米，让连接更简单!"));
        QString JsonStr = ana.SendCancelDisplayInformationJsonData(true,QString::fromLocal8Bit("取消通知成功"),JsonId,"cancelNotice");
        ClientSocket->sendTextMessage(JsonStr);
    }
    else if(ana.Type=="logout")//客户端退出
    {
        //ClientSocket->close();
        PictureUrl.clear();
        NoticeContent.clear();
        HistoryNoticeContent.clear();
        NoticeStartTime.clear();
        NoticeEndTime.clear();
        SwitchTime = 0;
        ChangeNoticeTimer.stop();
        ChangePicTimer.stop();
        ScollTimer.stop();
        ConfirmTimer.stop();
        SendCountTimer.stop();
        FileNameVec.clear();
        NoticeShowVec.clear();
        PicIndex = 0;
        Productiveapacity = 0;
        CountNum = 0;
        TempCountNum = 0;
        for(int i = 0;i<(int)NoticeStartTimerID.size();i++)
        {
            if(NoticeStartTimerID[i]!=-1)
                killTimer(NoticeStartTimerID[i]);
            if(NoticeEndTimerID[i]!=-1)
                killTimer(NoticeEndTimerID[i]);
        }
        NoticeStartTimerID.clear();
        NoticeEndTimerID.clear();
        NoticeText = "";
        RunCounter.ThreadRunFlag = false;
        QThread::msleep(1000);
        StartCountTime.clear();
        this->hide();
        emit ShowLoginDlg();
    }
    else if (ana.Type=="confirm")
    {
        if(FileNameVec.size()!=0)
        {
            if(PicIndex>=0&&PicIndex<FileNameVec.size())
            {
                ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
                ui->AllWidget->setStyleSheet("border-image: url();");
                ui->ProWidget->setStyleSheet("border-image: url();");
                ShowWhatThis->setStyleSheet("border-image: url();");
                ui->label->setStyleSheet(QString("border-image: url();color:red;"));
            }
            else
            {
                ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex-1]));
                ui->AllWidget->setStyleSheet("border-image: url();");
                ui->ProWidget->setStyleSheet("border-image: url();");
                ShowWhatThis->setStyleSheet("border-image: url();");
                ui->label->setStyleSheet(QString("border-image: url();color:red;"));
            }
        }
        if(ConfirmTimer.isActive())
        {
            ConfirmTimer.stop();
        }
    }
    else  //无效json
    {

    }
}

void OpsViewer::GetShowMessageSlot()
{
    Analysis ana;
    FileNameVecSize = FileNameVec.size();
    for(int i = 0;i<(int)PictureUrl.size();i++)
    {
        UrlRequestGet(QUrl(PictureUrl[i]));
        if(FileNameVec.size()==1)
        {
            ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[0]));
            ui->AllWidget->setStyleSheet("border-image: url();");
            ui->ProWidget->setStyleSheet("border-image: url();");
            ui->ProWidget->setStyleSheet("border-image: url();");
            ShowWhatThis->setStyleSheet("border-image: url();");
            ui->label->setStyleSheet(QString("border-image: url();color:red;"));
        }
        if(!ChangePicTimer.isActive())
            ChangePicTimer.start(SwitchTime);
    }
    if(FileNameVecSize!=FileNameVec.size())
    {
        ConfirmTimer.setSingleShot(true);
        ConfirmTimer.start(ConfirmTime);
        IsUserConfirm = false;
        FirstSendJson = true;
    }
    for(int j = 0;j<(int)NoticeContent.size();j++)
    {
        HistoryNoticeContent.push_back(NoticeContent[j]);
        QDateTime time1 = QDateTime::currentDateTime();
        QDateTime time2 = QDateTime::fromString(NoticeStartTime[j],"yyyy-MM-dd HH:mm:ss");
        QDateTime time3 = QDateTime::fromString(NoticeEndTime[j],"yyyy-MM-dd HH:mm:ss");
        int StartId = 0,EndId = 0;
        if(time2<=time1)
        {
            StartId = startTimer(1000);
        }
        else
        {
            StartId = startTimer(time1.secsTo(time2)*1000);
        }
        if(time3<=time1)
        {
            EndId = startTimer(6000);
        }
        else
        {
            EndId = startTimer(time1.secsTo(time3)*1000);
        }
        if(StartId!=0)
            NoticeStartTimerID.push_back(StartId);
        if(EndId!=0)
            NoticeEndTimerID.push_back(EndId);
    }
    QString JsonStr = ana.SendDisplayInformationJsonData(true,"",JsonId);
    ClientSocket->sendTextMessage(JsonStr);
}

void OpsViewer::timerEvent(QTimerEvent *event)
{
    for(int i = 0;i<(int)NoticeStartTimerID.size();i++)
    {
        if(event->timerId()==NoticeStartTimerID[i])
        {
            {
                lock_guard<mutex> lock(o_mutex);
                NoticeShowVec.push_back(HistoryNoticeContent[i]);
                killTimer(NoticeStartTimerID[i]); //定时器触发一次就关闭掉
                NoticeStartTimerID[i] = -1;
            }
            if(!ChangeNoticeTimer.isActive())
                ChangeNoticeTimer.start(SwitchTime);
            break;
        }
        else if (event->timerId()==NoticeEndTimerID[i])
        {
            killTimer(NoticeEndTimerID[i]);
            NoticeEndTimerID[i] = -1;
            vector<QString>::iterator it2 = std::find(NoticeShowVec.begin(),NoticeShowVec.end(),HistoryNoticeContent[i]);
            if(it2!=NoticeShowVec.end())
            {
                {
                    lock_guard<mutex> lock(o_mutex);
                    NoticeShowVec.erase(it2);
                }
                ChangNotice();
            }
            break;
        }
        else
        {

        }
    }
}

void OpsViewer::UrlRequestGet( const QUrl url )
{   
    QNetworkAccessManager qnam;
    const QUrl aurl( url );
    QNetworkRequest qnr( aurl );
    QNetworkReply *reply = qnam.get( qnr );

    QEventLoop eventloop;
    connect( reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec( QEventLoop::ExcludeUserInputEvents);

    QDir dir;
    QFile *file;
    QString DirPath = dir.currentPath();
    if(!dir.exists(DirPath+"\\Image"))
        dir.mkdir("Image");
    QFileInfo info(QUrl(url).path());
    QString filename = info.fileName();
    if(filename.isEmpty())
        filename = "test.png";
    file = new QFile("./Image/"+filename);
    file->open(QIODevice::WriteOnly);
    if(file)
    {
        file->write(reply->readAll() );
        FileNameVec.push_back("./Image/"+filename);
    }
    else
    {
        QMessageBox::warning(this,"warning","open file fail!");
    }
    reply->deleteLater();
    file->flush();
    file->close();
    file->deleteLater();
    file = 0;
    reply = 0;
}

void OpsViewer::ThisShow()
{
    ui->BaseWidget->setStyleSheet(QString("border-image: url();"));
    ui->AllWidget->setStyleSheet("border-image: url();");
    ui->ProWidget->setStyleSheet("border-image: url();");
    ui->label->setStyleSheet(QString("border-image: url();color:red;"));
    ui->label->setText(QString::fromLocal8Bit("几米，让连接更简单！"));
    ui->label_4->setText(QString::fromLocal8Bit("当前产能："));
    this->setWindowTitle("ESOPViewer_V1.0.3         "+QString::fromLocal8Bit("当前用户：")+OpsUser);
    this->showMaximized();
    RunCounter.ThreadRunFlag = true;
    RunCounter.start();
}

void OpsViewer::ChangePic()
{
    if(FileNameVec.size()!=0)
    {
        if(PicIndex<FileNameVec.size())
        {
            ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
            ui->AllWidget->setStyleSheet("border-image: url();");
            ui->ProWidget->setStyleSheet("border-image: url();");
            ShowWhatThis->setStyleSheet("border-image: url();");
            ui->label->setStyleSheet(QString("border-image: url();color:red;"));
            PicIndex+=1;
        }
        else
        {
            PicIndex = 0;
            ui->BaseWidget->setStyleSheet(QString("border-image: url(%1);").arg(FileNameVec[PicIndex]));
            ui->AllWidget->setStyleSheet("border-image: url();");
            ui->ProWidget->setStyleSheet("border-image: url();");
            ShowWhatThis->setStyleSheet("border-image: url();");
            ui->label->setStyleSheet(QString("border-image: url();color:red;"));
            PicIndex+=1;
        }
        if(PicIndex==FileNameVec.size())
        {
            if(!IsUserConfirm)
            {
                ui->ConfirmWidget->show();
                ui->ConfirmWidget->setMouseTracking(true);
            }
        }
        TimerRunning = true;
        IsLastClick = false;
    }
}

void OpsViewer::ChangNotice()
{
    lock_guard<mutex> lock(o_mutex);
    if(NoticeShowVec.size()!=0)
    {
        if(NoticeIndex<NoticeShowVec.size())
        {
            //ui->label->setText(NoticeShowVec[NoticeIndex]);
            if(ScollTimer.isActive())
            {
                ScollTimer.stop();
            }
            NoticeText = NoticeShowVec[NoticeIndex];
            nPos = 0;
            ui->label->setText(NoticeText);
            ScollTimer.start(NoticeScrollTimei.toInt());
            NoticeIndex+=1;
        }
        else
        {
            NoticeIndex = 0;
            if(ScollTimer.isActive())
            {
                ScollTimer.stop();
            }
            //ui->label->setText(NoticeShowVec[NoticeIndex]);
            NoticeText = NoticeShowVec[NoticeIndex];
            nPos = 0;
            ui->label->setText(NoticeText);
            ScollTimer.start(NoticeScrollTimei.toInt());
            NoticeIndex+=1;
        }
    }
    else
    {
        //ui->label->setText(QString::fromLocal8Bit("几米，让连接更简单!"));
        NoticeText = QString::fromLocal8Bit("几米，让连接更简单!");
    }
}

void OpsViewer::ScollPlayNotice()
{   
    if (nPos > NoticeText.length())
        nPos = 0;
    ui->label->setStyleSheet(QString("border-image: url();color:red;"));
    ui->label->setText(NoticeText.mid(nPos));
    nPos++;
}

void OpsViewer::ConfirmSlot()
{
    if(FirstSendJson)
    {
        Analysis ana;
        QString JsonStr = ana.SendConfirmJson(OpsUser,true);
        ClientSocket->sendTextMessage(JsonStr);
        FirstSendJson = false;
    }
    if(!IsUserConfirm)
    {
        Warning WarnDlg;
        WarnDlg.exec();
        if(FileNameVec.size()!=0)
        {
            int ConSeconedConfirmTime;
            ConSeconedConfirmTime = (int)FileNameVec.size()*SwitchTime;
            ConfirmTimer.setSingleShot(true);
            ConfirmTimer.start(ConSeconedConfirmTime);
        }
    }
}

void OpsViewer::closeEvent(QCloseEvent *event)
{
    for(int i = 0; i<FileNameVec.size(); i++)
    {
        QFile file(FileNameVec[i]);
        if(file.exists())
            file.remove();
    }
    RunCounter.ThreadRunFlag = false;
    QThread::msleep(1000);
}
