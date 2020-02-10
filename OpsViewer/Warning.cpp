#include "Warning.h"
#include "ui_Warning.h"

bool RedFlag = false;
Warning::Warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warning)
{
    ui->setupUi(this);
    connect(&WarnTimer,SIGNAL(timeout()),this,SLOT(ChangeBGColor()));
    WarnTimer.start(2000);
    ui->widget->setStyleSheet("border-image: url(:/image/confirm.png);");
}

Warning::~Warning()
{
    delete ui;
}

void Warning::on_pushButton_clicked()
{
    WarnTimer.stop();
    accept();
}

void Warning::ChangeBGColor()
{
    if(RedFlag)
    {
        ui->widget_2->setStyleSheet("background-color:red;");
        RedFlag = false;
    }
    else
    {
        ui->widget_2->setStyleSheet(";");
        RedFlag = true;
    }
}

void Warning::closeEvent(QCloseEvent *)
{
    WarnTimer.stop();
}
