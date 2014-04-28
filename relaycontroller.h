#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <QMainWindow>
#include <QDebug>
#include <QMap>
#include <QLabel>
#include "relaycontrollerserver.h"

namespace Ui {
class relayController;
}

class relayController : public QWidget
{
    Q_OBJECT

public:
    explicit relayController(QWidget *parent = 0);
    ~relayController();

public slots:
    void slot_connectionStateChange(bool state);
    void slot_relayStateChange(QString str);
    void slot_addClientIpAddress(QString ipAddress);

private:
    Ui::relayController *ui;
    relayControllerServer _socketToRelayController;
    bool _connectionState;
    QMap<QString, int> onOfButtonMap;
    QVector<QLabel*> arrayOfLabel;

    void initializeSignalsFromOnOfBtn();
    void initializeArrayOfLabel();
    void onOfButtonMapInitialize(int countButton);
    void changeLabelState(int numberLabel, QString message);

private slots:
    void slot_connectionToHost();
    void slot_OnOfbtnClick(QString buttonName);
    void slot_startServer();

};


#endif // RELAYCONTROLLER_H
