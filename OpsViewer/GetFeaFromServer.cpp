#include "GetFeaFromServer.h"
#include "analysis.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

extern QString IP;
extern QString Port;
GetFeaFromServer::GetFeaFromServer()
{

}

void GetFeaFromServer::run()
{
    Analysis ana;
    QNetworkAccessManager qnam;
    QString RecvJsonStr;
    QString urlStr = QString("http://%1:%2/mes_server/sop/getFaceInformation").arg(IP).arg(Port);
    const QUrl aurl(urlStr);
    QNetworkRequest qnr( aurl );
    QNetworkReply *reply = qnam.get( qnr );
    QEventLoop eventloop;
    connect( reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec( QEventLoop::ExcludeUserInputEvents);
    RecvJsonStr = reply->readAll();
    if(ana.AnalysisFaceFeature(RecvJsonStr))
    {
         emit GetFeaFromServerSucceed(ana.UserNameVec,ana.FaceFeaVec);
    }
    else
    {
        emit GetFeaFromServerFail();
    }
}
