#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QDialog>
#include <vector>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <QTimer>
#include "BatchRegister.h"
#include "FaceDetect.h"
#include"GetFeaFromServer.h"
#include <QMetaType>
using namespace std;
using namespace cv;

namespace Ui {
class LoginServer;
}

class LoginServer : public QDialog
{
    Q_OBJECT

public:
    explicit LoginServer(QWidget *parent = 0);
    ~LoginServer();

    QString getHostMacAddress();

    void OpenCam();
    void CloseCam();

    typedef vector<QString> QstringVec;
    typedef vector<ASF_FaceFeature> AsfVec;
    typedef ASF_FaceFeature AsfFea;
    cv::Mat rgbFrame;
    cv::VideoCapture rgbCapture;
    cv::VideoCapture cap;
    cv::Mat f;

public slots:
    void CompareSucceed(QString str);
    void GetFeaFromServerSucceedSlot(QstringVec vec1, AsfVec vec2);
    void GetFeaFromServerFailSlot();
    void LoginRecvMessage(QString str);
    void LoginTimeOutSlot();
protected slots:
    void readFarme(IplImage *iframe);
    void PreExtractFeatureFailSlot(QString str);
    void PreDetectFaceFailSlot(QString str);
    void GetFutureSucceedSlot(QString str,AsfFea);
    void NoPictureSlot();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_dockWidget_visibilityChanged(bool visible);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::LoginServer *ui;


private:
    QImage    *imag;
    CvCapture *cam;  //视频获取结构， 用来作为视频获取函数的一个参数
    QTimer GetFrameTimer;
    QTimer UpdateFlagTimer;
    QTimer LoginTimeOut;
    BatchRegister *BatchReg;
public:


    bool SaveRegister(QString name, QString featurestr);
    bool get_feature(IplImage *image, ASF_FaceFeature &feature);
signals:
    void SendIplImage(IplImage  *frame,ASF_SingleFaceInfo FaceInfo);
    void ShowMainWidget();
protected:
    void closeEvent(QCloseEvent *);

protected slots:
    void ThisWindowShow();
    void SendLoginJson();
    void UpdateLogoutFlag();
};

#endif // LOGINSERVER_H
