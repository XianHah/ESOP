#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QList>
#include <vector>
#include "arcsoft_face_sdk.h"
using namespace std;

class Analysis
{
public:
    Analysis();
    bool AnalysisReceiveInformation(QString JsonData);//解析接收的信息
    bool AnalysisReceiveLoginInformation(QString JsonData);//解析登录信息
    bool AnalysisReceiveDisplayInformation(QString JsonData);//解析接收的显示信息
    bool AnalysisReceiveCancelDisplayInformation(QString JsonData);//解析取消显示信息

    QString SendDisplayInformationJsonData(bool Result, QString Message, int id);//发送显示信息
    QString SendCancelDisplayInformationJsonData(bool Result, QString Message, int id, QString type);//发送取消显示信息

    void ValueClear();//清空
    void AnalysisHttpRecv(QString jsonstr);
    bool AnalysisFaceFeature(QString jsonstr);
    void CovertQstring2Feature(QString FeatureStr, ASF_FaceFeature &feature);
    void GetJsonID(QString jsonstr);
public:
    int Id;
    int result;
    QString Direction;
    QString Type;
    int SwitchInterval;
    int Confirminterval;
    QString LoginData;
    QString Mac;
    QString UserName;
    QString time;
    QString UserDetail;
    vector<QString> QlistPicturePath;//图片地址链表
    vector<QString> QlistNoticeTitle;//标题链表
    vector<QString> QlistNoticeContent;//内容链表
    vector<QString> QlistNoticeStartTime;//开始时间链表
    vector<QString> QlistNoticeEndTime;//结束时间链表
public:
    vector<ASF_FaceFeature> FaceFeaVec;
    vector<QString> UserNameVec;

    QString SendLoginInformationJsonData(QString macaddr, QString name);
    bool AnalysisLoginJson(QString jsonstr);
    QString SendLogoutJson(QString name);
    QString SendConfirmJson(QString name, bool IsTimerOut);
    QString SendCountNumJson(QString name, int num, QString StartTime);
};

#endif // ANALYSIS_H
