#include "analysis.h"
#include <QDateTime>

//QByteArray allData = QByteArray("{\"id\":123,\"direction\":\"request\", \"type\":\"show\",\"body\": {\"switchInterval\":100,\"picture\":[ {\"path\":\"www.baidu.com\" },{  \"path\":\"www.baidu.com\" },{\"path\":\"www.baidu.com\" }  ],  \"notice\":[ {\"title\":\"标题1\",\"content\":\"内容1\", \"startTime\":\"开始播放时间\",\"endTime\":\"结束播放时间\" }, { \"title\":\"标题2\",\"content\":\"内容2\", \"startTime\":\"开始播放时间\", \"endTime\":\"结束播放时间\"}, {\"title\":\"标题3\", \"content\":\"内容3\", \"startTime\":\"开始播放时间\", \"endTime\":\"结束播放时间\" }  ] } }");
//QByteArray allData = QByteArray("{\"id\":1, \"direction\":\"request\", \"type\":\"login\", \"body\": { \"mac\":\"123456\", \"userName\":\"姓名\", \"time\":\"2019-09-01 12:12:12\" } }");
//QByteArray allData = QByteArray("{ \"id\":345, \"direction\":\"request\", \"type\":\"cancelShow\", \"body\": {} }");
//
Analysis::Analysis()
{
}

bool Analysis::AnalysisReceiveDisplayInformation(QString JsonData)
{
  QByteArray allData = QByteArray(JsonData.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return false;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();


                if(rootObj.contains("id"))
                {
                    QJsonValue value = rootObj.value("id");
                    Id = value.toInt();
//                    qDebug()<< value.toInt();
                }
                if(rootObj.contains("direction"))
                {
                    QJsonValue value = rootObj.value("direction");
                    Direction = value.toString();
//                    qDebug()<< value.toString();
                }
                if(rootObj.contains("type"))
                {
                    QJsonValue value = rootObj.value("type");
                    Type = value.toString();
//                    qDebug()<< value.toString();
                }
                if(rootObj.contains("body"))
                {
                    QJsonValue bodyValue = rootObj.value("body");
                    QJsonObject body = bodyValue.toObject();
//                    qDebug()<< bodyValue.toObject();
                    if(body.contains("switchInterval"))
                    {
                        QJsonValue value = body.value("switchInterval");
                        SwitchInterval = value.toInt();
//                        qDebug()<< value.toInt();
                    }
                    if(body.contains("timeLength"))
                    {
                        QJsonValue value = body.value("timeLength");
                        Confirminterval = value.toInt();
                    }
                    if(body.contains("picture"))
                    {

                        QJsonValue valueArray  = body.value("picture");
                        if(valueArray.isArray())
                        {
                            QJsonArray jsonArray = valueArray.toArray();
                            for(int i =0; i< jsonArray.count(); i++)
                            {
                                QJsonValue PictureValue = jsonArray[i];
//                                    qDebug()<<PictureValue;
                                if(PictureValue.isObject())
                                {
                                    QJsonObject pathObj = PictureValue.toObject();
//                                    qDebug()<<pathObj;
                                    if(pathObj.contains("path"))
                                    {
                                        QJsonValue value = pathObj.value("path");
//                                        qDebug()<< value.toString();
                                        QlistPicturePath.push_back(value.toString());
                                    }
                                }
                            }

                        }
                    }

                    if(body.contains("notice"))
                    {
                        QJsonValue valueArray  = body.value("notice");
                        if(valueArray.isArray())
                        {
                            QJsonArray jsonArray = valueArray.toArray();
                            for(int i =0; i< jsonArray.count(); i++)
                            {
                                QJsonValue PictureValue = jsonArray[i];

                                if(PictureValue.isObject())
                                {
                                    QJsonObject noticeObj = PictureValue.toObject();

                                    if(noticeObj.contains("title"))
                                    {
                                        QJsonValue value = noticeObj.value("title");
    //                                    qDebug()<< value.toString();
                                        QlistNoticeTitle.push_back(value.toString());
                                    }
                                    if(noticeObj.contains("content"))
                                    {
                                        QJsonValue value = noticeObj.value("content");
    //                                    qDebug()<< value.toString();
                                        QlistNoticeContent.push_back(value.toString());
                                    }
                                    if(noticeObj.contains("startTime"))
                                    {
                                        QJsonValue value = noticeObj.value("startTime");
    //                                    qDebug()<< value.toString();
                                        QlistNoticeStartTime.push_back(value.toString());
                                    }
                                    if(noticeObj.contains("endTime"))
                                    {
                                        QJsonValue value = noticeObj.value("endTime");
    //                                    qDebug()<< value.toString();
                                        QlistNoticeEndTime.push_back(value.toString());
                                    }
                                }
                            }

                        }
                    }
                }
                return true;
           }
           return false;
       }
}

