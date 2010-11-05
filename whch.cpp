#include "whch.h"
#include "ui_whch.h"
#include "ui_dialogTasksClients.h"
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include "whchTask.h"
#include "whchTableModel.h"
#include <QDate>
#include <QFileDialog>
#include <QLineEdit>

Whch::Whch(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::whch)
{
    // Set GUI.
    m_ui->setupUi(this);

    // Set model/view.
    m_model = new WhchTableModel();
    m_ui->tableView->setModel(m_model);

    // Resize start/end columns and rows.
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);


    // When return is pressed on the lineEdit widget:
    // Get current tasks paramenters and display new task.
    QObject::connect(m_ui->lineEdit, SIGNAL(returnPressed()),
                     this, SLOT(setCurrentTaskParameters()));
    // Clean displayed input.
    QObject::connect(m_ui->lineEdit, SIGNAL(returnPressed()),
                     this, SLOT(onLineEditReturn()));
}

Whch::~Whch()
{
    delete m_ui;
}

/* ----- */
/* SLOTS */
/* ----- */

void Whch::setCurrentTaskParameters()
{
    WhchTask currentTask;
    currentTask.details = m_ui->lineEdit->text();
    currentTask.name = m_ui->comboBox->currentText();

    //Display new task.
    m_model->setNewTask(currentTask);

    //Resize start and end columns
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);
}

void Whch::onLineEditReturn()
{
    m_ui->lineEdit->clear();
}

void Whch::showClientTasksInTable(const QString &client)
{
    QStringList clientsTasks = m_model->getClientTasks(client);

    // Set number of rows.
    m_uiDialog->tableWidget->setRowCount(clientsTasks.size());

    for (int i=0; i< clientsTasks.size(); ++i)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(clientsTasks.at(i));
        m_uiDialog->tableWidget->setItem(i, 0, newItem);
    }
}

/* ------------- */
/* ACTION SLOTS. */
/* ------------- */

void Whch::on_actionQuit_triggered()
{
    this->close();
}

void Whch::on_actionTasks_triggered()
{
    m_uiDialog = new Ui::Dialog;
    QDialog *taskDialog = new QDialog;

    m_uiDialog->setupUi(taskDialog);

    // Load list of clients.
    QStringList clients = m_model->getClientsList();
    m_uiDialog->comboBox->addItems(clients);

    // Load list of related tasks to the client.
    QObject::connect(m_uiDialog->comboBox,SIGNAL(activated(QString)),
                     this, SLOT(showClientTasksInTable(QString)));

    taskDialog->show();
}

void Whch::on_actionAbout_whch_triggered()
{
    return QMessageBox::aboutQt(this,"WHCH (Working hours counter for humans");
}
