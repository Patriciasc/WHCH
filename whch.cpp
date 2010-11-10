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
#include <QDebug>
#include <QStandardItemModel>

Whch::Whch(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::whch),
    m_uiDialog(new Ui::Dialog)
{
    // Set GUI.
    m_ui->setupUi(this);

    // Set model/view.
    m_model = new WhchTableModel();
    m_ui->tableView->setModel(m_model);

    // Set list of available tasks.
    QStringList tasks = m_model->getAttributesList("name");
    m_ui->comboBox->addItems(tasks);

    // Resize start/end columns and rows.
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);

    // Get current tasks paramenters and display new task.
    // Clean displayed input.
    QObject::connect(m_ui->lineEdit, SIGNAL(returnPressed()),
              this, SLOT(onLineEditReturn()));
}

Whch::~Whch()
{
    delete m_ui;
    delete m_uiDialog;
}

/* ----- */
/* SLOTS */
/* ----- */

// Sets a new task with current input parameters.
void Whch::onLineEditReturn()
{
    WhchTask currentTask;
    currentTask.m_details = m_ui->lineEdit->text();
    currentTask.m_name = m_ui->comboBox->currentText();

    //Display new task.
    m_model->setNewTask(currentTask);

    //Resize start and end columns
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);

    // Clear input.
    m_ui->lineEdit->clear();
}

// Shows the related tasks of the selected client.
void Whch::onDialogComboboxItemActivated(const QString &client)
{
    // Make it editable for adding a new client.
    if (client.compare("Add new client") == 0)
    {
        m_uiDialog->comboBox->setEditable(true);
        m_uiDialog->comboBox->clearEditText();
        QLineEdit *lineEdit = m_uiDialog->comboBox->lineEdit();
        QObject::connect(lineEdit, SIGNAL(returnPressed()),
                         this, SLOT(onDialogLineEditReturn()));
    }

    // Set current available tasks and new line for new task.
    QStringList clientTasks = m_model->getClientTasks(client);
    clientTasks << m_SessionData.value(client) << "";

    for (int i=0; i<clientTasks.size(); ++i)
        qDebug() << "Task: " << clientTasks.at(i) << "\n";

    // Set number of rows.
    m_uiDialog->tableWidget->setRowCount(clientTasks.size());

    // Show list of tasks.
    for (int i=0; i< clientTasks.size(); ++i)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(clientTasks.at(i));
        m_uiDialog->tableWidget->setItem(i, 0, newItem);
    }
}

// Makes last row editable.
void Whch::onDialogTableCellChanged(QTableWidgetItem* item)
{
    if ((item->column() == 0) && (item->row() == m_uiDialog->tableWidget->rowCount()-1))
        m_uiDialog->tableWidget->editItem(item);
}

// Save user's new task into the session's structure.
void Whch::onDialogTableItemChanged(QTableWidgetItem* item)
{
    if (item->text().compare("") != 0)
    {
        std::cout << "in" << std::endl;
        QString currentClient(m_uiDialog->comboBox->currentText());
        QStringList tasks(m_SessionData.value(currentClient));

        // Do not repeat tasks.
        if(!tasks.contains(item->text()))
        {
            qDebug() << "added task: " << item->text();
            tasks << item->text();
            m_SessionData.insert(currentClient, tasks);
        }
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
    QDialog *taskDialog = new QDialog;
    m_uiDialog->setupUi(taskDialog);

    // Insert always at the top.
    m_uiDialog->comboBox->setInsertPolicy(QComboBox::InsertAtTop);

    // Load list of clients.
    QStringList clients = m_model->getAttributesList("client");
    clients << m_SessionData.keys() << "Add new client";
    m_uiDialog->comboBox->addItems(clients);

    // Load list of tasks for initial client
    /*onDialogComboboxItemActivated(m_uiDialog->comboBox->currentText());*/

    // Load list of related tasks to the selected client.
    QObject::connect(m_uiDialog->comboBox, SIGNAL(activated(QString)),
                     this, SLOT(onDialogComboboxItemActivated(QString)));

    // Makes last row editable.
    QObject::connect(m_uiDialog->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
                     this, SLOT(onDialogTableCellChanged(QTableWidgetItem*)));

    // Manages data if any cell of the last row is edited.
    QObject::connect(m_uiDialog->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
                     this, SLOT(onDialogTableItemChanged(QTableWidgetItem*)));

    taskDialog->show();
}

// Update session data.
void Whch::onDialogLineEditReturn()
{
    //Save new client in list of current session clients.
    QString newClient = m_uiDialog->comboBox->lineEdit()->text();
    QStringList tasks;
    m_SessionData.insert(newClient, tasks);

    m_uiDialog->comboBox->lineEdit()->clear();
    m_uiDialog->comboBox->setEditable(false);
}

void Whch::on_actionAbout_whch_triggered()
{
    return QMessageBox::aboutQt(this,"WHCH (Working hours counter for humans");
}
