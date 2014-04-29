#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#define Gb1 1000000000
#define Mb100 100000000
#define Mb1 1000000

#include <QMainWindow>
#include <QVector>
#include <QScopedPointer>
#include <QPushButton>
#include <QDebug>
#include <videostreamview.h>
#include <QList>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <dialog_addcamera.h>
#include <QStringListModel>
#include <QMultiMap>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QCloseEvent>
#include <QEvent>
#include <QFileDialog>
#include <filemeneger.h>
#include <QtConcurrent/QtConcurrentRun>
#include <QAction>

namespace Ui {
class videoController;
}

class videoController : public QWidget
{
    Q_OBJECT

public:
    explicit videoController(QWidget *parent = 0);
    ~videoController();
    void closeEvent(QCloseEvent *event);
    //bool eventFilter(QObject *t, QEvent *e);


public slots:
    void HandlerDataToAddCamera(QString IPadres, QString Port, QString Login, QString Password);

private:
    //Properties
    Ui::videoController *ui;
    int currentCountVideoWidget;
    QVector<QSharedPointer<VideoStreamView> > vectorOfWidget;
    struct infoAboutCamera
    {
        QString ipAdress;
        QString port;
        QString userName;
        QString password;

        infoAboutCamera(QString IPAdress = NULL, QString Port = NULL, QString UserName = NULL, QString Password = NULL):
            ipAdress(IPAdress),
            port(Port),
            userName(UserName),
            password(Password)
        {
            qDebug() << "Конструктор структуры";
        }

        ~infoAboutCamera()
        {
            qDebug() << "Деструктор структуры";
        }
    };
    //QList<infoAboutCamera> cameraList;
    QList<QString> cameraIPAdress;
    QMultiMap<QString, infoAboutCamera> cameraListMap;
    QStringListModel model;
    int durationVideo;
    QString path;
    qint64 maxSizeALlFiles;
    qint64 currentSizeALlFiles;
    QAction *aboutCamera;
    QAction *deleteCameraFromList;

    //Methods
    void decrementCountVideoWidget(int count);
    void incrementCountVideoWidget(int count);
    void initializeForm(int dimensionForm);
    void refreshCamList();
    void SetPath();
    void AddWidget(int indexWidget, int counterRow, int counterColumn);
    void SetLineEditDigitValidator();
    void ClearCurrentDir(FileMeneger &filemeneger, QString path);
    void SetContextMenuForCamList();

private slots:
    void HandleACtion4();
    void HandleAction9();
    void HandleACtion16();
    void AddCamera();
    void slot_SendInfoAboutCam(QString IPAdress, int IDobject);
    void slot_GetDurationVideo();
    void slot_SetPath();
    void slot_AddNewFileSize(quint64 newFileSize);
    void slot_ChangeSizeDir();
    void action_aboutCamera();
    void action_deleteCameraFromList();

signals:
    void signal_durationChanged(int duration);
    void signal_appClose();
    void signal_pathChanged(QString newPath);
};

#endif // VIDEOCONTROLLER_H
