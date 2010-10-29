#include "whch.h"
#include "ui_whch.h"
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include "whch_task.h"
#include "whch_tablemodel.h"
#include <QDate>

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);
    /* Set model/view. */
    m_model = new whch_TableModel();
    ui->tableView->setModel(m_model);
    ui->tableView->resizeRowsToContents();
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                     m_model, SLOT(set_new_task()));
}

whch::~whch()
{
    delete ui;
}
