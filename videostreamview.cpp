#include "videostreamview.h"
#include "ui_videostreamview.h"

VideoStreamView::VideoStreamView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoStreamView)
{
    ui->setupUi(this);
}

VideoStreamView::VideoStreamView(int Duration, int IDWidget, QString Path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoStreamView),
    duration(Duration),
    idWidget(IDWidget),
    path(Path),
    p_capture(NULL),
    stateVideo(false),
    stateRecordVideo(false),
    stateMotionDetector(false),
    detectmotion(nullptr),
    stepGrabFrame(20),
    countFrame(0),
    RecordIsActive("запись активна"),
    RecordIsNoActive("запись не активна")
{
    ui->setupUi(this);
    ui->lb_video->setAcceptDrops(true);
    this->setAcceptDrops(true);
    ui->lb_video->installEventFilter(this);
    ui->cb_detectMotion->setEnabled(false);
    timer = nullptr;
    writer = nullptr;
    timerFrame = new QTimer(this);
    ChangeRecordVideoLabel(stateRecordVideo);

    connect(ui->pb_StartRecord, SIGNAL(clicked()),this, SLOT(slot_StartRecord()));
    connect(ui->pb_StopRecord, SIGNAL(clicked()), this , SLOT(slot_StopRecord()));
    connect(ui->cb_detectMotion, SIGNAL(stateChanged(int)), this, SLOT(slot_changeMotionDetect(int)));
    connect(this, SIGNAL(signal_GrabFrameEnd()), SLOT(slot_StartStream()));
    connect(timerFrame, SIGNAL(timeout()), SLOT(slot_StartStream()));
}

bool VideoStreamView::InitializeCapture()
{
    qDebug() << "Initizlize camera";
    p_capture = cvCreateFileCapture(connectioString.toStdString().c_str());
    if(p_capture)
    {
        qDebug() << " Initialize camera is successeful";
        stateVideo = true;
        return true;
    }
    else
    {
        qDebug() << " Initialize camera is error";
        return false;
    }
}

VideoStreamView::~VideoStreamView()
{
    qDebug() <<"Запускаем деструктор класса " << this->objectName() << "with id = " << this->idWidget;
    if(stateVideo)
    {
        StopStream();
        qDebug() << stateVideo;
    }
    ReleaseResources();
    delete ui;
    qDebug() << "Деструктор завершен успешно";
}

void VideoStreamView::slot_StartStream()
{
    if(p_capture && stateVideo)
    {
        ui->lb_status->setText("есть сигнал");
        while (stateVideo) {
            QFuture<void> future = QtConcurrent::run(this, &VideoStreamView::GrabFrame);
            future.waitForFinished();
            QApplication::processEvents();
        }
    }
    else
    {
        qDebug() << "Error";
        ReleaseResources();
    }
    //timerFrame->start(stepGrabFrame);
}

void VideoStreamView::GrabFrame()
{
    IplImage *img = cvCreateImage(cvSize(needWidth, needHeight), IPL_DEPTH_8U, 3);
    try
    {
        const IplImage *imgFromCam = NULL;
        imgFromCam = cvQueryFrame(p_capture);
        if(imgFromCam == NULL)
        {
            qDebug() << "Камера не доступна";
            return;
        }

        //если включен датчик движения и запись видео не ведется
        if(stateMotionDetector && !stateRecordVideo)
        {
            //передаем кадры классу детоктора движения
            if(detectmotion && 24 == countFrame)
            {
                detectmotion->detectMotion(imgFromCam);
                countFrame = 0;
            }
            else
                countFrame++;
        }
        if(stateRecordVideo)
        {
            cvWriteFrame(writer, imgFromCam);
        }
        cvResize(imgFromCam, img);
        ui->lb_video->setPixmap(QPixmap::fromImage(IplImage2QImage(img)));
        cvReleaseImage(&img);
    }
    catch(...)
    {
        stateVideo = false;
        if(img)
        {
            cvReleaseImage(&img);
        }
        qDebug() << "Поизошла неизвестная ошикбка";
    }
    QThread::msleep(50);
}

