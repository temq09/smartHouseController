#include "dialog_addcamera.h"
#include "ui_dialog_addcamera.h"

Dialog_addCamera::Dialog_addCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_addCamera)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(HandleEnterData()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

Dialog_addCamera::~Dialog_addCamera()
{
    qDebug() << "Called the destructor of the dialog_addcamera class ";
    delete ui;
}

void Dialog_addCamera::HandleEnterData()
{
    QString ip_adress = ui->le_ipadress->text();
    QString login = ui->le_login->text();
    QString password = ui->le_password->text();
    int port = -1;
    port = ui->le_port->text().toInt();


    if(port == -1)
    {
        qDebug() << "Port is bad";
        QMessageBox mbox;
        mbox.setText("Port is incorrect");
        mbox.exec();
        return;
    }

    if(login.isEmpty())
    {
        QMessageBox mbox;
        mbox.setText("Port is incorrect");
        mbox.exec();
        return;
    }
    emit EnterDataAreSuccessful(ip_adress,QString::number(port) , login, password);
    close();
}

void Dialog_addCamera::setDataAboutCamera(QString _ip, QString _port, QString _login, QString _password)
{
    ui->le_ipadress->setText(_ip);
    ui->le_port->setText(_port);
    ui->le_login->setText(_login);
    ui->le_password->setText(_password);
}
