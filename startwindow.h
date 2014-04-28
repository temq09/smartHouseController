#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

namespace Ui {
class startWindow;
}

class startWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit startWindow(QWidget *parent = 0);
    ~startWindow();

private:
    Ui::startWindow *ui;
};

#endif // STARTWINDOW_H
