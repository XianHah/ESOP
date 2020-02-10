#ifndef FACEDETECT_H
#define FACEDETECT_H
#include <QThread>
#include "opencv2/opencv.hpp"
#include "ArcFaceEngine.h"
#include <vector>
#include "LoginServer.h"
#include "arcsoft_face_sdk.h"
using namespace std;
using namespace cv;
class FaceDetect : public QThread
{
    Q_OBJECT
public:
    FaceDetect();
    void run();
    bool ThreadRunFlag = false;
    void get_feature(IplImage* image, ASF_FaceFeature& feature) ;
signals:
    void CompareSucceed(QString);
    void SendImg(IplImage* image);
};

#endif // FACEDETECT_H
