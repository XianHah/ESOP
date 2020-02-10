#include "CountThread.h"
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>
counter c;
CountThread::CountThread()
{

}

void CountThread::run()
{
    QSettings *setting = new QSettings("./Setting.ini",QSettings::IniFormat);
    int TriggerLength ,TriggerHeight ,CamNum ,LearningNum,ShowVideo ,X1,X2,Y1,Y2,ObjMaxSize,ObjMinSize;
    int ObjSize;
    TriggerLength = setting->value("COUNTER/TriggerLength").toInt();
    TriggerHeight = setting->value("COUNTER/TriggerHeight").toInt();
    LearningNum = setting->value("COUNTER/LearningNum").toInt();
    ShowVideo = setting->value("COUNTER/ShowVideo").toInt();
    CamNum = setting->value("COUNTER/CounterCamNum").toInt();
    X1 = setting->value("COUNTER/X1").toInt();
    X2 = setting->value("COUNTER/X2").toInt();
    Y1 = setting->value("COUNTER/Y1").toInt();
    Y2 = setting->value("COUNTER/Y2").toInt();
    ObjMaxSize = setting->value("COUNTER/ObjMaxSize").toInt();
    ObjMinSize = setting->value("COUNTER/ObjMinSize").toInt();
    ObjSize = setting->value("COUNTER/ObjSize").toInt();

    //history video
    int IsSaveVideo,FPS,VideoWidth,VideoHeight;
    IsSaveVideo = setting->value("HistoryVideo/SaveVideo").toInt();
    FPS = setting->value("HistoryVideo/FPS").toInt();
    VideoWidth = setting->value("HistoryVideo/Width").toInt();
    VideoHeight = setting->value("HistoryVideo/Height").toInt();
    cv::VideoWriter writer;
    if(IsSaveVideo)
    {
        QString strPath;
        strPath = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
        strPath+=".avi";
        QDir dir;
        QString DirPath = dir.currentPath();
        if(!dir.exists(DirPath+"\\HistoryVideo"))
            dir.mkdir("HistoryVideo");
        strPath = "./HistoryVideo/"+strPath;
        if(writer.open(strPath.toStdString(), CV_FOURCC('M', 'P', '4', '2'),FPS,cv::Size(VideoWidth,VideoHeight), true))
        {
            qDebug()<<"writer open succeed";
        }
        else
        {
            emit OpenCamFail(QString::fromLocal8Bit("视频录制失败，请重新打开摄像头!"));
            ThreadRunFlag = false;
            return ;
        }
    }

    c.set_trigger_lh_thresold(TriggerLength,TriggerHeight);
    c.set_trigger_cb(cb_lh,cb_hl);
    //roi
    //c.set_roi(cv::Point(20,484),cv::Point(761,495));
    c.set_roi(cv::Point(X1,Y1),cv::Point(X2,Y2));
    c.set_obj_size(ObjSize);
    c.set_leaning_num(LearningNum);
    c.set_obj_maxsize(ObjMaxSize);
    c.set_obj_minsize(ObjMinSize);
    if(0)
    {
        c.enable_debug_window();
    }
    c.init();
    int CountNum = 0;
    //setp4:start counter while
    cv::VideoCapture cap(CamNum);
    if (!cap.isOpened())
    {
       emit OpenCamFail(QString::fromLocal8Bit("视觉计数摄像头打开失败，请重新打开!"));
       ThreadRunFlag = false;
       return ;
    }
    cv::Mat f;
    int flag = 0;
    while(ThreadRunFlag)
    {
        if(cap.read(f))
        {
            if(IsSaveVideo)
            {
              writer << f;
            }
             //step5:feed a image
            c.feed(f);
            //draw the debug info
            f=c.draw(f);
            CountNum = c.get_num();
            emit SendNum(CountNum);
            if(ShowVideo)
            {
                cv::imshow("img",f);
                cv::waitKey(20);
            }
            if(flag==0)
            {
                emit OpenCamSucceed();
                flag++;
            }
        }
    }
    c.num_reset();
    if(IsSaveVideo)
    {
       writer.release();
    }
}

void cb_lh()
{
    c.triger_start_cb();
}
void cb_hl()
{
    c.num_increase_cb();
    //step2:add your callbaclk here
}
