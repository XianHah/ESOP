#ifndef WARNING_H
#define WARNING_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Warning;
}

class Warning : public QDialog
{
    Q_OBJECT

public:
    explicit Warning(QWidget *parent = 0);
    ~Warning();
    QTimer WarnTimer;

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_clicked();
    void ChangeBGColor();

private:
    Ui::Warning *ui;
};

#endif // WARNING_H
