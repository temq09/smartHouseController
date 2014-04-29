#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
#include <QMouseEvent>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QApplication>

class myListView : public QListView
{
    Q_OBJECT
public:
    explicit myListView(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

private:
    QPoint dragStartPos;

signals:

public slots:

};

#endif // MYLISTVIEW_H
