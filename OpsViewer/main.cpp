﻿#include "OpsViewer.h"
#include <QApplication>
#include "LoginServer.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginServer Login;
    OpsViewer w;
    QObject::connect(&Login,SIGNAL(ShowMainWidget(QString)),&w,SLOT(ThisShow(QString)));
    QObject::connect(&w,SIGNAL(ShowLoginDlg()),&Login,SLOT(ThisWindowShow()));
    Login.show();   
    //w.show();
    return  a.exec();
}