QImage VideoStreamView::IplImage2QImage(IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        //в этом месте в дебаге программа падает с сообщением о порче дэбажной кучи
        return img.rgbSwapped();
    }
    else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1)
    {
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
        {
            colorTable.push_back(qRgb(i, i, i));
        }
        img.setColorTable(colorTable);
        return img;
    }
    else
    {
        qWarning() << "Image cannot be converted.";
        return QImage();
    }
}

void VideoStreamView::ReSizeImg()
{
    int widthLabel = ui->lb_video->size().width();
    int heightLabel = ui->lb_video->size().height();
    needWidth = 0;
    needHeight = 0;
    if(widthLabel >= heightLabel)
    {
        needWidth = heightLabel*1.3333;
        needHeight = heightLabel;
    }
    else if (widthLabel < heightLabel)
    {
        needWidth = widthLabel;
        needHeight = widthLabel / 1.3333;
    }

    if((heightLabel - needHeight) < 5)
    {
        needWidth = needWidth*0.9;
        needHeight = needHeight*0.9;
    }
    else if((widthLabel-needWidth)< 5)
    {
        needWidth = needWidth*0.9;
        needHeight = needHeight*0.9;
    }
}

void VideoStreamView::StopStream()
{
    qDebug() << "Останавливаем трансляцию видео потока";
    if(stateVideo)
    {
        stateVideo = false;
        ui->lb_status->setText("нет сигнала");
        qDebug() << "Состояние видео потока - " <<stateVideo;
    }
}

void VideoStreamView::slot_StartRecord()
{
    if(!stateVideo)
    {
        qDebug() << "Трансляция не активна. Запись не возможна";
        return;
    }

    /* если уже идет запись видео, то сначало ее надо остановить*/
    if(stateRecordVideo)
    {
        slot_StopRecord();
    }
    StartRecord();
}

void VideoStreamView::StartRecord()
{
    //удаляем старый таймер если он есть
    if(timer)
    {
        delete timer;
        timer = nullptr;
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_EndRecordTime()));
    //удаляем старого писателя если он есть
    if(writer)
    {
        cvReleaseVideoWriter(&writer);
    }
    //генерируем путь
    QDateTime date = QDateTime::currentDateTime();
    QString pathTemp = QString("%1\\%2\\%3\\%4").arg(path).arg(date.date().month()).arg(date.date().day()).arg(this->ipAdress);
    qDebug() << pathTemp;
    QDir dir;
    if(dir.mkpath(pathTemp))
    {
        try
        {
            GenerateFileName(pathTemp);
            writer = cvCreateVideoWriter(nameVideo.toStdString().c_str(), CV_FOURCC('X', 'V', 'I', 'D'), 15, cvSize(640, 480), 1);
            if(writer)
            {
                //если писатель успешно создан, то флаг записи устанавливаем в тру
                qDebug() << "Writer create completed";
                stateRecordVideo = true;
                ChangeRecordVideoLabel(stateRecordVideo);
            }

            //если продолжительность не равно нулю(т.е. пишем не в один файл)
            if(duration != 0)
            {
                timer->start(duration);
                qDebug() << "Start timer at " << duration << "msec";
            }
        }
        catch(...)
        {
            qDebug() << "Error when creating writer";
            stateRecordVideo = false;
            ChangeRecordVideoLabel(stateRecordVideo);
        }
    }
    else
    {
        qDebug() << "Невозможно создать папку";
    }
}

void VideoStreamView::slot_EndRecordTime()
{
    qDebug() << "Record time is ended.";
    //в данный метод входим после таймаута таймера записи видео.

    //Определяем размер последнего записанного файла
    emit signal_SendSizeFile(CalcSizeLastFile());

    //если датчик движения включен, то флаг записи устанавливаем в false,
    //это значит что прекращаем запись видео до момента следующего срабатывания датчика
    if(stateMotionDetector)
    {
        slot_StopRecord();
    }
    //если датчик выключен, то вызываем метод записи видео
    else
    {
        slot_StartRecord();
    }
    qDebug() << "leave endRecordTime method";
}

