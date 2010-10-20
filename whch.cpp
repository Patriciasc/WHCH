#include "whch.h"
#include "ui_whch.h"

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);
}

whch::~whch()
{
    delete ui;
}
