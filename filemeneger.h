#ifndef FILEMENEGER_H
#define FILEMENEGER_H

#define OneGb 1000000000

#include <QDebug>
#include <QDir>
#include <QObject>
#include <QList>
#include <QTextCodec>
#include <QDateTime>
#include <QFile>

class FileMeneger : public QObject
{
    Q_OBJECT

public:
    explicit FileMeneger(QObject *parent = 0);
    ~FileMeneger();
    qint64 CalcSizeDir(QString pathToCurrentDir);
    qint64 ClearDir(QString pathForClear);
    QString rootPath;
    QString FindOldFile(QString path, qint64 &sizeDeletedFile);
    bool DeleteOldFile(QString pathToDelete);

public slots:
    void SetPathForCalcSize(QString path); 

private:
    Q_DISABLE_COPY(FileMeneger)
};

#endif // FILEMENEGER_H
