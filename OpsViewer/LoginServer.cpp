#include "LoginServer.h"
#include "ui_LoginServer.h"
#include <QNetworkInterface>
#include <QWebSocket>
#include <QDebug>
#include "Register.h"
#include <vector>
#include <mutex>
#include <strmif.h>
#include <initguid.h>
#include <string>
#include <QThread>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include "ArcFaceEngine.h"
#include <QSettings>
#include "analysis.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMetaType>
#include <OpsViewer.h>

using namespace cv;
#define FACE_FEATURE_SIZE 1032
#define VIDEO_FRAME_DEFAULT_WIDTH 640
#define VIDEO_FRAME_DEFAULT_HEIGHT 480
#define appID "HEAZoCjLjBZzCbv84N4s4dJz7g93TyKujSRp6u5aywSZ"
#define sdkKey "GpEGXY2VX9ThyKufds3ohw3bhS5n196ewrwYrqvUbLXV"

//全局
ArcFaceEngine VideoEngine;
ArcFaceEngine ImageEngine;
QWebSocket *ClientSocket;
vector<ASF_FaceFeature>FeatureVec;
vector<QString>UserName;
IplImage  *frame;
ASF_SingleFaceInfo faceInfo;
mutex g_mutex;
FaceDetect *FaceDectObj;
QString IP,Port,RGBThreshold ,IRThreshold ,CompaerThreshold ,CamNum ,WebUrl,NoticeScrollTimei;
GetFeaFromServer *FeatureFromServer;
QString OpsUser;
QString MACAddr;
bool UserLogoutFlag = false;

LoginServer::LoginServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginServer)
{
    ui->setupUi(this);
    //LoginDlg = new LoginServer;
    //LoginDlg = this;
    QSettings *setting = new QSettings("./Setting.ini",QSettings::IniFormat);
    IP = setting->value("Server/IP").toString();
    Port = setting->value("Server/PORT").toString();
    RGBThreshold = setting->value("Threshold/RGBThreshold").toString();
    IRThreshold = setting->value("Threshold/IRThreshold").toString();
    CompaerThreshold = setting->value("Threshold/CompaerThreshold").toString();
    CamNum = setting->value("Cam/CamNum").toString();
    WebUrl = setting->value("WebSocketServer/URL").toString();
    NoticeScrollTimei = setting->value("ScollTime/Time").toString();
    ui->lineEdit_2->setText(WebUrl);

    MACAddr = getHostMacAddress();
    ui->lineEdit->setText(MACAddr);

    ui->lineEdit_2->setFocus();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->label_3->hide();
    ui->lineEdit_3->hide();
    ui->pushButton_2->hide();
    ui->pushButton_4->hide();
    ui->CamLable->setScaledContents(true);
    ui->MainCam->setScaledContents(true);
    ui->dockWidget->hide();
    //connect(&GetFrameTimer,SIGNAL(timeout()),this,SLOT(readFarme()));
    //OpenCam();
    MRESULT faceRes = ImageEngine.ActiveSDK((char*)appID,(char*)sdkKey,0);
    ui->textEdit_2->append("ImageEngine ActiveSDK "+QString::number(faceRes));
    faceRes = ImageEngine.InitEngine(ASF_DETECT_MODE_IMAGE);
    ui->textEdit_2->append("ImageEngine InitEngine "+QString::number(faceRes));
    faceRes = VideoEngine.InitEngine(ASF_DETECT_MODE_VIDEO);
    ui->textEdit_2->append("VideoEngine InitEngine "+QString::number(faceRes));
    //connect(this,SIGNAL(SendIplImage(IplImage*,ASF_SingleFaceInfo)),this,SLOT(CompareFace(IplImage*,ASF_SingleFaceInfo)));
    ui->textEdit_2->append(QString::fromLocal8Bit("正在加载人脸库..."));
    FeatureFromServer = new GetFeaFromServer;
    FeatureFromServer->start();
    qRegisterMetaType<vector<QString>>("QstringVec");
    qRegisterMetaType<vector<ASF_FaceFeature>>("AsfVec");
    connect(FeatureFromServer,SIGNAL(GetFeaFromServerSucceed(QstringVec,AsfVec)),this,SLOT(GetFeaFromServerSucceedSlot(QstringVec,AsfVec)));
    connect(FeatureFromServer,SIGNAL(GetFeaFromServerFail()),this,SLOT(GetFeaFromServerFailSlot()));

    FaceDectObj = new FaceDetect;
    FaceDectObj->ThreadRunFlag = true;
    FaceDectObj->start();
    connect(FaceDectObj,SIGNAL(CompareSucceed(QString)),this,SLOT(CompareSucceed(QString)));
    connect(FaceDectObj,SIGNAL(SendImg(IplImage*)),this,SLOT(readFarme(IplImage*)));

    ClientSocket = new QWebSocket;
    connect(ClientSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(LoginRecvMessage(QString)));
    connect(ClientSocket,SIGNAL(connected()),this,SLOT(SendLoginJson()));
    connect(&LoginTimeOut,SIGNAL(timeout()),this,SLOT(LoginTimeOutSlot()));
}

