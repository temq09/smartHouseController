#ifndef DETECTMOTION_H
#define DETECTMOTION_H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <QDebug>

class DetectMotion : public QObject
{
    Q_OBJECT

public:
    DetectMotion(IplImage* firstFrame);
    ~DetectMotion();
    void detectMotion(const IplImage* currentFrameFromCamera);

private:
    cv::Mat mask;
    cv::Mat maskBin;
    cv::Mat prevFrame;
    cv::Mat currentFrame;
    CvMat detc;
    CvSeq* cont;
    CvMemStorage* mem;
    int minRadius;
    bool prevFrameIsAviable;

    void ReleaseResources();
    void DetectedMovement();

signals:
    void signal_motionDetected();
};

#endif // DETECTMOTION_H
