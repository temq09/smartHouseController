#include "detectmotion.h"

DetectMotion::DetectMotion(IplImage *firstFrame)
{
    prevFrame = cvCreateMat(firstFrame->height, firstFrame->width, firstFrame->depth);
    currentFrame = cvCreateMat(firstFrame->height, firstFrame->width, firstFrame->depth);
    cont = nullptr;
    mem = cvCreateMemStorage(0);
    minRadius = 1;
    prevFrameIsAviable = false;
}

DetectMotion::~DetectMotion()
{
    ReleaseResources();
}

void DetectMotion::detectMotion(const IplImage *currentFrameFromCamera)
{
    if(!prevFrameIsAviable)
    {
        prevFrame = cv::Mat(currentFrameFromCamera);
        cv::cvtColor(prevFrame, prevFrame, CV_RGB2GRAY);
        cv::blur(prevFrame, prevFrame, cv::Size(2,2));
        prevFrameIsAviable = true;
    }
    else
    {
        qDebug() << "2";
        currentFrame = cv::Mat(currentFrameFromCamera);
        cv::cvtColor(currentFrame, currentFrame, CV_RGB2GRAY);
        cv::blur(currentFrame, currentFrame, cv::Size(2,2));
        cv::absdiff(currentFrame, prevFrame, mask);
        cv::threshold(mask, maskBin, 50, 250, CV_THRESH_BINARY);
        cv::erode(maskBin, maskBin, cv::Mat());
        cv::dilate(maskBin, maskBin, cv::Mat());
        prevFrame = currentFrame.clone();
        detc = maskBin.clone();

        cvFindContours(&detc, mem, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

        for(CvSeq* c = cont; c!=NULL; c=c->h_next)
        {
            CvPoint2D32f center_1;
            float radius_1 = 0;
            cvMinEnclosingCircle(c, &center_1, &radius_1);
            if (radius_1>minRadius)
            {
                qDebug() << "Есть движение";
                DetectedMovement();
                break;
            }
        }
        cvClearMemStorage(mem);
    }

}

void DetectMotion::ReleaseResources()
{
    if(mem)
    {
        cvReleaseMemStorage(&mem);
    }
}

void DetectMotion::DetectedMovement()
{
    prevFrameIsAviable = false;
    emit signal_motionDetected();
}