LoginServer::~LoginServer()
{
    VideoEngine.UnInitEngine();
    ImageEngine.UnInitEngine();
    delete ui;
}

void LoginServer::on_pushButton_2_clicked()
{
   // reject();
    //Analysis ana;
    //ana.SendDisplayInformationJsonData(true,"456");
}

void LoginServer::on_pushButton_clicked()
{
    QString Url = ui->lineEdit_2->text();
   // QString JsonStr = SendDisplayInformationJsonData();
    Url = "ws://"+Url;
    ClientSocket->open(QUrl(Url));

    accept();
}


QString LoginServer::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}



void LoginServer::on_pushButton_3_clicked()   //注册人脸
{
    Register res;
    FaceDectObj->ThreadRunFlag = false;
    if(res.exec()==QDialog::Accepted)
    {
        ui->dockWidget->show();
    }
    else
    {
        FaceDectObj->ThreadRunFlag = true;
        FaceDectObj->start();
    }
}

void LoginServer::on_pushButton_4_clicked()//打开摄像头
{
    QString ButtonText;
    ButtonText = ui->pushButton_4->text();
    QThread::msleep(2000);
    if(ButtonText=="open cam")
    {
        ui->pushButton_4->setText(tr("close cam"));
        OpenCam();
    }
    else
    {
        ui->pushButton_4->setText(tr("open cam"));
        GetFrameTimer.stop();
        cvReleaseCapture(&cam);
        ui->MainCam->setPixmap(QPixmap(""));
    }
}

void LoginServer::OpenCam()
{
    cam = cvCreateCameraCapture(0);
    GetFrameTimer.start(10);
}

void LoginServer::CloseCam()
{
    GetFrameTimer.stop();
    cvReleaseCapture(&cam);//释放内存；
}

void LoginServer::readFarme(IplImage *iframe)//读摄像头帧
{
    QImage image = QImage((const uchar*)iframe->imageData, iframe->width, iframe->height, QImage::Format_RGB888).rgbSwapped();
    ui->MainCam->setPixmap(QPixmap::fromImage(image.mirrored(true,false)));
}

void LoginServer::CompareSucceed(QString str)  //dneglu
{
    OpsUser = str;
    ui->textEdit_2->append(str+QString::fromLocal8Bit("--识别成功，开始登录"));
    QString Url = ui->lineEdit_2->text();
    Url = "ws://"+Url;
    ClientSocket->open(QUrl(Url));
    LoginTimeOut.setSingleShot(true);
    LoginTimeOut.start(6000);
}

void LoginServer::SendLoginJson()
{
    Analysis ana;
    QString JsonStr = ana.SendLoginInformationJsonData(MACAddr,OpsUser);
    ClientSocket->sendTextMessage(JsonStr);
    //qDebug()<<JsonStr;
}

void LoginServer::LoginTimeOutSlot()
{
    FaceDectObj->ThreadRunFlag = true;
    FaceDectObj->start();
}

