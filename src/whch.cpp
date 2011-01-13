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
#include "whchTask.h"
#include "whchTableModel.h"
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QTimer>
#include <QComboBox>
#include <QtXmlPatterns>
#include <QSettings>
#include <QDebug>

static const QString NEW_CLIENT(QObject::tr("Add new client"));
static const QString NEW_TASK(QObject::tr("Add new task"));
static const QString WARNING(QObject::tr("Soo... What have you been doing in the last minutes?"));

Whch::Whch(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::whch),
    m_uiDialog(new Ui::Dialog),
    m_seconds(0),
    m_timer(new QTimer)
{
    // Set GUI.
    m_ui->setupUi(this);

    // Set model/view.
    m_model = new WhchTableModel();
    m_ui->tableView->setModel(m_model);

    // Load session data.
    loadSessionData();

    // Set list of available tasks.
    setComboboxTasks();

    // Resize start/end columns and rows.
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);

    m_ui->lineEdit->setEnabled(false);
    m_ui->StopButton->setEnabled(false);
    m_ui->lcdNumber->display("00:00:00");
}

Whch::~Whch()
{
    /* Save session data. */
    saveSessionData();

    delete m_ui;
    delete m_uiDialog;
}

/* ------------------- */
/* AUXILIARY FUNCTIONS */
/* ------------------- */

void Whch::onTimerTimeOut()
{
    int seconds = ++m_seconds;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;

    QTime *time = new QTime(hours,minutes,seconds);
    QString text = time->toString("HH:mm:ss");

    m_ui->lcdNumber->display(text);
}

