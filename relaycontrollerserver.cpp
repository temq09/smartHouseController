#include "relaycontrollerserver.h"

relayControllerServer::relayControllerServer()
{
    //variable setting
    _connectionState = false;
    _relayCount = 16;
    _serverSocket = new QTcpServer(this);
    initializeRelayControllerState();

    //signal and slots setting
    connect(&_clientSocket, SIGNAL(connected()) , this, SLOT(slot_connectionStateChange()));
    connect(&_clientSocket, SIGNAL(readyRead()), this, SLOT(readNewDataFromClientSocket()));
}

void relayControllerServer::initializeRelayControllerState()
{
    for(int i = 0; i<_relayCount; i++)
    {
        RelayControllerState relay;
        relay.numberRelay = i+1;
        relay.stateRelay = false;
        arrayOfRelay.append(relay);
    }
}

void relayControllerServer::connectToHost(QString ipAddress, quint16 port)
{
    if(ipAddress.isNull() || port == NULL)
    {
        qDebug() << "ip адрес или порт имеют недопустимое значение.";
    }

    _clientSocket.connectToHost(ipAddress, port);
}

void relayControllerServer::slot_connectionStateChange()
{
    _connectionState = true;
    getRelayStateFromController();
    emit signal_connectionStateChange(_connectionState);
}

void relayControllerServer::changeStateRelay(int relayNumber)
{
    /*
     * В этом методе мы по номеру кнопки определяем ее состояние в текущий момент
     * и отправляем соответствующий код для выключения или включения реле соответствующего номеру кнопки.
     * 17 - код выключения реле
     * 18 - код включения реле
     * 16 - код, после получения которого контроллер отсылает пакет данных.
     */

    qDebug()<<"Определяем включенно или выключенно реле";
    int OnOfCode = 0;
    int param2 = 0;
    //int relayNumberCorrect = relayNumber - 1; // Т.к. в векторе индексация с нуля, то надо уменьшить на единицу чтобы узнать состояние нужного реле

    if(arrayOfRelay[relayNumber-1].stateRelay)
    {
        OnOfCode = 17;
        qDebug()<<"Реле включенно, выключаем";
    }
    else
    {
        OnOfCode = 18;
        qDebug() << "Реле выключенно, включаем";
    }

    controllerStateChange(OnOfCode, param2, arrayOfRelay[relayNumber-1].numberRelay);
    getRelayStateFromController();
}

//функция формирует и отправляет пакет контроллеру
void relayControllerServer::controllerStateChange(int onOfCode, int param2, int numberRelay)
{
    qDebug()<<"Формируем массив для отправки";
    if(_clientSocket.state() == QAbstractSocket::ConnectedState)
    {
        int i = numberRelay + param2 + onOfCode + 85 + 1;
        QByteArray byteArray;
        byteArray.resize(8);
        byteArray[0] = (quint8)85;
        byteArray[1] = (quint8)1;
        byteArray[2] = (quint8)onOfCode;
        byteArray[3] = (quint8)0;
        byteArray[4] = (quint8)0;
        byteArray[5] = (quint8)param2;
        byteArray[6] = (quint8)numberRelay;
        byteArray[7] = (quint8)i;

        try
        {
            _clientSocket.write(byteArray);
            _clientSocket.flush();
            qDebug() << "Данные отправлены";
        }
        catch(QException ex)
        {
            qDebug() << "Ошибка при передаче данных";
        }
    }
    else
        qDebug() << "Соединение не открыто";
}

int relayControllerServer::getCountRelay()
{
    return _relayCount;
}


/*
 *Этот метод вызывается при инициализации программы,
 *а также после отправке данных о изменении состояния реле
 *для того, чтобы определить включенные и выключенные реле.
 */
void relayControllerServer::getRelayStateFromController()
{
    if(_clientSocket.state() == QAbstractSocket::ConnectedState)
    {
        this->controllerStateChange(16,0,0);
    }
}

/*
 * Данный слот вызывается при получении новых данных от сервера.
 * После получения, данные анализируются и отправляются клиентам.
 */
void relayControllerServer::readNewDataFromClientSocket()
{
    try
    {
        QByteArray newData;
        newData.resize(8);
        if(_clientSocket.bytesAvailable() > 8)
            qDebug() << "Мало данных";
        newData = _clientSocket.readAll();
        qDebug() << "Информация считана";
        QString frameFromController = byteConverter(newData);
        qDebug() <<"Конвертированные данные" << frameFromController;
        if(frameFromController.startsWith("22") && frameFromController.length() == 16)
        {
            qDebug() << "Формат кадра правильный. Начинаем обработку данных";
            QString partFrame = frameFromController.right(6);
            partFrame = partFrame.left(4);
            QString binary = QString::number(partFrame.toInt(0,16), 2);
            qDebug() << partFrame << ";" << binary;
            QString reverseFrameStr =  reverseFrame(binary);
            this->slot_handleFrame(reverseFrameStr);
        }
    }
    catch(QException ex)
    {
        qDebug() << "Ошибка при чтении информации";
    }
}


