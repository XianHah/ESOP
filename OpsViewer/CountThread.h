#ifndef COUNTTHREAD_H
#define COUNTTHREAD_H
#include <QThread>
#include "counter.h"

class CountThread : public QThread
{
    Q_OBJECT
public:
    CountThread();
    void run();
    bool ThreadRunFlag = false;
signals:
    void SendNum(int);
    void OpenCamFail(QString);
    void OpenCamSucceed();
};

void cb_lh();
void cb_hl();
#endif // COUNTTHREAD_H
