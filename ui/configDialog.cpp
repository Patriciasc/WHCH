#include "configDialog.h"
#include "ui_configDialog.h"

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
}

configDialog::~configDialog()
{
    delete ui;
}
