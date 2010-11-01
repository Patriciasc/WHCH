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
                     ui->lineEdit, SLOT(clear()));

    QObject::connect(ui->lineEdit, SIGNAL(textChanged(QString)),
                     m_model, SLOT(get_details_input(QString)));

    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                     m_model, SLOT(set_new_task()));
}

whch::~whch()
{
    delete ui;
}

void whch::on_actionNew_file_triggered()
{
    std::cout << "new file action" << std::endl;
}

void whch::on_actionOpen_file_triggered()
{
    std::cout << "open file action" << std::endl;
}

void whch::on_actionQuit_triggered()
{
    std::cout << "quit action" << std::endl;
}

void whch::on_actionTasks_triggered()
{
    std::cout << "new task" << std::endl;
}

void whch::on_actionAbout_whch_triggered()
{
    std::cout << "about dialog" << std::endl;
}