bool Analysis::AnalysisReceiveInformation(QString JsonData)
{
  QByteArray allData = QByteArray(JsonData.toUtf8());
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return false;
    }
    else
    {
        if(jsonDoc.isObject())
        {
            QJsonObject rootObj = jsonDoc.object();
            if(rootObj.contains("id"))
            {
                QJsonValue value = rootObj.value("id");
                Id = value.toInt();
//                qDebug()<< value.toInt();
            }
            if(rootObj.contains("direction"))
            {
                QJsonValue value = rootObj.value("direction");
                Direction = value.toString();
//                qDebug()<< value.toString();
            }
            if(rootObj.contains("type"))
            {
                QJsonValue value = rootObj.value("type");
                Type = value.toString();
//                qDebug()<< value.toString();
            }
            if(rootObj.contains("body"))
            {
                QJsonValue bodyValue = rootObj.value("body");
                QJsonObject body = bodyValue.toObject();
//                qDebug()<< bodyValue.toObject();
                if(body.contains("switchInterval"))
                {
                    QJsonValue value = body.value("switchInterval");
                    SwitchInterval = value.toInt();
//                    qDebug()<< value.toInt();
                }
                if(body.contains("mac"))
                {
                    QJsonValue value = body.value("mac");
                    Mac = value.toString();
//                    qDebug()<< value.toString();
                }
                if(body.contains("userName"))
                {
                    QJsonValue value = body.value("userName");
                    UserName = value.toString();
//                    qDebug()<< value.toString();
                }
                if(body.contains("time"))
                {
                    QJsonValue value = body.value("time");
                    time = value.toString();
//                    qDebug()<< value.toString();
                }
                if(body.contains("picture"))
                {

                    QJsonValue valueArray  = body.value("picture");
//                    qDebug()<< valueArray;
                    if(valueArray.isArray())
                    {
                        QJsonArray jsonArray = valueArray.toArray();
                        for(int i =0; i< jsonArray.count(); i++)
                        {
                            QJsonValue PictureValue = jsonArray[i];
//                            qDebug()<<PictureValue;
                            if(PictureValue.isObject())
                            {
                                QJsonObject pathObj = PictureValue.toObject();
//                                qDebug()<<pathObj;
                                if(pathObj.contains("path"))
                                {
                                    QJsonValue value = pathObj.value("path");
//                                    qDebug()<< value.toString();
                                    QlistPicturePath.push_back(value.toString());
                                }
                            }
                        }

                    }
                }

                if(body.contains("notice"))
                {
                    QJsonValue valueArray  = body.value("notice");
                    if(valueArray.isArray())
                    {
                        QJsonArray jsonArray = valueArray.toArray();
                        for(int i =0; i< jsonArray.count(); i++)
                        {
                            QJsonValue NoticeValue = jsonArray[i];

                            if(NoticeValue.isObject())
                            {
                                QJsonObject noticeObj = NoticeValue.toObject();

                                if(noticeObj.contains("title"))
                                {
                                    QJsonValue value = noticeObj.value("title");
//                                    qDebug()<< value.toString();
                                    QlistNoticeTitle.push_back(value.toString());
                                }
                                if(noticeObj.contains("content"))
                                {
                                    QJsonValue value = noticeObj.value("content");
//                                    qDebug()<< value.toString();
                                    QlistNoticeContent.push_back(value.toString());
                                }
                                if(noticeObj.contains("startTime"))
                                {
                                    QJsonValue value = noticeObj.value("startTime");
//                                    qDebug()<< value.toString();
                                    QlistNoticeStartTime.push_back(value.toString());
                                }
                                if(noticeObj.contains("endTime"))
                                {
                                    QJsonValue value = noticeObj.value("endTime");
//                                    qDebug()<< value.toString();
                                    QlistNoticeEndTime.push_back(value.toString());
                                }

                            }
                        }

                    }
                }
            }
            return true;
        }
        return false;
    }
}

