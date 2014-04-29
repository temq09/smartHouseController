#include "videocontroller.h"
#include "ui_videocontroller.h"

videoController::videoController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoController)
{
    ui->setupUi(this);

    slot_GetDurationVideo();
    SetLineEditDigitValidator();
    path = "C:\\VideoFromCam";
    ui->le_Path->setText(path);
    currentSizeALlFiles = 0;
    ui->le_SizeDir->setText("0");

    SetContextMenuForCamList();

    connect(ui->pb_4, SIGNAL(clicked()), SLOT(HandleACtion4()));
    connect(ui->pb_9, SIGNAL(clicked()), SLOT(HandleAction9()));
    connect(ui->pb_16, SIGNAL(clicked()), SLOT(HandleACtion16()));
    connect(ui->pb_addCam, SIGNAL(clicked()), SLOT(AddCamera()));
    connect(ui->cb_duration, SIGNAL(currentIndexChanged(int)), SLOT(slot_GetDurationVideo()));
    connect(ui->pb_SetPath, SIGNAL(clicked()), SLOT(slot_SetPath()));
    connect(ui->le_SizeDir, SIGNAL(editingFinished()), this, SLOT(slot_ChangeSizeDir()));
    currentCountVideoWidget = 0;
    initializeForm(4);
    slot_ChangeSizeDir();
}

void videoController::SetContextMenuForCamList()
{
    ui->lv_listOfCam->setContextMenuPolicy(Qt::ActionsContextMenu);
    aboutCamera = new QAction("Редактировать", ui->lv_listOfCam);
    deleteCameraFromList = new QAction("Удалить из списка",ui->lv_listOfCam);
    connect(aboutCamera, SIGNAL(triggered()), this, SLOT(action_aboutCamera()));
    connect(deleteCameraFromList, SIGNAL(triggered()), this, SLOT(action_deleteCameraFromList()));
    ui->lv_listOfCam->addAction(aboutCamera);
}

void videoController::SetLineEditDigitValidator()
{
    QValidator *validator = new QIntValidator(0, 100, this);
    ui->le_SizeDir->setValidator(validator);
}

void videoController::slot_GetDurationVideo()
{
    int duration = 0;
    switch (ui->cb_duration->currentIndex())
    {
    case 0: duration = 120 * 1000;  //sec to msec
        break;
    case 1: duration = 240 * 1000;
        break;
    case 2: duration = 480 * 1000;
        break;
    case 3: break;
    }
    qDebug() << "Video duration is : " << duration << "msec";
    this->durationVideo = duration;
    emit signal_durationChanged(duration);
}

void videoController::initializeForm(int dimensionForm)
{
    int counterColumn = 0;
    int counterRow = 0;
    for (int i = 0; i < dimensionForm; i++)
    {
        if(counterColumn == sqrt(dimensionForm))
        {
            counterColumn = 0;
            counterRow++;
        }
        AddWidget(i, counterRow, counterColumn);
        counterColumn++;
    }
    currentCountVideoWidget += dimensionForm;
    qDebug() << currentCountVideoWidget;
}

void videoController::AddWidget(int indexWidget, int counterRow, int counterColumn)
{
    vectorOfWidget.push_back(QSharedPointer<VideoStreamView> (new VideoStreamView(this->durationVideo, indexWidget, this->path, this)));
    ui->gridLayout->addWidget(vectorOfWidget.last().data(),counterRow, counterColumn);
    connect(vectorOfWidget.last().data(), SIGNAL(signal_SendIpAdress(QString,int)), this, SLOT(slot_SendInfoAboutCam(QString,int)));
    connect(this, SIGNAL(signal_durationChanged(int)), vectorOfWidget.last().data(), SLOT(durationChange(int)));
    connect(this, SIGNAL(signal_appClose()), vectorOfWidget.last().data(), SLOT(StopStream()));
    connect(this, SIGNAL(signal_pathChanged(QString)), vectorOfWidget.last().data(), SLOT(pathChange(QString)));
    connect(vectorOfWidget.last().data(), SIGNAL(signal_SendSizeFile(quint64)), this, SLOT(slot_AddNewFileSize(quint64)));
}

videoController::~videoController()
{
    qDebug() << "Запускаем деструктор класса mainwindow";
    delete ui;
}

