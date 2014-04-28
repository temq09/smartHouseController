#ifndef RELAYCONTROLLERSERVER_H
#define RELAYCONTROLLERSERVER_H

#include <QUdpSocket>
#include <QDebug>
#include <QVector>
#include <QIODevice>
#include <QException>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHash>

class relayControllerServer : public QObject
{
    Q_OBJECT
public:
    relayControllerServer();
    void connectToHost(QString ipAddress, quint16 port);
    void changeStateRelay(int relayNumber);  //метод вызывается внешними приложениями
    int getCountRelay();
    void startMyServer(int port);

public slots:
    void readNewDataFromClientSocket();
    void slot_stopServer();

private:
    bool _connectionState;
    QString _ipAddress;
    quint16 _port;
    QTcpSocket _clientSocket;
    QTcpServer* _serverSocket;
    int _relayCount;
    QHash<int, QTcpSocket*> _listOfClient;
    struct RelayControllerState
    {
        int numberRelay; // номер реле. Начинается с 1 до 16.
        bool stateRelay; // состояние. по умолчанию выключенно(false).
    };
    QVector<RelayControllerState> arrayOfRelay;

    void parseDataFromClients(QString dataFromClient);
    void initializeRelayControllerState();
    void controllerStateChange(int onOfCode, int param2, int numberRelay);
    void getRelayStateFromController();
    QString byteConverter(QByteArray byteArray);
    QString reverseFrame(QString binaryString);
    //void handleFrame(QString str);


private slots:
    void slot_connectionStateChange();
    void slot_handleFrame(QString str);
    void slot_newConnection();
    void slot_readNewData();
    void slot_clientDisconnect();
    void slot_removeAllClient();

signals:
    void signal_connectionStateChange(bool state);
    //void signal_relayStateChange(bool state, int numRelay);

    void signal_sendFrameToMainWindow(QString strToSend);
    void signal_sendClientIpAddress(QString ipAddress);

};

#endif // RELAYCONTROLLERSERVER_H
