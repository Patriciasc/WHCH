/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whch.cpp              #
 * #                                                                      #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>            #
 * #                                                                      #
 * # Copyright (C) 2010 Openismus GmbH                                    #
 * #                                                                      #
 * # Version: 0.3                                                         #
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
#include "whchDomModel.h"
#include "whchTask.h"
#include "whchClientsTasksDialog.h"
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
#include <QCloseEvent>

Whch::Whch(QWidget *parent) :
        QMainWindow(parent),
        m_ui(new Ui::whch),
        m_domModel(0),
        m_treeProxyModel(0),
        m_tableProxyModel(0),
        m_tableProxyModelHistory(0),
        m_trayIcon(0),
        m_trayIconMenu(0),
        restoreAction(0),
        quitAction(0),
        m_seconds(0),
        m_timer(0)
{
    m_ui->setupUi(this);

    setupModelView();

    setupData();

    customizeUi();
}

Whch::~Whch()
{
    saveSessionData();

    delete m_ui;
}

// ## Slots ##

// Shows all available tasks (from session and .xml file) and enables the
// user to add new ones.
void Whch::onUiComboboxItemActivated(const QString &task)
{
    if (task.compare(NEW_TASK) == 0)
    {
        m_ui->lineEdit->clear();
        on_actionTasksClients_triggered();
    }
}

// Starts timer.
void Whch::on_StartButton_clicked()
{
    m_ui->StartButton->setEnabled(false);
    m_ui->StopButton->setEnabled(true);
    m_ui->lineEdit->setEnabled(true);

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
    m_timer->start(1000);
}

// Stops timer and saves new introduced task.
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

            // Resize start and end columns.
            m_ui->tableView->resizeColumnToContents(1);
            m_ui->tableView->resizeColumnToContents(2);

            // Clear input and enable start button.
            m_ui->lineEdit->clear();
            m_ui->lineEdit->setEnabled(false);
            m_ui->StartButton->setEnabled(true);

            m_trayIcon->setToolTip("Spent time on task:\n 0 hours, 0 minutes");
        }
    }
}

void Whch::on_lineEdit_returnPressed()
{
    on_StopButton_clicked();
}

// Handler for tray icon's activation.
void Whch::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
    case QSystemTrayIcon::Trigger:
        showMessage();
        break;
    default:
        ;
    }
}

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

    // Update tooltip status.
    QString hoursText;
    QString minutesText;
    ((hours > 1) || (hours == 0)) ? hoursText = " hours," : hoursText=" hour,";
    ((minutes > 1) || (minutes == 0)) ? minutesText = " minutes" : minutesText=" minute";
    m_trayIcon->setToolTip("Spent time on task:\n"+ QString("%1").arg(hours) + hoursText + " " + QString("%1").arg(minutes) + minutesText);
}

void Whch::onItemInTreeViewClicked(const QModelIndex &index)
{
    int timeType = index.data(WhchDomModel::TimeTypeRole).toInt();
    if (timeType == WhchDomModel::DayTimeType)
    {

        QModelIndex domModelIndex = m_treeProxyModel->mapToSource(index);
        QModelIndex tableModelIndex = m_tableProxyModelHistory->mapFromSource(domModelIndex);

        m_ui->tableViewHistory->showRow(0);
        m_ui->tableViewHistory->setRootIndex(tableModelIndex);
    }
    else
    {
        m_ui->tableViewHistory->reset();
        m_ui->tableViewHistory->hideRow(0);
    }

    // Display total number of hours worked for the current selected time period.
    QString workedTime = m_domModel->workedTime(m_treeProxyModel->mapToSource(index));
    if (timeType == WhchDomModel::RootTimeType)
        m_ui->label->setText("");
    else
        m_ui->label->setText("Total worked time: " + workedTime);

    m_ui->tableViewHistory->scrollToBottom();
    m_ui->tableViewHistory->resizeColumnToContents(1);
    m_ui->tableViewHistory->resizeColumnToContents(2);
}

void Whch::onNewTasksAdded(const QStringList &tasks)
{
    m_ui->comboBox->clear();
    m_ui->comboBox->addItems(tasks);
}