bool Analysis::AnalysisReceiveLoginInformation(QString JsonData)
{
    QByteArray allData = QByteArray(JsonData.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return false;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();

                if(rootObj.contains("id"))
                {
                    QJsonValue value = rootObj.value("id");
                    Id = value.toInt();
//                    qDebug()<< value.toInt();
                }
                if(rootObj.contains("direction"))
                {
                    QJsonValue value = rootObj.value("direction");
                    Direction = value.toString();
//                    qDebug()<< value.toString();
                }
                if(rootObj.contains("type"))
                {
                    QJsonValue value = rootObj.value("type");
                    Type = value.toString();
//                    qDebug()<< value.toString();
                }
                if(rootObj.contains("body"))
                {
                    QJsonValue bodyValue = rootObj.value("body");
                    QJsonObject body = bodyValue.toObject();
//                    qDebug()<< bodyValue.toObject();
                    if(body.contains("mac"))
                    {
                        QJsonValue value = body.value("mac");
                        Mac = value.toString();
//                        qDebug()<< value.toString();
                    }
                    if(body.contains("userName"))
                    {
                        QJsonValue value = body.value("userName");
                        UserName = value.toString();
//                        qDebug()<< value.toString();
                    }
                    if(body.contains("time"))
                    {
                        QJsonValue value = body.value("time");
                        time = value.toString();
//                        qDebug()<< value.toString();
                    }

                }
                return true;
           }
           return false;
       }
}

bool Analysis::AnalysisReceiveCancelDisplayInformation(QString JsonData)
{
    QByteArray allData = QByteArray(JsonData.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return false;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();

                if(rootObj.contains("id"))
                {
                    QJsonValue value = rootObj.value("id");
                    Id = value.toInt();
//                    qDebug()<< value.toInt();
                }
                if(rootObj.contains("direction"))
                {
                    QJsonValue value = rootObj.value("direction");
                    Direction = value.toString();
//                    qDebug()<< value.toString();
                }
                if(rootObj.contains("type"))
                {
                    QJsonValue value = rootObj.value("type");
                    Type = value.toString();
//                    qDebug()<< value.toString();
                }
                return true;
           }
           return false;
       }
}

QString Analysis::SendLoginInformationJsonData(QString macaddr,QString name)
{
    // 构建 Json 对象
    QJsonObject bodyObject;
    QString TimeStr;
    TimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    bodyObject.insert("mac", macaddr);
    bodyObject.insert("userName", name);
    bodyObject.insert("time", TimeStr);

    QJsonObject json;
    json.insert("id", 1);
    json.insert("direction", "request");
    json.insert("type", "login");
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    return strJson;
}

QString Analysis::SendDisplayInformationJsonData(bool Result,QString Message,int id )
{
    // 构建 Json 对象 - body
    QJsonObject bodyObject;
    if(Result == true)
    {
        bodyObject.insert("result", 200);
        bodyObject.insert("data", QString::fromLocal8Bit(("展示成功")));
    }
    else
    {
        bodyObject.insert("result", 412);
        bodyObject.insert("data", Message);
    }


    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", id); //123
    json.insert("direction", "response");
    json.insert("type", "show");  //
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    return strJson;
}

QString Analysis::SendCancelDisplayInformationJsonData(bool Result,QString Message,int id,QString type )
{
    // 构建 Json 对象 - body
    QJsonObject bodyObject;
    if(Result == true)
    {
        bodyObject.insert("result", 200);

    }
    else
    {
        bodyObject.insert("result", 412);
    }
    bodyObject.insert("data", Message);

    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", id);
    json.insert("direction", "response");
    json.insert("type", type);
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
//    qDebug() << strJson;

    return strJson;
}

void Analysis::ValueClear()
{
    Id = 0;
    Direction="";
    Type="";
    SwitchInterval= 0;
    Mac ="";
    UserName="";
    time="";
    QlistPicturePath.clear();//图片地址链表
    QlistNoticeTitle.clear();//标题链表
    QlistNoticeContent.clear();//内容链表
    QlistNoticeStartTime.clear();//开始时间链表
    QlistNoticeEndTime.clear();//结束时间链表
}

void Analysis::AnalysisHttpRecv(QString jsonstr)
{
    QByteArray allData = QByteArray(jsonstr.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return ;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();

                if(rootObj.contains("result"))
                {
                    QJsonValue value = rootObj.value("result");
                    result = value.toInt();
                }
                else
                {
                    result = 0;
                }
           }
       }
}

