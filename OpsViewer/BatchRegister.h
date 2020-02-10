#ifndef BATCHREGISTER_H
#define BATCHREGISTER_H
#include <QThread>
#include <vector>
#include "opencv2/opencv.hpp"
#include "ArcFaceEngine.h"
using namespace cv;
using namespace std;

class BatchRegister : public QThread
{
    Q_OBJECT

public:
    BatchRegister();
    ~BatchRegister();
    void run();
    QString DirPath;
    vector<QString>FullPath;
    vector<QString>FileName;
    bool FindFile();
    void GetFeature(IplImage *image, ASF_FaceFeature &feature);
    ArcFaceEngine Sengine;
    MRESULT res = 1;
    typedef ASF_FaceFeature AsfFea;
    bool get_feature(IplImage *image, ASF_FaceFeature &feature);
signals:
    void PreDetectFaceFail(QString);
    void PreExtractFeatureFail(QString);
    void GetFutureSucceed(QString,AsfFea);
    void NoPicture();
};

#endif // BATCHREGISTER_H
