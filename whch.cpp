#include "whch.h"
#include "ui_whch.h"
#include "ui_new_task_dialog.h"
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
    m_ui(new Ui::whch)
{
    // Set GUI.
    m_ui->setupUi(this);

    // Set model/view.
    m_model = new whch_TableModel();
    m_ui->tableView->setModel(m_model);

    // Resize start/end columns and rows.
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);


    // When return is pressed on the lineEdit widget:
    // Get current tasks paramenters and display new task.
    QObject::connect(m_ui->lineEdit, SIGNAL(returnPressed()),
                     this, SLOT(set_current_task_parameters()));
    // Clean displayed input.
    QObject::connect(m_ui->lineEdit, SIGNAL(returnPressed()),
                     this, SLOT(on_line_edit_return()));
}

whch::~whch()
{
    delete m_ui;
    delete m_ui_dialog;
}

/* ------------- */
/* --- SLOTS.--- */
/* ------------- */

void whch::set_current_task_parameters()
{
    whch_task current_task;
    current_task.details = m_ui->lineEdit->text();
    current_task.name = m_ui->comboBox->currentText();

    //Display new task.
    m_model->set_new_task(current_task);

    //Resize start and end columns
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);
}

void whch::on_line_edit_return()
{
    m_ui->lineEdit->clear();
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
    m_ui_dialog = new Ui::Dialog;
    std::cout << "new task" << std::endl;
    QDialog *task_dialog = new QDialog;
    //FIXME: Need to make the dialog child of the window.
    m_ui_dialog->setupUi(task_dialog);
    task_dialog->show();
}

void whch::on_actionAbout_whch_triggered()
{
    return QMessageBox::aboutQt(this,"WHCH (Working hours counter for humans");
}
