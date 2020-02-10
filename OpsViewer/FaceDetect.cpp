#include "FaceDetect.h"
#include <QDebug>
extern vector<ASF_FaceFeature>FeatureVec;
extern vector<QString>UserName;
extern IplImage  *frame;
extern ASF_SingleFaceInfo faceInfo;
extern mutex g_mutex;
#define appID "HEAZoCjLjBZzCbv84N4s4dJz7g93TyKujSRp6u5aywSZ"
#define sdkKey "GpEGXY2VX9ThyKufds3ohw3bhS5n196ewrwYrqvUbLXV"
extern QString RGBThreshold ,IRThreshold ,CompaerThreshold,CamNum;
extern ArcFaceEngine VideoEngine;
extern ArcFaceEngine ImageEngine;
extern bool UserLogoutFlag;
FaceDetect::FaceDetect()
{

}

void FaceDetect::run()
{
    ArcFaceEngine m_imageFaceEngine;
    MRESULT faceRes = m_imageFaceEngine.ActiveSDK((char*)appID, (char*)sdkKey, 0);
    faceRes = m_imageFaceEngine.InitEngine(ASF_DETECT_MODE_IMAGE);//Image
    faceRes = m_imageFaceEngine.InitEngine(ASF_DETECT_MODE_IMAGE);//Image
    /******************************************************************/
        cv::VideoCapture cap(CamNum.toInt());
        if (!cap.isOpened())
        {
            qDebug()<<"can not open";
        }
        cv::Mat f;

//        cv::Mat img_c = cv::imread("F:\\QTProject\\OpsViewer\\image\\timg.jpg");
//        IplImage fimg_c = IplImage(img_c);
//        ASF_FaceFeature feature_c = { 0 };
//            feature_c.featureSize = 1032;
//            feature_c.feature = (MByte *)malloc(feature_c.featureSize * sizeof(MByte));//对比完后需要自行释放这一块的内存
//        get_feature(&fimg_c, feature_c);

        ASF_FaceFeature feature_f_t = { 0 };
        feature_f_t.featureSize = 1032;
        feature_f_t.feature = (MByte *)malloc(feature_f_t.featureSize * sizeof(MByte));//对比完后需要自行释放这一块的内存

        while (ThreadRunFlag)
        {
            if (cap.read(f))
            {
                IplImage fimg = IplImage(f);
                IplImage fimg1 = IplImage(f);
                ASF_SingleFaceInfo info = { 0 };
                ASF_SingleFaceInfo info1 = { 0 };
                if (m_imageFaceEngine.PreDetectFace(&fimg, info, true) != 0)
                {
                    //发射信号，在主页面绘制图片
                    emit SendImg(&fimg1);
                    continue;
                }
                info1 = info;
                cvRectangle(&fimg1, cvPoint(info1.faceRect.left, info1.faceRect.top),
                    cvPoint(info1.faceRect.right, info1.faceRect.bottom), cvScalar(0, 0, 255), 2);
                //发射信号，在主页面绘制图片
                emit SendImg(&fimg1);
                if (m_imageFaceEngine.PreExtractFeature(&fimg, feature_f_t, info) != 0)
                {
                    continue;
                }
                if(!UserLogoutFlag)
                {
                    for(int i = 0;i<(int)FeatureVec.size();i++)
                    {
                        if(!ThreadRunFlag)
                        {
                            break;
                        }
                        MFloat confidenceLevel = 0;
                        MRESULT pairRes = 1;
                        pairRes = m_imageFaceEngine.FacePairMatching(confidenceLevel,feature_f_t, FeatureVec[i]);
                        if (pairRes == 0)
                        {
                            if(confidenceLevel>=CompaerThreshold.toFloat())
                            {
                                emit CompareSucceed(UserName[i]);
                                m_imageFaceEngine.UnInitEngine();
                                return;
                            }

                        }
                        else
                        {
                           continue;
                        }
                    }
                }

             }
        }
        m_imageFaceEngine.UnInitEngine();
}

void FaceDetect::get_feature(IplImage* image, ASF_FaceFeature& feature)
{

    //检测脸
    ASF_SingleFaceInfo info = { 0 };
    ASF_SingleFaceInfo info_1 = { 0 };
    if (ImageEngine.PreDetectFace(image, info, true) == 0)
    {
        if (ImageEngine.PreExtractFeature(image, feature, info) == 0)
        {
            //以hex输出得到的特征码前10位
            std::cout << "人脸特征信息前10位：";
            unsigned char *data = (unsigned char*)feature.feature;
            for (int i = 0; i < 10; i++)
            {
                std::cout << std::hex << (int)data[i];
            }
            std::cout << std::dec << std::endl;
        }
    }
    else
    {
        //失败 找不到脸
        std::cout << "face feature failed" << std::endl;
    }
}