void LoginServer:: LoginRecvMessage(QString str)
{
    //qDebug()<<str;
    Analysis ana;
    if(ana.AnalysisLoginJson(str))
    {
        if(ana.result==200&&ana.Type=="login")
        {
            if(LoginTimeOut.isActive())
            {
                LoginTimeOut.stop();
            }
            this->hide();
            UserLogoutFlag = false;
            emit ShowMainWidget(ana.UserDetail);
        }
        else if (ana.result==412&&ana.Type=="login")
        {
            ui->textEdit_2->append(ana.LoginData);
        }
        else
        {
            ui->textEdit_2->append(QString::fromLocal8Bit("连接服务器失败"));
        }
    }
    else
    {
        ui->textEdit_2->append("json error");
    }
}

void LoginServer::GetFeaFromServerSucceedSlot(LoginServer::QstringVec vec1, AsfVec vec2)
{
    UserName.assign(vec1.begin(), vec1.end());
    FeatureVec.assign(vec2.begin(), vec2.end());
    ui->textEdit_2->append(QString::fromLocal8Bit("加载人脸库成功"));
}

void LoginServer::GetFeaFromServerFailSlot()
{
    ui->textEdit_2->append(QString::fromLocal8Bit("加载人脸库失败，请先开启后台服务器，然后点击重新加载人脸库进行重新加载"));
}


void LoginServer::on_dockWidget_visibilityChanged(bool visible) //dock关闭或打开
{
        if(visible)
        {
            FaceDectObj->ThreadRunFlag = false;
            ui->MainCam->setPixmap(QPixmap(""));
        }
        else
        {
            FaceDectObj->ThreadRunFlag = true;
            FaceDectObj->start();
        }
}

/*批量注册——begin*/
void LoginServer::on_pushButton_5_clicked()  //批量注册
{
    QString srcDirPath = QFileDialog::getExistingDirectory(
                  this, "choose src Directory",
                   "/");
    if (srcDirPath.isEmpty())
    {
       return;
    }
    else
    {
       qDebug() << "srcDirPath=" << srcDirPath;
       srcDirPath += "/";
    }
    BatchReg = new BatchRegister;
    qRegisterMetaType<ASF_FaceFeature>("AsfFea");
    connect(BatchReg,SIGNAL(PreDetectFaceFail(QString)),this,SLOT(PreDetectFaceFailSlot(QString)));
    connect(BatchReg,SIGNAL(PreExtractFeatureFail(QString)),this,SLOT(PreExtractFeatureFailSlot(QString)));
    connect(BatchReg,SIGNAL(GetFutureSucceed(QString,AsfFea)),this,SLOT(GetFutureSucceedSlot(QString,AsfFea)));
    connect(BatchReg,SIGNAL(NoPicture()),this,SLOT(NoPictureSlot()));
    BatchReg->DirPath = srcDirPath;
    BatchReg->start();
}

void LoginServer::PreDetectFaceFailSlot(QString str)
{
    ui->textEdit->append(str+QString::fromLocal8Bit("不是有效照片"));
}
void LoginServer::PreExtractFeatureFailSlot(QString str)
{
    ui->textEdit->append(str+QString::fromLocal8Bit("提取特征失败"));
}
void LoginServer::GetFutureSucceedSlot(QString str, ASF_FaceFeature fea)
{
    QString FeatureStr,str1;
    for (int i = 0; i < 1032; i++)
    {
        str1 = QString::number(fea.feature[i],16);
        if(str1.length()==1)
        {
            str1="0"+str1;
        }
        FeatureStr.append(str1);
    }
    if(SaveRegister(str,FeatureStr))
    {
        FeatureVec.push_back(fea);
        UserName.push_back(str);
    }
}
void LoginServer::NoPictureSlot()
{
    ui->textEdit->append(QString::fromLocal8Bit("该文件夹中无有效照片"));
}
/*批量注册——end*/