void videoController::HandleACtion4()
{
    if(currentCountVideoWidget == 9)
    {
        this->decrementCountVideoWidget(5);
    }
    else if(currentCountVideoWidget == 16)
    {
        this->decrementCountVideoWidget(7);
        this->decrementCountVideoWidget(5);
    }
}

void videoController::HandleAction9()
{
    if(currentCountVideoWidget == 4)
    {
        this->incrementCountVideoWidget(5);
    }
    else if(currentCountVideoWidget == 16)
    {
        this->decrementCountVideoWidget(7);
    }
}

void videoController::HandleACtion16()
{
    if(currentCountVideoWidget == 4)
    {
        this->incrementCountVideoWidget(5);
        this->incrementCountVideoWidget(7);
    }
    else if(currentCountVideoWidget == 9)
    {
        this->incrementCountVideoWidget(7);
    }
}

void videoController::decrementCountVideoWidget(int count)
{
    qDebug() << "Удаляем лишние ячейки";
    qDebug() << "Текущее состояние таблицы:" ;
    qDebug() << "Количество строк"  << ui->gridLayout->rowCount();
    qDebug() << "Количество столбцов " << ui->gridLayout->columnCount();
    int numberRow = 0;
    int numberColumn = 0;
    numberRow = sqrt(currentCountVideoWidget) - 2 ;  //количество строк которое должно стать после удаления
    numberColumn = sqrt(currentCountVideoWidget) - 2 ; //количество столбцов которое должно стать после удаления
    if(numberRow != 0 && numberColumn != 0)
    {
        int counterRow = sqrt(currentCountVideoWidget) - 1;
        int counterColumn = sqrt(currentCountVideoWidget) - 1;
        while(true)
        {
            if(counterRow > numberRow || counterColumn > numberColumn)
            {
                qDebug() << "Удаляем ячейку с индексами " << counterRow << counterColumn;
                ui->gridLayout->removeWidget(vectorOfWidget.last().data());
                //vectorOfWidget.last().data()->StopStream();
                vectorOfWidget.pop_back();
            }

            if(counterColumn == 0 && counterRow == 0)
            {
                qDebug() << "Необходимые ячейки удалены";
                qDebug() << "Текущее состояние таблицы:" ;
                qDebug() << "Количество строк"  << ui->gridLayout->rowCount();
                qDebug() << "Количество столбцов " << ui->gridLayout->columnCount();
                currentCountVideoWidget -=count;
                qDebug() << currentCountVideoWidget;
                return;
            }
            else if(counterColumn == 0)
            {
                counterRow --;
                counterColumn = sqrt(currentCountVideoWidget) - 1;
            }
            else
            {
                counterColumn --;
            }
        }
    }
}

void videoController::incrementCountVideoWidget(int count)
{
    int numberRow = 0;
    int numberColumn = 0;
    numberRow = sqrt(currentCountVideoWidget) ;  //текущее количество строк
    numberColumn = sqrt(currentCountVideoWidget) ; //текущее количество столбцов
    qDebug() << numberRow << " " << numberColumn;
    if(numberRow != 0 && numberColumn != 0)
    {
        int counterRow = 0;
        int counterColumn = 0;
        int id = currentCountVideoWidget;
        for(int i = 0 ; i< currentCountVideoWidget + count; i++)
        {

            //при выполнении этого условия индексы показывают на ячейку которую нужно заполнить виджетом
            if(counterRow == numberRow || counterColumn == numberColumn)
            {
                AddWidget(id, counterRow, counterColumn);
                id++;
            }

            //выполняется когда заполнили 1 крайний столбик справа и 1 строку снизу
            if(counterColumn  == numberColumn  && counterRow  == numberRow  )
            {
                numberRow++;
                numberColumn++;
                counterColumn = 0;
                counterRow = 0;
            }
            //выполняется когда вставили виджет в крайний столбик справа
            else if(counterColumn > numberColumn - 1  )
            {
                counterColumn = 0;
                counterRow ++;
            }
            else
            {
                counterColumn++;
            }
        }
        currentCountVideoWidget +=count;
    }
}

void videoController::slot_SendInfoAboutCam(QString IPAdress, int IDobject)
{
    infoAboutCamera info = cameraListMap.value(IPAdress);
    vectorOfWidget.at(IDobject).data()->SetDataAboutCamera(info.ipAdress, info.port, info.userName, info.password);
}

