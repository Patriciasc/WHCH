#include "whch.h"
#include "ui_whch.h"
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include "whch_task.h"
#include "whch_tablemodel.h"
#include <QDate>
#include <QFileDialog>
#include <QLineEdit>

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);

    // Set model/view.
    m_model = new whch_TableModel();
    ui->tableView->setModel(m_model);
    ui->tableView->resizeRowsToContents();

    // Resize start and end columns.
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);


    // When return is pressed on the lineEdit widget:
    //Get current tasks paramenters and display new task.
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                     this, SLOT(set_current_task_parameters()));
    // Clean displayed input.
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                     ui->lineEdit, SLOT(clear()));
}

whch::~whch()
{
    delete ui;
}

void whch::set_current_task_parameters()
{
    whch_task current_task;
    current_task.details = ui->lineEdit->text();
    current_task.name = ui->comboBox->currentText();

    //Display new task.
    m_model->set_new_task(current_task);

    //Resize start and end columns
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
}

/* ------------- */
/* ACTION SLOTS. */
/* ------------- */

void whch::on_actionQuit_triggered()
{
    this->close();
}

void whch::on_actionTasks_triggered()
{
    std::cout << "new task" << std::endl;
}

void whch::on_actionAbout_whch_triggered()
{
    return QMessageBox::aboutQt(this,"WHCH (Working hours counter for humans");
}
