#include "relaycontroller.h"
#include "ui_relaycontroller.h"

relayController::relayController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::relayController)
{
    ui->setupUi(this);

    //variable setting
    _connectionState = false;

    //signal and slots setting
    initializeSignalsFromOnOfBtn();
    connect(&_socketToRelayController, SIGNAL(signal_connectionStateChange(bool)), this, SLOT(slot_connectionStateChange(bool)));
    connect(ui->pb_connect, SIGNAL(clicked()), this, SLOT(slot_connectionToHost()));
    connect(&_socketToRelayController, SIGNAL(signal_sendFrameToMainWindow(QString)), this, SLOT(slot_relayStateChange(QString)));
    connect(ui->pb_startServer, SIGNAL(clicked()), this, SLOT(slot_startServer()));
    //connect(&_socketToRelayController, SIGNAL(), this, SLOT(slot_addClientIpAddress(QString)));
    connect(ui->pb_stopServer, SIGNAL(clicked()), &_socketToRelayController, SLOT(slot_stopServer()));
    //other settings
    onOfButtonMapInitialize(_socketToRelayController.getCountRelay());
    initializeArrayOfLabel();
}

relayController::~relayController()
{
    delete ui;
}


//заполняет QMap именами кнопок и номером который соответсвует порядковому номеру кнопки.
void relayController::onOfButtonMapInitialize(int countButton)
{
    for (int i =0; i <countButton; i++)
    {
        onOfButtonMap.insert(QString("%1%2").arg("pb_btn").arg(i+1), i+1);
    }
}

void relayController::initializeSignalsFromOnOfBtn()
{
    connect(ui->pb_btn1, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn2, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn3, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn4, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn5, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn6, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn7, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn8, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn9, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn10, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn11, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn12, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn13, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn14, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn15, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
    connect(ui->pb_btn16, SIGNAL(clickByButton(QString)), this, SLOT(slot_OnOfbtnClick(QString)));
}

void relayController::initializeArrayOfLabel()
{
    arrayOfLabel.append(ui->lb_relay1);
    arrayOfLabel.append(ui->lb_relay2);
    arrayOfLabel.append(ui->lb_relay3);
    arrayOfLabel.append(ui->lb_relay4);
    arrayOfLabel.append(ui->lb_relay5);
    arrayOfLabel.append(ui->lb_relay6);
    arrayOfLabel.append(ui->lb_relay7);
    arrayOfLabel.append(ui->lb_relay8);
    arrayOfLabel.append(ui->lb_relay9);
    arrayOfLabel.append(ui->lb_relay10);
    arrayOfLabel.append(ui->lb_relay11);
    arrayOfLabel.append(ui->lb_relay12);
    arrayOfLabel.append(ui->lb_relay13);
    arrayOfLabel.append(ui->lb_relay14);
    arrayOfLabel.append(ui->lb_relay15);
    arrayOfLabel.append(ui->lb_relay16);
}

void relayController::slot_connectionToHost()
{
    QString ipAddress = ui->le_ipAddress->text();
    quint16 port = (quint16)ui->le_port->text().toInt();
    if(ipAddress.isEmpty() || port == NULL)
    {
        qDebug() << "отсутствует ip адрес или порт.";
        return;
    }
    _socketToRelayController.connectToHost(ipAddress, port);
}

void relayController::slot_connectionStateChange(bool state)
{
    if(state)
    {
        qDebug()<<"Соединение с контроллером установленно.";
        _connectionState = true;
        ui->lb_state->setText("подключено");
    }
    else if(!state)
    {
        qDebug()<<"Соединеие не установленно или разорванно.";
        _connectionState = false;
        ui->lb_state->setText("НЕ подключено");
    }
}

//вызывается при нажатии кнопки для включения/выключения реле.
void relayController::slot_OnOfbtnClick(QString buttonName)
{
    int numberOfButton = -1;

    numberOfButton = onOfButtonMap.value(buttonName);
    qDebug() << numberOfButton;
    if(-1 == numberOfButton)
    {
        qDebug() << "Ошибка в определении номера нажатой кнопки.";
        return;
    }
    _socketToRelayController.changeStateRelay(numberOfButton);
}

void relayController::slot_relayStateChange(QString str)
{
    QString relayOn = "Включенно";
    QString relayOff = "Выключенно";

    for(int i = 0; i < str.length(); i++)
    {
        if(str.at(i) == "0")
            arrayOfLabel[i]->setText(relayOff);
        else
            arrayOfLabel[i]->setText(relayOn);
    }
}

void relayController::slot_startServer()
{
    _socketToRelayController.startMyServer(2209);
}

void relayController::slot_addClientIpAddress(QString ipAddress)
{
    qDebug() << ipAddress;
}