//метод конвертирования информации полученной от контроллера в строку.
QString relayControllerServer::byteConverter(QByteArray byteArray)
{
    QByteArray hexArray;
    hexArray.resize(16);
    //hexArray = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70 };
    hexArray[0] = 48;
    hexArray[1] = 49;
    hexArray[2] = 50;
    hexArray[3] = 51;
    hexArray[4] = 52;
    hexArray[5] = 53;
    hexArray[6] = 54;
    hexArray[7] = 55;
    hexArray[8] = 56;
    hexArray[9] = 57;
    hexArray[10] = 65;
    hexArray[11] = 66;
    hexArray[12] = 67;
    hexArray[13] = 68;
    hexArray[14] = 69;
    hexArray[15] = 70;
    QByteArray arrayOfChar;
    arrayOfChar.resize(2 * byteArray.length());
    int i = 0;
    for(int j = 0;j<byteArray.length();j++)
    {
        int k = 0xFF & byteArray[j];
        int m = i + 1;
        arrayOfChar[i] = hexArray[(k >> 4)];
        i = m + 1;
        arrayOfChar[m] = hexArray[(k&0xF)];
    }
    return QString(arrayOfChar);
}


//инвертируем кадр т.к. индекс символа = порядковому номеру реле
QString relayControllerServer::reverseFrame(QString binaryString)
{
    QString str = binaryString;
    while(str.length() < _relayCount)
        str.insert(0, "0");
    QString reverseStr;
    for(int i = 0, j = str.length() - 1; j >= 0; j--, i++)
    {
        reverseStr.append(str.at(j));
    }
    qDebug() << str << "\n" << reverseStr;
    return reverseStr;
}

void relayControllerServer::slot_handleFrame(QString str)
{
    qDebug() << "rrr";
    for(int i = 0; i < str.length(); i++)
    {
        (str.at(i) == "0") ? arrayOfRelay[i].stateRelay = false : arrayOfRelay[i].stateRelay = true;
    }
    //отправляем пакет с информацией о состоянии реле всем клиентам.
    QByteArray dataToSend(str.toLatin1());
    foreach (int clientDescriptor, _listOfClient.keys())
    {
        _listOfClient.value(clientDescriptor)->write(dataToSend);
        _listOfClient.value(clientDescriptor)->flush();
    }
    //на этот сигнал подписанно главное окно приложения. Высылается строка с состоянием всех реле.
    emit signal_sendFrameToMainWindow(str);
}

void relayControllerServer::startMyServer(int port)
{
    if(!_serverSocket->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server not started";
        _serverSocket->close();
        return;
    }
    else
        qDebug() << "Сервер запущен успешно";

    connect(_serverSocket, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));
}

void relayControllerServer::slot_newConnection()
{
    while(_serverSocket->hasPendingConnections())
    {
        QTcpSocket* clientSocket = _serverSocket->nextPendingConnection();
        int clientDescriptor = _serverSocket->socketDescriptor();
        _listOfClient.insert(clientDescriptor, clientSocket);
        emit signal_sendClientIpAddress( clientSocket->localAddress().toString());
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slot_readNewData()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slot_clientDisconnect()));
    }
    getRelayStateFromController();
}

//получаем данные от подключенных к приложению клиентов
void relayControllerServer::slot_readNewData()
{
    QByteArray dataFromClient;
    QTcpSocket *client = (QTcpSocket*) sender();
    dataFromClient = client->readAll();
    qDebug() << "Данные получены";
    QString str(dataFromClient);
    qDebug() << str;
    parseDataFromClients(str);
}

//обрабатываем полученные данные
void relayControllerServer::parseDataFromClients(QString dataFromClient)
{
    if(dataFromClient.startsWith("22"))
    {
        qDebug() << "Формат кадра правильный. Начинаем обработку";
        int relayNumber = dataFromClient.right(2).toInt();
        if(relayNumber <= 0 || relayNumber > 15)
        {
            qDebug() << "Неправильный номер реле.";
            return;
        }
        changeStateRelay(relayNumber);
    }
}

//вызывается при отключение клиента от программы
void relayControllerServer::slot_clientDisconnect()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    qDebug() << "Клиент с ip адресом - " << clientSocket->localAddress().toString() << " отключился";
    if ( 0 < _listOfClient.remove(clientSocket->socketDescriptor()))
    {
        qDebug() << "Клиент с ip адресом " << clientSocket->localAddress().toString() << " удален из списка подписчиков";
    }
}

void relayControllerServer::slot_stopServer()
{
    if(_serverSocket->isListening())
    {
        qDebug() << "Отключаем всех клиентов";
        slot_removeAllClient();

        qDebug() << "Останавливаем сервер";
        _serverSocket->close();
    }
}

//вызывается при отсановке сервера для удаления всех подключенных сокетов.
void relayControllerServer::slot_removeAllClient()
{
    foreach (int socketDescriptor, _listOfClient.keys())
    {
        _listOfClient.value(socketDescriptor)->close();
    }
}
