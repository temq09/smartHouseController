#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <QWidget>

namespace Ui {
class relayController;
}

class relayController : public QWidget
{
    Q_OBJECT

public:
    explicit relayController(QWidget *parent = 0);
    ~relayController();

private:
    Ui::relayController *ui;
};

#endif // RELAYCONTROLLER_H
