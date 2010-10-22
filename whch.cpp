#include "whch.h"
#include "ui_whch.h"

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);

    /* Set model/view with test data. */
    QList<QString> mylist;
    mylist << "one" << "two" << "three";

    m_model = new whch_TableModel(mylist);
    ui->tableView->setModel(m_model);


}

whch::~whch()
{
    delete ui;
}
