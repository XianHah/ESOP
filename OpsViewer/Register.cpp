#include "Register.h"
#include "ui_register.h"
#include <QMessageBox>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_2_clicked()
{
    reject();
}

void Register::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="bobo")
        accept();
    else
        QMessageBox::warning(this,"warning","Password error");
}