//单个注册
bool LoginServer::get_feature(IplImage* image, ASF_FaceFeature& feature)
{
    ASF_SingleFaceInfo info = { 0 };
    if (ImageEngine.PreDetectFace(image, info, true) == 0)
    {
        if (ImageEngine.PreExtractFeature(image, feature, info) != 0)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void LoginServer::on_pushButton_6_clicked()
{
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.jpeg *.jpg)"));
     if (!fileName.isEmpty())
     {

         QFileInfo fileinfo(fileName);
         ArcFaceEngine Sengine;
         MRESULT res = 1;
         res = Sengine.ActiveSDK((char*)appID,(char*)sdkKey,0);
         res = Sengine.InitEngine(ASF_DETECT_MODE_IMAGE);
          if(res==0)
          {
              QString PicPath;
              PicPath = fileName;
              PicPath.replace("/","\\");
              cv::Mat img_c = cv::imread(PicPath.toLocal8Bit().data());
              IplImage fimg_c = IplImage(img_c);
              ASF_FaceFeature feature_c = { 0 };
              feature_c.featureSize = 1032;
              feature_c.feature = (MByte *)malloc(feature_c.featureSize * sizeof(MByte));//对比完后需要自行释放这一块的内存
              if(get_feature(&fimg_c, feature_c))
              {
                  QString FeatureStr,str;
                  for (int i = 0; i < 1032; i++)
                  {
                      str = QString::number(feature_c.feature[i],16);
                      if(str.length()==1)
                      {
                          str="0"+str;
                      }
                      FeatureStr.append(str);
                  }
                  if(SaveRegister(fileinfo.fileName().left(fileinfo.fileName().indexOf(".")),FeatureStr))
                  {
                      FeatureVec.push_back(feature_c);
                      UserName.push_back(fileinfo.fileName().left(fileinfo.fileName().indexOf(".")));
                  }
                  //free(feature_c.feature);
                  Sengine.UnInitEngine();
              }
              else
              {
                  ui->textEdit_2->append(fileinfo.fileName().left(fileinfo.fileName().indexOf("."))+QString::fromLocal8Bit("注册失败"));
              }
          }
          else
          {
              ui->textEdit_2->append("dll fail");
          }

     }
     else
     {
         return;
     }
}

void LoginServer::closeEvent(QCloseEvent *)
{
    FaceDectObj->ThreadRunFlag = false;
}

void LoginServer::ThisWindowShow()
{
    this->showMaximized();
    ClientSocket->close();
    UserLogoutFlag = true;
    FaceDectObj->ThreadRunFlag = true;
    FaceDectObj->start();
    UpdateFlagTimer.setSingleShot(true);
    UpdateFlagTimer.start(3000);
    connect(&UpdateFlagTimer,SIGNAL(timeout()),this,SLOT(UpdateLogoutFlag()));
}

void LoginServer::UpdateLogoutFlag()
{
    UserLogoutFlag = false;
}

bool LoginServer::SaveRegister(QString name,QString featurestr)
{
    Analysis ana;
    QNetworkAccessManager qnam;
    QString RecvJsonStr;
    QString urlStr = QString("http://%1:%2/mes_server/sop/addFaceInformation").arg(IP).arg(Port);
    QString Param = QString("userName=%1&feature=%2").arg(name).arg(featurestr);
    //qDebug()<<Param;
    QByteArray content = Param.toUtf8();

    const QUrl aurl(urlStr);
    QNetworkRequest qnr( aurl );
   // QNetworkReply *reply = qnam.get( qnr );
    qnr.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    QNetworkReply *reply = qnam.post( qnr, content);


    QEventLoop eventloop;
    connect( reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec( QEventLoop::ExcludeUserInputEvents);
    RecvJsonStr = reply->readAll();
    ana.AnalysisHttpRecv(RecvJsonStr);
    if(ana.result==400)
    {
        ui->textEdit->append(QString::fromLocal8Bit("用户名和人脸特征不能为空"));
        return false;
    }
    else if(ana.result==412)
    {
        ui->textEdit->append(QString::fromLocal8Bit("该用户已注册"));
        return false;
    }
    else if(ana.result==200)
    {
         ui->textEdit->append(name+QString::fromLocal8Bit("注册成功"));
         return true;
    }
    else
    {
        ui->textEdit->append(name+QString::fromLocal8Bit("注册失败"));
        return false;
    }
}


void LoginServer::on_pushButton_7_clicked()
{
    ui->textEdit_2->append(QString::fromLocal8Bit("正在加载人脸库..."));
    FeatureFromServer->start();
}