bool Analysis::AnalysisFaceFeature(QString jsonstr)
{
      QByteArray allData = QByteArray(jsonstr.toUtf8());
      QJsonParseError json_error;
         QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

         if(json_error.error != QJsonParseError::NoError)
         {
             return false;
         }
         else
         {
             if(jsonDoc.isObject())
             {
                  QJsonObject rootObj = jsonDoc.object();
                  if(rootObj.contains("data"))
                  {

                      QJsonValue valueArray  = rootObj.value("data");
                      if(valueArray.isArray())
                      {
                          QJsonArray jsonArray = valueArray.toArray();
                          for(int i =0; i< jsonArray.count(); i++)
                          {
                              QJsonValue PictureValue = jsonArray[i];
                              if(PictureValue.isObject())
                              {
                                  ASF_FaceFeature fea;
                                  fea.featureSize = 1032;
                                  fea.feature = (MByte *)malloc(fea.featureSize * sizeof(MByte));
                                  QJsonObject pathObj = PictureValue.toObject();
                                  QJsonValue value = pathObj.value("feature");
                                  CovertQstring2Feature(value.toString(),fea);
                                  FaceFeaVec.push_back(fea);
                                  value = pathObj.value("userName");
                                  UserNameVec.push_back(value.toString());
                               }
                            }
                            return true;
                          }
                      else
                      {
                        return false;
                      }

                      }
                  else {
                      return false;
                  }
            }
            else {
                return false;
             }

             }
}

void Analysis::CovertQstring2Feature(QString FeatureStr,ASF_FaceFeature& feature) //转换qstring为feature
{
    int FeatureIndex = 0;
    bool ok;
    for(int i = 0;i<FeatureStr.length();i+=2)
    {
        feature.feature[FeatureIndex] = FeatureStr.mid(i,2).toInt(&ok,16);
        FeatureIndex++;
    }
}

void Analysis::GetJsonID(QString jsonstr)
{
    QByteArray allData = QByteArray(jsonstr.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return ;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();

                if(rootObj.contains("type"))
                {
                    QJsonValue value = rootObj.value("type");
                    Type = value.toString();
                }
                else
                {
                    Type = "";
                }
                if(rootObj.contains("id"))
                {
                    QJsonValue value = rootObj.value("id");
                    Id = value.toInt();
                }
                else
                {
                    Id = -1;
                }
           }
       }
}

bool Analysis::AnalysisLoginJson(QString jsonstr)
{
    QByteArray allData = QByteArray(jsonstr.toUtf8());
    QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return false;
       }
       else
       {
           if(jsonDoc.isObject())
           {
                QJsonObject rootObj = jsonDoc.object();

                if(rootObj.contains("body"))
                {
                    QJsonValue bodyValue = rootObj.value("body");
                    QJsonObject body = bodyValue.toObject();
                    QJsonValue value = body.value("result");
                    result = value.toInt();
                    value = body.value("data");
                    LoginData = value.toString();

                }
                if(rootObj.contains("type"))
                {
                    QJsonValue value = rootObj.value("type");
                    Type = value.toString();

                }
                else
                {
                    result = 0;
                    return false;
                }
           }
           else
           {
                return false;
           }
       }
    return true;
}

QString Analysis::SendLogoutJson(QString name)
{
    // 构建 Json 对象 - body
    QString TimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QJsonObject bodyObject;
    bodyObject.insert("userName", name);
    bodyObject.insert("time", TimeStr);


    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", 567);
    json.insert("direction", "request");
    json.insert("type", "logout");
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

QString Analysis::SendConfirmJson(QString name,bool IsTimerOut)
{
    // 构建 Json 对象 - body
    QString TimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QJsonObject bodyObject;
    bodyObject.insert("userName", name);
    bodyObject.insert("time", TimeStr);
    if(!IsTimerOut)
        bodyObject.insert("result", "succeed");
    else
        bodyObject.insert("result", "time_out");


    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", 1);
    json.insert("direction", "request");
    json.insert("type", "confirm");
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

QString Analysis::SendCountNumJson(QString name,int num,QString StartTime)
{
    // 构建 Json 对象 - body
    QString TimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QJsonObject bodyObject;
    bodyObject.insert("userName", name);
    bodyObject.insert("startTime", StartTime);
    bodyObject.insert("endTime", TimeStr);
    bodyObject.insert("number", num);


    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", 1);
    json.insert("direction", "request");
    json.insert("type", "count");
    json.insert("body", QJsonValue(bodyObject));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}















