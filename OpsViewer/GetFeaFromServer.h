#ifndef GETFEAFROMSERVER_H
#define GETFEAFROMSERVER_H
#include <QThread>
#include <vector>
#include <arcsoft_face_sdk.h>
using namespace std;

class GetFeaFromServer : public QThread
{
    Q_OBJECT
public:
    GetFeaFromServer();
    void run();
    typedef vector<QString> QstringVec;
    typedef vector<ASF_FaceFeature> AsfVec;
signals:
    void GetFeaFromServerSucceed(QstringVec vec1,AsfVec vec2);
    void GetFeaFromServerFail();
};

#endif // GETFEAFROMSERVER_H
