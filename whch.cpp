/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whch.cpp              #
 * #                                                                      #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>            #
 * #                                                                      #
 * # Copyright (C) 2010 Openismus GmbH                                    #
 * #                                                                      #
 * # Version: 0.1                                                         #
 * #                                                                      #
 * # Description: Application for tracking the time spent on individual   #
 * # projects.                                                            #
 * #                                                                      #
 * # WHCH is free software; you can redistribute it and/or modify         #
 * # it under the terms of the GNU General Public License as published by #
 * # the Free Software Foundation; either version 3 of the License,       #
 * # or (at your option) any later version.                               #
 * #                                                                      #
 * # WHCH is distributed in the hope that it will be useful,              #
 * # but WITHOUT ANY WARRANTY; without even the implied warranty of       #
 * # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    #
 * # General Public License for more details.                             #
 * #                                                                      #
 * # You should have received a copy of the GNU General Public License    #
 * # along with WHCH. If not, see <http://www.gnu.org/licenses/>.         #
 * ########################################################################
*/

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

static const QString NEW_CLIENT = "Add new client";

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
    QStringList tasks = m_model->AttributesList("name");
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

/* ------------------- */
/* AUXILIARY FUNCTIONS */
/* ------------------- */

/* List of new added tasks. */
QStringList Whch::sessionTasks()
{
    QStringList sessionTasks;
    QMap<QString, QStringList>::const_iterator i = m_sessionData.constBegin();

    while (i != m_sessionData.constEnd())
    {
        QStringList itemTasks = i.value();
        for (int j = 0; j < itemTasks.size(); ++j)
        {
            // Do not repeat elements.
            QString item(itemTasks.at(j));
            if (!sessionTasks.contains(item))
                sessionTasks << item;
        }
        ++i;
    }
    return sessionTasks;
}

/* Get related client of a task. */
QString Whch::sessionClientOfTask(const QString &task)
{
    QMap<QString, QStringList>::const_iterator i = m_sessionData.constBegin();
    QString sessionClient;

    while (i != m_sessionData.constEnd())
    {
        QStringList itemTasks = i.value();
        for (int j = 0; j < itemTasks.size(); ++j)
        {
            if(task.contains(itemTasks.at(j)))
                return sessionClient = i.key();
        }
        ++i;
    }
    return sessionClient;
}

/* ----- */
/* SLOTS */
/* ----- */

// Sets a new task with current input parameters.
void Whch::onLineEditReturn()
{
    WhchTask currentTask;
    currentTask.m_details = m_ui->lineEdit->text();
    QString text(m_ui->comboBox->currentText());
    currentTask.m_name = text;

    // The user did not add new tasks or clients.
    if(m_sessionData.isEmpty())
        currentTask.m_client = m_model->clientOfTask(text);
    else
        currentTask.m_client = sessionClientOfTask(text);

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
    if (client.compare(NEW_CLIENT) == 0)
    {
        m_uiDialog->comboBox->setEditable(true);
        m_uiDialog->comboBox->clearEditText();
        QLineEdit *lineEdit = m_uiDialog->comboBox->lineEdit();
        QObject::connect(lineEdit, SIGNAL(returnPressed()),
                         this, SLOT(onDialogLineEditReturn()));
    }

    // Set current available tasks and new line for new task.
    QStringList clientTasks(m_model->ClientTasks(client));
    if (m_sessionData.contains(client))
    {
        QStringList sessionClientTasks(m_sessionData.value(client));
        for (int i = 0; i < sessionClientTasks.size(); ++i)
        {
            QString sessionTaskItem(sessionClientTasks.at(i));
            if (!clientTasks.contains(sessionTaskItem))
                clientTasks << sessionTaskItem;
        }
    }
    clientTasks << "";

    // Set number of rows.
    m_uiDialog->tableWidget->setRowCount(clientTasks.size());

    // Show list of tasks.
    for (int i=0; i< clientTasks.size(); ++i)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(clientTasks.at(i));
        m_uiDialog->tableWidget->setItem(i, 0, newItem);
    }
}

// Update session data.
void Whch::onDialogLineEditReturn()
{
    //Save new client in list of current session clients.
    QStringList clients(m_model->AttributesList("client"));
    QString newClient = m_uiDialog->comboBox->lineEdit()->text();
    if (!clients.contains(newClient))
    {
        QStringList tasks;
        m_sessionData.insert(newClient, tasks);
    }
        m_uiDialog->comboBox->setEditable(false);
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
    QString itemText (item->text());
    if (itemText.compare("") != 0)
    {
        // Update task's combobox.
        if((!sessionTasks().contains(itemText)) && (!m_model->AttributesList("name").contains(itemText)))
            m_ui->comboBox->addItem(itemText);

        // Save data into session structure.
        QString currentClient(m_uiDialog->comboBox->currentText());
        QStringList tasks(m_sessionData.value(currentClient));

        // Do not repeat tasks.
        if(!tasks.contains(itemText))
        {
            tasks << itemText;
            m_sessionData.insert(currentClient, tasks);
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

// Sets up dialog for adding tasks and clients.
void Whch::on_actionTasks_triggered()
{
    QDialog *taskDialog = new QDialog;
    m_uiDialog->setupUi(taskDialog);

    // Insert new added clients always at the top.
    m_uiDialog->comboBox->setInsertPolicy(QComboBox::InsertAtTop);

    // Load list of clients.
    QStringList clients(m_model->AttributesList("client"));
    if (!m_sessionData.isEmpty())
    {
        QStringList sessionClients (m_sessionData.keys());
        for (int i=0; i < sessionClients.size(); ++i)
        {
            QString sessionClientItem(sessionClients.at(i));
            if(!clients.contains(sessionClientItem))
                clients << sessionClientItem;
        }
    }
    clients << NEW_CLIENT;
    m_uiDialog->comboBox->addItems(clients);

    // Load list of tasks for initial client
    QString currentClient (m_uiDialog->comboBox->currentText());
    if (currentClient.compare(NEW_CLIENT) != 0)
        onDialogComboboxItemActivated(currentClient);

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

void Whch::on_actionAbout_whch_triggered()
{
    return QMessageBox::aboutQt(this,"WHCH (Working hours counter for humans");
}
