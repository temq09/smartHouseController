#ifndef VIDEOSTREAMVIEW_H
#define VIDEOSTREAMVIEW_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTimer>
#include <QDir>
#include <QPixmap>
#include <QImage>
#include <QPicture>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QCloseEvent>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <detectmotion.h>
#include <QtConcurrent/QtConcurrent>

namespace Ui {
class VideoStreamView;
}

class VideoStreamView : public QWidget
{
    Q_OBJECT

public:
    VideoStreamView(QWidget *parent = 0);
    VideoStreamView(int Duration, int IDWidget, QString Path, QWidget *parent = 0);
    ~VideoStreamView();
    void SetDataAboutCamera(QString IPAdress, QString Port, QString Login, QString Password);
    void setIDWidget(int id);
    bool eventFilter(QObject *t, QEvent *e);

public slots:
    void durationChange(int Duration);
    void pathChange(QString newPath);
    void StopStream();


private:
    Ui::VideoStreamView *ui;
    QString ipAdress;
    QString port;
    QString login;
    QString password;
    QString path;
    int duration;
    QTimer* timer;
    CvCapture* p_capture;
    bool stateVideo;
    bool stateRecordVideo;
    bool stateMotionDetector;
    QString nameVideo;
    int idWidget;
    QString connectioString;
    int needWidth;
    int needHeight;
    int stepGrabFrame;
    CvVideoWriter *writer;
    DetectMotion* detectmotion;
    QTimer* timerFrame;
    int countFrame;
    QString RecordIsActive;
    QString RecordIsNoActive;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void StartRecord();
    void GenerateFileName(QString path);
    QImage IplImage2QImage(IplImage *iplImage);
    void ReleaseResources();
    void GrabFrame();
    void ReSizeImg();
    bool InitializeCapture();
    IplImage* GetCurrentFrame();
    void ChangeRecordVideoLabel(bool state);
    quint64 CalcSizeLastFile();

private slots:
    void slot_StartRecord();
    void slot_StopRecord();
    void slot_changeMotionDetect(int state);
    void slot_EndRecordTime();
    void slot_StartStream();

signals:
    void signal_SendIpAdress(QString ipAdress, int id);
    void signal_GrabFrameEnd();
    void signal_SendSizeFile(quint64 sizeFile);
};

#endif // VIDEOSTREAMVIEW_H
