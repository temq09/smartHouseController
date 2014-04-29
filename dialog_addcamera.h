#ifndef DIALOG_ADDCAMERA_H
#define DIALOG_ADDCAMERA_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Dialog_addCamera;
}

class Dialog_addCamera : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_addCamera(QWidget *parent = 0);
    ~Dialog_addCamera();
    void setDataAboutCamera(QString _ip, QString _port, QString _login, QString _password);

private:
    Ui::Dialog_addCamera *ui;
    QString ipAdress;
    QString port;
    QString login;
    QString password;

private slots:
    void HandleEnterData();

signals:
    void EnterDataAreSuccessful(QString IPadres, QString Port, QString Login, QString Password);
};

#endif // DIALOG_ADDCAMERA_H
