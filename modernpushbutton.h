#ifndef MODERNPUSHBUTTON_H
#define MODERNPUSHBUTTON_H

#include <QPushButton>

class modernPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit modernPushButton(QWidget *parent = 0);

signals:
    void clickByButton(QString buttonName);

public slots:
    void slot_clickByButton();

};

#endif // MODERNPUSHBUTTON_H
