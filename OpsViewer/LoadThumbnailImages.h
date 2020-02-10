#ifndef LOADTHUMBNAILIMAGES_H
#define LOADTHUMBNAILIMAGES_H
#include <QThread>
#include <vector>
#include <QString>
#include "inc/free/arcsoft_face_sdk.h"
#include "ArcFaceEngine.h"
#include <comdef.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")
using namespace std;
class LoadThumbnailImages : public QThread
{
    Q_OBJECT
public:
    LoadThumbnailImages();
    ~LoadThumbnailImages();
    void run();
    vector<QString>FilePath;
    vector<QString>FileName;
    bool FindFile(const QString &path);
    ArcFaceEngine m_imageFaceEngine;
    char appID[MAX_PATH] = "HEAZoCjLjBZzCbv84N4s4dJz7g93TyKujSRp6u5aywSZ";
    char  sdkKey[MAX_PATH] = "GpEGXY2VX9ThyKufds3ohw3bhS5n196ewrwYrqvUbLXV";
    char  activeKey[MAX_PATH] = "";
    BOOL IsImageGDIPLUSValid(QString filePath);
    void get_feature(IplImage *image, ASF_FaceFeature &feature);
signals:
    void SetFeature(ASF_FaceFeature);
};

#endif // LOADTHUMBNAILIMAGES_H
