#include "modernpushbutton.h"

modernPushButton::modernPushButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(slot_clickByButton()));
}

void modernPushButton::slot_clickByButton()
{
    emit clickByButton(this->objectName());
}