void Whch::onSessionDataUpdated(const QString &client,
                                const QStringList &tasks)
{
    m_sessionData.insert(client, tasks);
}

// Sets up dialog for adding tasks and clients.
void Whch::on_actionTasksClients_triggered()
{
    WhchClientsTasksDialog *clientsTasksDialog = new WhchClientsTasksDialog(m_domModel, m_sessionData);

    connect(clientsTasksDialog, SIGNAL(newTasksAdded(QStringList)),
             this, SLOT(onNewTasksAdded(QStringList)));

    connect(clientsTasksDialog, SIGNAL(sessionDataUpdated(QString,QStringList)),
             this, SLOT(onSessionDataUpdated(QString, QStringList)));
}

void Whch::on_actionHistory_View_triggered(bool checked)
{
    if(checked)
    {
        m_ui->stackedWidget->setCurrentWidget(m_ui->pageHistory);
        m_ui->tableViewHistory->reset();

        initializeHistoryViews();

        m_ui->tableViewHistory->resizeColumnToContents(1);
        m_ui->tableViewHistory->resizeColumnToContents(2);
        m_ui->tableViewHistory->scrollToBottom();
    }
    else
    {
        m_ui->stackedWidget->setCurrentWidget(m_ui->page);
        m_ui->tableView->scrollToBottom();
    }

}

void Whch::on_actionAbout_whch_triggered()
{
    QMessageBox::about(this, tr("WHCH (Working Hours Counter for Humans)"),
                       tr("WHCH stands for '<b>Working Hours Counter for Humans</b>'. "
                          "This is an application for tracking the time spent on "
                          "individual projects."));
}

void Whch::on_actionQuit_triggered()
{
    this->close();
}

// Exports the .xml file to a wikimedia format.
void Whch::on_actionExport_to_wiki_format_triggered()
{
    QXmlQuery query(QXmlQuery::XSLT20);
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

// ## Auxiliary functions ##

void Whch::customizeUi()
{
    setTryIcon();

    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(1);
    m_ui->tableView->resizeColumnToContents(2);
    m_ui->tableView->scrollToBottom();
    m_ui->tableView->setColumnHidden(0,true);
    m_ui->tableViewHistory->setColumnHidden(0,true);
    m_ui->treeView->setHeaderHidden(true);

    m_ui->lineEdit->setEnabled(false);
    m_ui->StopButton->setEnabled(false);
    m_ui->lcdNumber->display("00:00:00");
}

void Whch::setupData()
{
    loadSessionData();
    setComboboxTasks();
}

void Whch::setupModelView()
{
    m_domModel = new WhchDomModel(XML_FILENAME, this);

    m_treeProxyModel = new WhchTreeProxyModel(this);
    m_treeProxyModel->setSourceModel(m_domModel);
    m_ui->treeView->setModel(m_treeProxyModel);

    m_tableProxyModel = new WhchTableProxyModel(this);
    m_tableProxyModel->setSourceModel(m_domModel);
    m_ui->tableView->setModel(m_tableProxyModel);
  
    m_tableProxyModelHistory = new WhchTableProxyModel(this);
    m_tableProxyModelHistory->setSourceModel(m_domModel);  
    m_ui->tableViewHistory->setModel(m_tableProxyModelHistory);

    // Connect tree view with table view.
    connect(m_ui->treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onItemInTreeViewClicked(QModelIndex)));

    // Show current day tasks.
    setCurrentDayIndex();
}

// Show current day tasks.
void Whch::setCurrentDayIndex()
{
    QModelIndex currentDay= m_tableProxyModel->mapFromSource(m_domModel->currentDayIndex());
    if (currentDay.isValid())
    {
        m_ui->tableView->showRow(0);
        m_ui->tableView->setRootIndex(currentDay);
    }
    else
        m_ui->tableView->hideRow(0);
}

// Get related client of a task.
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

// Load session data from setting's file.
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

// Saves session data into setting's file.
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

