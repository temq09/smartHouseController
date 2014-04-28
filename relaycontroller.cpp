#include "relaycontroller.h"
#include "ui_relaycontroller.h"

relayController::relayController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::relayController)
{
    ui->setupUi(this);
}

relayController::~relayController()
{
    delete ui;
}
