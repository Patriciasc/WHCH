#include "whch.h"
#include "ui_whch.h"
#include <QtUiTools>

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    /* Model test. */
    /* This is now showing in 2 windows. I need to make the table
       be shown in the app's TableView declared in the .ui file.*/
    /* I have done some tests but still did not find the right way.*/
    QList<QString> mylist;
    mylist << "one" << "two" << "three";

    whch_TableModel *model = new whch_TableModel(mylist,this);
    QTableView *view = new QTableView;
    view->setModel(model);

    view->show();

    ui->setupUi(this);
}

whch::~whch()
{
    delete ui;
}
