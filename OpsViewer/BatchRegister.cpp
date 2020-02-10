#include "BatchRegister.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

#define FACE_FEATURE_SIZE 1032
#define appID "HEAZoCjLjBZzCbv84N4s4dJz7g93TyKujSRp6u5aywSZ"
#define sdkKey "GpEGXY2VX9ThyKufds3ohw3bhS5n196ewrwYrqvUbLXV"

BatchRegister::BatchRegister()
{

}

BatchRegister::~BatchRegister()
{

}

void BatchRegister::run()
{
    if(!FindFile())
    {
        emit NoPicture();
        return;
    }
    res = Sengine.ActiveSDK((char*)appID,(char*)sdkKey,0);
    res = Sengine.InitEngine(ASF_DETECT_MODE_IMAGE);

     if(res==0)
     {
         //qDebug()<<1;
         for(int i = 0; i<FullPath.size();i++)
         {
             QString PicPath;
             PicPath = FullPath[i];
             PicPath.replace("/","\\");
             //IplImage* Iplimg = cvLoadImage(PicPath.toLocal8Bit().data());
             cv::Mat img_c = cv::imread(PicPath.toLocal8Bit().data());
             IplImage fimg_c = IplImage(img_c);
             ASF_FaceFeature feature = { 0 };
             feature.featureSize = FACE_FEATURE_SIZE;
             feature.feature = (MByte *)malloc(feature.featureSize * sizeof(MByte));//对比完后需要自行释放这一块的内存
             ASF_SingleFaceInfo info = { 0 };
             if(get_feature(&fimg_c, feature))
             {
                 emit GetFutureSucceed(FileName[i],feature);
             }
             else
             {
                emit PreExtractFeatureFail(FileName[i]);
             }
         }
     }
     Sengine.UnInitEngine();
}


bool BatchRegister::FindFile()
{
    QString path = DirPath;
    QDir dir(path);
    if (!dir.exists())
    {
        return false;
    }
    dir.setFilter(QDir::Dirs | QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    do
    {

        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName() == "." | fileInfo.fileName() == "..")
        {
            i++;
            continue;
        }
        bool bisDir = fileInfo.isDir();
        if (bisDir)
        {
            //FindFile(fileInfo.filePath());
        }
        else
        {
            FullPath.push_back(fileInfo.path()+"/"+fileInfo.fileName()) ;
            FileName.push_back(fileInfo.fileName().left(fileInfo.fileName().indexOf(".")));
        }
        i++;
    } while (i < list.size());
    if(FullPath.size()>0)
        return true;
    else
        return false;
}

bool BatchRegister::get_feature(IplImage* image, ASF_FaceFeature& feature)
{
    ASF_SingleFaceInfo info = { 0 };
    if (Sengine.PreDetectFace(image, info, true) == 0)
    {
        if (Sengine.PreExtractFeature(image, feature, info) != 0)
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

void BatchRegister::GetFeature(IplImage* image, ASF_FaceFeature& feature) //提取特征
{
    //检测脸
//    ASF_SingleFaceInfo info = { 0 };
//    if (Sengine.PreDetectFace(image, info, true) == 0) {
//        qDebug()<<"PreDetectFace";

//    }

//    if (Sengine.PreExtractFeature(image, feature, info) == 0) {
//        qDebug()<<"PreExtractFeature";
//        unsigned char *data = (unsigned char*)feature.feature;
//        for (int i = 0; i < feature.featureSize; i++)
//        {
//            qDebug()<<data[i];
//        }
//    }
//    else {

//    }
}