/* List of new added tasks. */
QStringList Whch::sessionTasks()
{
    QStringList sessionTasks;
    MapQStringToList::const_iterator i = m_sessionData.constBegin();

    while (i != m_sessionData.constEnd())
    {
        QStringList itemTasks = i.value();
        for (int j = 0; j < itemTasks.size(); ++j)
        {
            // Do not repeat elements.
            const QString item(itemTasks.at(j));
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
    MapQStringToList::const_iterator i = m_sessionData.constBegin();
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

QStringList Whch::sessionClientTasks(const QString &client)
{
    return m_sessionData[client];
}

bool Whch::isSessionClient(const QString &client)
{
    if (m_sessionData.contains(client))
        return true;
    return false;
}

// Return a list of the total tasks of a client. Those saved
// in the .xml file and those that are session data.
QStringList Whch::clientTotalTasks(const QString &client)
{
    QStringList clientTasks;

    if (m_model->isClient(client))
        clientTasks << m_model->ClientTasks(client);
    if (m_sessionData.contains(client))
    {
        QStringList sessionClientTasks(m_sessionData.value(client));
        for (int i = 0; i < sessionClientTasks.size(); ++i)
        {
            const QString sessionTaskItem(sessionClientTasks.at(i));
            if (!clientTasks.contains(sessionTaskItem))
                clientTasks << sessionTaskItem;
        }
    }
    return clientTasks;
}

// Return total tasks (session and .xml file ones).
QStringList Whch::totalTasks()
{
    QStringList totalTasks (m_model->AttributesList("name"));
    QStringList newTasks (sessionTasks());

    for (int i = 0; i < newTasks.count(); ++i)
    {
        const QString sessionTaskItem(newTasks.at(i));
        if (!totalTasks.contains(sessionTaskItem))
            totalTasks << sessionTaskItem;
    }
    return totalTasks;
}

/* ----- */
/* SLOTS */
/* ----- */

// Shows the related tasks of the selected client.
void Whch::onDialogComboboxItemActivated(const QString &client)
{
    // Make it editable for adding a new client.
    if (client.compare(NEW_CLIENT) == 0)
    {
        m_uiDialog->comboBox->setEditable(true);
        m_uiDialog->comboBox->clearEditText();
        QObject::connect(m_uiDialog->comboBox->lineEdit(), SIGNAL(returnPressed()),
                         this, SLOT(onDialogComboboxLineEditReturn()));
    }
    else
        m_uiDialog->comboBox->setEditable(false);

    // Set current available tasks and new line for new task.
    QStringList clientTasks(m_model->ClientTasks(client));
    if (m_sessionData.contains(client))
    {
        QStringList sessionClientTasks(m_sessionData.value(client));
        for (int i = 0; i < sessionClientTasks.size(); ++i)
        {
            const QString sessionTaskItem(sessionClientTasks.at(i));
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
void Whch::onDialogComboboxLineEditReturn()
{
    QString newClient = m_uiDialog->comboBox->currentText();

    if (newClient.compare("") != 0)
    {
        QStringList clients(m_model->AttributesList("client"));

        if (!clients.contains(newClient))
        {
            QStringList tasks;
            m_sessionData.insert(newClient, tasks);
        }
    }
}

// Shows all available tasks (from session and .xml file) and enables the
// user to add new ones.
void Whch::onUiComboboxItemActivated(const QString &task)
{
    if (task.compare(NEW_TASK) == 0)
    {
        // Displays "Add tasks/clients" dialog.
        m_ui->lineEdit->clear();
        //m_ui->lineEdit->setEnabled(false);
        on_actionTasks_triggered();
    }
}

// Makes last row editable.
void Whch::onDialogTableCellChanged(QTableWidgetItem *item)
{
    if ((item->column() == 0) && (item->row() == m_uiDialog->tableWidget->rowCount()-1))
        m_uiDialog->tableWidget->editItem(item);
}

// Save user's new task into the session's structure.
void Whch::onDialogTableItemChanged(QTableWidgetItem *item)
{
    const QString itemText (item->text());
    if (m_uiDialog->comboBox->currentText().compare("") != 0)
    {
        if (itemText.compare("") != 0)
        {
            // Update task's combobox.
            if (!totalTasks().contains(itemText))
            {
                m_ui->comboBox->clear();
                QStringList tasks;
                tasks << itemText << totalTasks() <<  NEW_TASK;
                m_ui->comboBox->addItems(tasks);
            }

            // Save data into session structure.
            const QString currentClient(m_uiDialog->comboBox->currentText());

            // Do not repeat tasks.
            if(!clientTotalTasks(currentClient).contains(itemText))
            {
                // FIXME: Commented code is for testing still.
                // This code should give feedback to the user
                // when introducing an already existent task.
                // Still not working.
                //std::cout << "if" << std::endl;

                // Change color back to white (for repeated elements).
                /* if (item->backgroundColor() != Qt::white)
            {
                const QColor warningColor (Qt::white);
                item->setBackgroundColor(warningColor);
            }*/

                // Add new task as session data.
                QStringList tasks(m_sessionData.value(currentClient));
                tasks << itemText;
                m_sessionData.insert(currentClient, tasks);

                // Insert new empty element.
                m_uiDialog->tableWidget->setRowCount(clientTotalTasks(currentClient).count()+1);
                QTableWidgetItem *newItem = new QTableWidgetItem("");
                m_uiDialog->tableWidget->setItem(clientTotalTasks(currentClient).count(), 0, newItem);
            }
            /*else
        {
            // Give feedback to the user on repeated elements.
            std::cout << "else" << std::endl;
            QColor warningColor;
            warningColor.setRed(200);
            item->setBackgroundColor(warningColor);
            item->setSelected(false);
        }*/
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
            const QString sessionClientItem(sessionClients.at(i));
            if(!clients.contains(sessionClientItem))
                clients << sessionClientItem;
        }
    }

    clients << NEW_CLIENT;
    m_uiDialog->comboBox->addItems(clients);

    // Load list of tasks for initial client
    const QString currentClient (m_uiDialog->comboBox->currentText());
    if (currentClient.compare(NEW_CLIENT) != 0)
        onDialogComboboxItemActivated(currentClient);

    // Load list of related tasks to the selected client.
    QObject::connect(m_uiDialog->comboBox, SIGNAL(activated(QString)),
                     this, SLOT(onDialogComboboxItemActivated(QString)));

    // Makes last row editable.
    QObject::connect(m_uiDialog->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
                     this, SLOT(onDialogTableCellChanged(QTableWidgetItem *)));

    // Manages data if any cell of the last row is edited.
    QObject::connect(m_uiDialog->tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)),
                     this, SLOT(onDialogTableItemChanged(QTableWidgetItem *)));

    taskDialog->show();
}

void Whch::on_actionAbout_whch_triggered()
{
    QMessageBox::about(this, tr("WHCH (Working Hours Counter for Humans)"),
                 tr("WHCH stands for '<b>Working Hours Counter for Humans</b>'. "
                    "This is an application for tracking the time spent on "
                    "individual projects."));
}

/* Exports the .xml file to a wikimedia format. */
void Whch::on_actionExport_to_wiki_format_triggered()
{
    QXmlQuery query(QXmlQuery::XSLT20);
    /* FIXME: Look for right installation path. */
    query.setFocus(QUrl(QDir::homePath() + "/" + "." + "whch_log.xml"));
    query.setQuery(QUrl("qrc:/whch_log.xslt"));

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export To Mediawiki Format"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            query.evaluateTo(&file);
            setWindowFilePath(fileName);
        }
        file.close();
    }
}

/* Load session data from setting's file. */
void  Whch::loadSessionData()
{
    QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("whch");
    QSettings settings;
    QStringList clients = settings.value("clients").toStringList();

    if (!clients.isEmpty())
    {
        for (int i = 0; i < clients.size(); ++i)
        {
            QStringList tasks = settings.value(clients[i]).toStringList();
            m_sessionData.insert(clients[i], tasks);
        }
    }

}

/* Saves session data into setting's file. */
void  Whch::saveSessionData()
{
    QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("whch");
    QSettings settings;

    MapQStringToList::const_iterator i = m_sessionData.constBegin();
    QStringList clientsList;
    while (i != m_sessionData.constEnd())
    {
        clientsList << i.key();
        settings.setValue(i.key(), i.value());
        ++i;
    }
    settings.setValue("clients", clientsList);
}

void  Whch::setComboboxTasks()
{
    QStringList sessionDataValues;
    QStringList tasks (m_model->AttributesList("name"));
    m_ui->comboBox->setInsertPolicy(QComboBox::InsertAtTop);
    MapQStringToList::const_iterator i = m_sessionData.constBegin();

    while (i != m_sessionData.constEnd())
    {
        sessionDataValues << i.value();
        ++i;
    }

    for (int i=0; i < sessionDataValues.size(); ++i)
    {
        if (tasks.contains(sessionDataValues.at(i)) == NULL)
            tasks << sessionDataValues.at(i);
    }

    tasks << NEW_TASK;
    m_ui->comboBox->addItems(tasks);

    if (m_ui->comboBox->currentText().compare(NEW_TASK) == 0)
        m_ui->lineEdit->setEnabled(false);

    QObject::connect(m_ui->comboBox, SIGNAL(activated(QString)),
              this, SLOT(onUiComboboxItemActivated(QString)));
}

/* Starts timer. */
void Whch::on_StartButton_clicked()
{
    m_ui->StartButton->setEnabled(false);
    m_ui->StopButton->setEnabled(true);
    m_ui->lineEdit->setEnabled(true);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
    m_timer->start(1000);
    onTimerTimeOut();
}

/* Stops timer and saves new introduced task. */
void Whch::on_StopButton_clicked()
{
    const QString lineEditText(m_ui->lineEdit->text());

    // Do not admit empty detail's fields.
    if (lineEditText.isEmpty() || lineEditText.compare(WARNING) == 0)
        m_ui->lineEdit->setText(WARNING);
    else
    {
        if (m_ui->comboBox->currentText().compare(NEW_TASK) == 0)
            m_ui->lineEdit->setText("Select a task first.");
        else
        {
            m_timer->stop();
            m_seconds = 0;
            m_ui->lcdNumber->display("00:00:00");
            m_ui->StopButton->setEnabled(false);

            setCurrentTask();

            //Resize start and end columns
            m_ui->tableView->resizeColumnToContents(0);
            m_ui->tableView->resizeColumnToContents(1);

            // Clear input and enable start button.
            m_ui->lineEdit->clear();
            m_ui->lineEdit->setEnabled(false);
            m_ui->StartButton->setEnabled(true);
        }
    }
}

void Whch::setCurrentTask()
{
    WhchTask currentTask;

    currentTask.m_details = m_ui->lineEdit->text();
    const QString text(m_ui->comboBox->currentText());
    currentTask.m_name = text;

    // Set task's related client.
    currentTask.m_client = m_model->clientOfTask(text);
    if (currentTask.m_client.compare("") == 0)
        currentTask.m_client =  sessionClientOfTask(text);

    //Display new task.
    m_model->setNewTask(currentTask);
}

void Whch::on_lineEdit_returnPressed()
{
    on_StopButton_clicked();
}