// Initialize the list of available tasks.
void  Whch::setComboboxTasks()
{
    QStringList sessionDataValues;
    QStringList tasks (m_domModel->AttributesList("name"));
    QStringList tasksClients;
    m_ui->comboBox->setInsertPolicy(QComboBox::InsertAtTop);

    // Retrieve tasks that already exist in the .xml file.
    for (int i=0; i<tasks.size(); ++i)
    {
        tasksClients << tasks.at(i) + " (" + m_domModel->xmlClientOfTask(tasks.at(i)) + ")";
    }

    // Retrieve current session tasks.
    MapQStringToList::const_iterator i = m_sessionData.constBegin();
    while (i != m_sessionData.constEnd())
    {
        QStringList currentElementTasks (i.value());
        for (int j = 0; j < currentElementTasks.size(); ++j)
            sessionDataValues << currentElementTasks.at(j) +
                    " (" + i.key() + ")";
        ++i;
    }

    // Do not repeat tasks.
    for (int i=0; i < sessionDataValues.size(); ++i)
    {
        if (tasksClients.contains(sessionDataValues.at(i)) == NULL)
            tasksClients << sessionDataValues.at(i);
    }

    // Update combobox.
    tasksClients << NEW_TASK;
    m_ui->comboBox->addItems(tasksClients);

    if (m_ui->comboBox->currentText().compare(NEW_TASK) == 0)
        m_ui->lineEdit->setEnabled(false);

    QObject::connect(m_ui->comboBox, SIGNAL(activated(QString)),
                     this, SLOT(onUiComboboxItemActivated(QString)));
}

void Whch::setCurrentTask()
{
    WhchTask currentTask;

    currentTask.m_details = m_ui->lineEdit->text();
    const QString text(m_ui->comboBox->currentText());
    currentTask.m_name = text;

    // Set task's related client.
    currentTask.m_client = m_domModel->xmlClientOfTask(text);
    if (currentTask.m_client.compare("") == 0)
        currentTask.m_client =  sessionClientOfTask(text);

    // Display new task.
    m_domModel->addNewTaskElement(currentTask);

    setCurrentDayIndex();

    m_ui->tableView->scrollToBottom();
}

// Creates menu actions for the system tray icon.
void Whch::createTrayIconMenuActions()
{
    restoreAction = new QAction(tr("&Show"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

// Creates system tray icon.
void Whch::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    //FIXME: Change this to make the icon not be part of the binary.
    QIcon icon = QIcon(":/data/icons/48x48/whch.png");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setVisible(true);
    setWindowIcon(icon);
    m_trayIcon->setToolTip("Spent time on task:\n 0 hours, 0 minutes");
}

// Sets up the tray icon.
void Whch::setTryIcon()
{
    createTrayIconMenuActions();
    createTrayIcon();
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

// Shows tray icon message.
void Whch::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    m_trayIcon->showMessage("WHCH", "Tracks the time spent on your projects.", icon, 7000);
}

// Overriden function. Leaves the app accesible from the tray icon.
void Whch::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible())
    {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Whch::initializeHistoryViews()
{
    QModelIndex currentDayIndex = m_domModel->currentDayIndex();

    if (currentDayIndex.isValid())
    {
        m_ui->tableViewHistory->showRow(0);

        // TODO: Nicer way of achieving the same effect?
        QModelIndex treeModelIndex = m_treeProxyModel->mapFromSource(currentDayIndex);
        m_ui->treeView->setExpanded(treeModelIndex.parent().parent().parent().parent(), true);
        m_ui->treeView->setExpanded(treeModelIndex.parent().parent().parent(), true);
        m_ui->treeView->setExpanded(treeModelIndex.parent().parent(), true);
        m_ui->treeView->setExpanded(treeModelIndex.parent(), true);
        
        QModelIndex tableModelIndex = m_tableProxyModelHistory->mapFromSource(currentDayIndex);
        m_ui->tableViewHistory->setRootIndex(tableModelIndex);

        QString dayWorkedHours = m_domModel->workedTime(currentDayIndex);
        m_ui->label->setText("Total worked time: " + dayWorkedHours);
    }
    else
    {
        m_ui->tableViewHistory->hideRow(0);
        m_ui->label->setText("");
    }
}