void VideoStreamView::slot_StopRecord()
{
    if (timer)
    {
        if(timer->isActive())
        {
            qDebug() << "Timer is active. Stop timer";
            timer->stop();
            delete timer;
            timer = nullptr;
        }
    }
    if(writer)
    {
        qDebug() << "Writer is active. Release writer";
        cvReleaseVideoWriter(&writer);
        stateRecordVideo = false;
        writer = nullptr;
        ChangeRecordVideoLabel(stateRecordVideo);
    }
    qDebug() << "Stop Record";
}

quint64 VideoStreamView::CalcSizeLastFile()
{
    QFileInfo fileInfo(nameVideo);
    if(fileInfo.isFile())
    {
        quint64 fileSize = fileInfo.size();
        qDebug() << "Размер файла: " << nameVideo << " = " << fileSize;
        return fileSize;
    }
    return 0;
}

void VideoStreamView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("application/infoCamera"))
    {
        event->acceptProposedAction();
    }
}

void VideoStreamView::dropEvent(QDropEvent *event)
{
    qDebug() << event->mimeData()->text();
    emit signal_SendIpAdress(event->mimeData()->text(), this->idWidget);
}

void VideoStreamView::SetDataAboutCamera(QString IPAdress, QString Port, QString Login, QString Password)
{
    ipAdress = IPAdress;
    port = Port;
    login = Login;
    password = Password;
    qDebug() << ipAdress << port << login << password << "duration:" << this->duration;
    //http://192.168.1.12:99/videostream.cgi?loginuse=admin&loginpas=
    connectioString = QString("http://%1:%4/videostream.cgi?loginuse=%2&loginpas=%3&.mjpg").arg(this->ipAdress).arg(this->login).arg(this->password).arg(this->port);
    qDebug() << connectioString;
    if(InitializeCapture())
    {
        slot_StartStream();
        ui->cb_detectMotion->setEnabled(true);
    }
}

void VideoStreamView::durationChange(int Duration)
{
    qDebug() << "for ip camera whit ip_adress" << this->ipAdress << "daration changed on:" << Duration << "msec";
    this->duration = Duration;
}

void VideoStreamView::pathChange(QString newPath)
{
    this->path = newPath;
}

void VideoStreamView::GenerateFileName(QString path)
{
    QDateTime time = QDateTime::currentDateTime();
    nameVideo = QString("%6\\AV_%1%2%5_%3%4.avi").arg(time.date().year()).arg(time.date().month()).arg(time.time().hour()).arg(time.time().minute()).arg(time.date().day()).arg(path);
    qDebug() << "File Name :" << nameVideo;
}

void VideoStreamView::ReleaseResources()
{
    if(p_capture)
    {
        qDebug() << "Release p_capture";
        cvReleaseCapture(&p_capture);
    }
    if(writer)
    {
        cvReleaseVideoWriter(&writer);
    }
}

bool VideoStreamView::eventFilter(QObject *t, QEvent *e)
{
    if(t == ui->lb_video)
    {
        if (e->type() == QEvent::Resize)
        {
            qDebug() << "Resize label";
            ReSizeImg();
            if(stateVideo)
            {
                QFuture<void> future = QtConcurrent::run(this, &VideoStreamView::GrabFrame);
                future.waitForFinished();
            }
        }
    }
    return QWidget::eventFilter(t,e);
}

void VideoStreamView::slot_changeMotionDetect(int state)
{
    switch(state)
    {
    case 0:
        qDebug() << "Выключаем датчик";
        if(detectmotion)
        {
            delete detectmotion;
            stateMotionDetector = false;
        }
        break;
    case 2:
        qDebug() << "Включаем датчик";
        detectmotion = new DetectMotion(GetCurrentFrame());
        if(detectmotion)
        {
            stateMotionDetector = true;
            connect(detectmotion, SIGNAL(signal_motionDetected()), this, SLOT(slot_StartRecord()));
        }
    }
}

IplImage *VideoStreamView::GetCurrentFrame()
{
    if(p_capture)
        return cvQueryFrame(p_capture);
    else
        return NULL;
}

void VideoStreamView::ChangeRecordVideoLabel(bool state)
{
    if(state)
    {
        ui->lb_stateRecord->setText(RecordIsActive);
    }
    else
    {
        ui->lb_stateRecord->setText(RecordIsNoActive);
    }
}