void videoController::AddCamera()
{
    QScopedPointer<Dialog_addCamera> dialogAddCamera(new Dialog_addCamera(this));
    connect(dialogAddCamera.data(), SIGNAL(EnterDataAreSuccessful(QString,QString,QString,QString)), this, SLOT(HandlerDataToAddCamera(QString,QString,QString,QString)));
    dialogAddCamera.data()->exec();
}

void videoController::HandlerDataToAddCamera(QString IPadres, QString Port, QString Login, QString Password)
{
    qDebug() << IPadres << Port << "\n" << Login << Password;
    infoAboutCamera CameraInfo(IPadres, Port, Login, Password);
    cameraIPAdress.append(IPadres);
    cameraListMap.insert(IPadres, CameraInfo);
    refreshCamList();
}

void videoController::refreshCamList()
{
    model.setStringList(cameraIPAdress);
    ui->lv_listOfCam->setModel(&model);
    qDebug() << "Refresh camera list";
}

void videoController::closeEvent(QCloseEvent *event)
{
    qDebug() << "Готовимся к закрытию приложения. Останавливаем видеопотоки.";
    emit signal_appClose();
    qDebug() << "Закрываем приложение";
    event->accept();
}

void videoController::slot_SetPath()
{
    qDebug() <<"Set path.";
    QString pathDir = QFileDialog::getExistingDirectory(this,
                                                        "Укажите путь",
                                                        "C:\\videoFromCam",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!pathDir.isEmpty())
    {
        this->path = pathDir;
        ui->le_Path->setText(pathDir);
        emit signal_pathChanged(pathDir);
    }
}

void videoController::slot_AddNewFileSize(quint64 newFileSize)
{
    currentSizeALlFiles += newFileSize;
    if( 0 == maxSizeALlFiles)
        return;
    else
    {
        FileMeneger filemeneger;
        //filemeneger.ClearDir(this->path);
        ClearCurrentDir(filemeneger, this->path);
    }
}

void videoController::slot_ChangeSizeDir()
{
    if(0 == ui->le_SizeDir->text().toInt())
    {
        maxSizeALlFiles = 0;
        qDebug() << "Размер папки не отслеживается";
    }
    else
    {
        qint64 sizeDir = ui->le_SizeDir->text().toInt();
        maxSizeALlFiles = sizeDir * Gb1;                //переводим в байты
        qDebug() << "Максимальный размер папки составляет - " << maxSizeALlFiles;
        FileMeneger filemeneger;
        QFuture<qint64> future = QtConcurrent::run(&filemeneger, &FileMeneger::CalcSizeDir, this->path);
        currentSizeALlFiles = future.result();
        ClearCurrentDir(filemeneger, this->path);
    }
}

void videoController::ClearCurrentDir(FileMeneger &filemeneger, QString path)
{
    int countDeletedFiles = 0;
    qint64 totalSizeDeletedFiles = 0;
    while(maxSizeALlFiles - currentSizeALlFiles < Mb100)
    {
        QFuture<qint64> future = QtConcurrent::run(&filemeneger, &FileMeneger::ClearDir, path);
        currentSizeALlFiles -= future.result();
        if(future.result()!=0)
        {
            countDeletedFiles++;
            totalSizeDeletedFiles += future.result();
        }
    }
    qDebug() << "Результаты очистки:" << "\nфайлов удаленно - " << countDeletedFiles << "\nобщий размер удаленных файлов - " <<totalSizeDeletedFiles/Mb1 << "Мб";
}

void videoController::action_aboutCamera()
{
    int index = -1;
    index = ui->lv_listOfCam->currentIndex().row();
    if(index != -1)
    {
        QScopedPointer<Dialog_addCamera> dialogAddCamera(new Dialog_addCamera(this));
        connect(dialogAddCamera.data(), SIGNAL(EnterDataAreSuccessful(QString,QString,QString,QString)), this, SLOT(HandlerDataToAddCamera(QString,QString,QString,QString)));
        QString currentIp = cameraIPAdress.at(index);
        infoAboutCamera info = cameraListMap.value(currentIp);
        dialogAddCamera.data()->setDataAboutCamera(info.ipAdress, info.port, info.userName, info.password);
        dialogAddCamera.data()->exec();
    }
}

void videoController::action_deleteCameraFromList()
{

}
