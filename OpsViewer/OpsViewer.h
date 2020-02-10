#ifndef OPSVIEWER_H
#define OPSVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QWebSocket>
#include <QtNetwork>
#include <vector>
#include "CountThread.h"

using namespace std;

namespace Ui {
class OpsViewer;
}

class OpsViewer : public QWidget
{
    Q_OBJECT

public:
    explicit OpsViewer(QWidget *parent = 0);
    ~OpsViewer();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    //message
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);
protected slots:
    void RecvMessage(QString message);  
    void ChangePic();
    void GetShowMessageSlot();
    void ChangNotice();
private:
    Ui::OpsViewer *ui;

private:
    QLabel *ShowWhatThis;
    QSize OldSize,NewSize;
    QPoint OldPoint,NewPoint;

private:
    QUrl url;
    vector<QString>FileNameVec;
    vector<QString>NoticeShowVec;
    vector<int>NoticeStartTimerID;
    vector<int>NoticeEndTimerID;
    int PicIndex = 0;
    int NoticeIndex = 0;
    QTimer ChangePicTimer;
    QTimer ChangeNoticeTimer;
    QTimer ScollTimer;
    QTimer ConfirmTimer;
    QTimer SendCountTimer;
    QTimer AllWidgetHideTimer;
    bool TimerRunning = false;
    bool IsLastClick = false;
    int SwitchTime;
    int ConfirmTime;
    int JsonId;
    int FileNameVecSize;
    int CountNum = 0;
    int SendCountTime;
    int Productiveapacity = 0;
    int TempCountNum = 0;

    QString StartCountTime;

    vector<QString> PictureUrl;
    vector<QString> NoticeContent;
    vector<QString> HistoryNoticeContent;
    vector<QString> NoticeStartTime;
    vector<QString> NoticeEndTime;

    CountThread RunCounter;


public:
    void UrlRequestGet(const QUrl url);  

public slots:
    void ConfirmSlot();
    void SendConfirmJsonSlot();
    void GetCountNum(int);
    void SendCountNumToServer();
    void OpenCamFailSlot(QString);
    void OpenCamSucceedlSlot();
    void AllWidgetHideSlot();
signals:
    void GetShowMessage();
    void ShowLoginDlg();
    void SendConfirmJson();
protected slots:
    void ThisShow();
    void ScollPlayNotice();
};

#endif // OPSVIEWER_H
