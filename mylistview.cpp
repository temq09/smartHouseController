#include "mylistview.h"

myListView::myListView(QWidget *parent) :
    QListView(parent)
{

}

void myListView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "mouse press left button";
        dragStartPos = event->pos();
    }
}

void myListView::mouseMoveEvent(QMouseEvent *e)
{
    if((e->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance())
    {
        qDebug() << "eee";
        return;
    }
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    int index = this->indexAt(dragStartPos).row();
    qDebug() << this->indexAt(dragStartPos).data().toString();
    mimeData->setText(this->indexAt(dragStartPos).data().toString());

    mimeData->setData("application/infoCamera", QByteArray());
    drag->setMimeData(mimeData);
    drag->exec();

}
