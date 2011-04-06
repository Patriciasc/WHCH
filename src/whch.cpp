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

static const QString NEW_CLIENT(QObject::tr("Add new client"));
static const QString NEW_TASK(QObject::tr("Add new task"));
static const QString WARNING(QObject::tr("Soo... What have you been doing in the last minutes?"));

Whch::Whch(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::whch),
    m_uiDialog(new Ui::Dialog),
    m_seconds(0)
{
    // Set GUI.
    m_ui->setupUi(this);

    // Set model/view.

    // Populate models and display views.
    m_domModel = new WhchDomModel(this);

    m_treeProxyModel = new WhchTreeProxyModel(this);
    m_treeProxyModel->setSourceModel(m_domModel);
    m_ui->treeView->setModel(m_treeProxyModel);

    m_tableProxyModel = new WhchTableProxyModel(this);
    m_tableProxyModel->setSourceModel(m_domModel);
    m_ui->tableView_2->setModel(m_tableProxyModel);
    m_ui->tableView->setModel(m_tableProxyModel);

    // Set current day view.
    QModelIndex currentDay= m_tableProxyModel->mapFromSource(m_domModel->currentDayIndex());
    if (currentDay.isValid())
        m_ui->tableView->setRootIndex(currentDay);
    else
        m_ui->tableView->hideRow(0);

    //Connect tree view with table view.
    connect(m_ui->treeView, SIGNAL(clicked(QModelIndex)),
            m_treeProxyModel, SLOT(onItemClicked(QModelIndex)));

    connect(m_treeProxyModel, SIGNAL(clicked(QModelIndex)),
            m_tableProxyModel, SLOT(onItemClicked(QModelIndex)));

    connect(m_tableProxyModel, SIGNAL(retrieve_children(QModelIndex)),
            this, SLOT(onClickedViewIndex(QModelIndex)));

    //Print tree.
    m_domModel->printModelIndexTree();

    // Set system try icon.
    setTryIcon();

    // Load session data.

    // Set list of available tasks.

    // Rearrange table view.
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->resizeColumnToContents(0);
    m_ui->tableView->resizeColumnToContents(1);
    m_ui->tableView->scrollToBottom();
    m_ui->tableView->setColumnHidden(0,true);
    m_ui->tableView_2->setColumnHidden(0,true);

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

    // Update tooltip status.
    QString hoursText;
    QString minutesText;
    ((hours > 1) || (hours == 0)) ? hoursText = " hours," : hoursText=" hour,";
    ((minutes > 1) || (minutes == 0)) ? minutesText = " minutes" : minutesText=" minute";
    m_trayIcon->setToolTip("Spent time on task:\n"+ QString("%1").arg(hours) + hoursText + " " + QString("%1").arg(minutes) + minutesText);
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

// Return the tasks of a session client.
QStringList Whch::sessionClientTasks(const QString &client)
{
    return m_sessionData[client];
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
    QIcon icon = QIcon(":/data/icons/16x16/whch.png");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setVisible(true);
    setWindowIcon(icon);
    m_trayIcon->setToolTip("Spent time on task:\n 0 hours, 0 minutes");
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

/* ----- */
/* SLOTS */
/* ----- */

// Shows all available tasks (from session and .xml file) and enables the
// user to add new ones.
void Whch::onUiComboboxItemActivated(const QString &task)
{
    if (task.compare(NEW_TASK) == 0)
    {
        // Displays "Add tasks/clients" dialog.
        m_ui->lineEdit->clear();
        //m_ui->lineEdit->setEnabled(false);
        //on_actionTasks_triggered();
    }
}

// Makes last row editable.
void Whch::onDialogTableCellChanged(QTableWidgetItem *item)
{
    if ((item->column() == 0) && (item->row() == m_uiDialog->tableWidget->rowCount()-1))
        m_uiDialog->tableWidget->editItem(item);
}

void Whch::onClickedViewIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        m_ui->tableView_2->showRow(0);
        m_ui->tableView_2->setRootIndex(index);
    }
    else
    {
        m_ui->tableView_2->reset();
        m_ui->tableView_2->hideRow(0);
    }
}

/* ------------- */
/* ACTION SLOTS. */
/* ------------- */

void Whch::on_actionQuit_triggered()
{
    this->close();
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

/* Starts timer. */
void Whch::on_StartButton_clicked()
{
    m_ui->StartButton->setEnabled(false);
    m_ui->StopButton->setEnabled(true);
    m_ui->lineEdit->setEnabled(true);

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
    m_timer->start(1000);
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
            //m_domModel->printModelIndexTree();
            //TODO: reload data in model to update
            //the tree and table view.
            //m_domModel->loadXmlFile("whch_log.xml");

            //Resize start and end columns
            m_ui->tableView->resizeColumnToContents(0);
            m_ui->tableView->resizeColumnToContents(1);

            // Clear input and enable start button.
            m_ui->lineEdit->clear();
            m_ui->lineEdit->setEnabled(false);
            m_ui->StartButton->setEnabled(true);

            m_trayIcon->setToolTip("Spent time on task:\n 0 hours, 0 minutes");
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
    /*currentTask.m_client = m_model->clientOfTask(text);
    if (currentTask.m_client.compare("") == 0)
        currentTask.m_client =  sessionClientOfTask(text);*/

    //Display new task.
    m_domModel->addNewTaskElement(currentTask);
    QModelIndex currentDay= m_tableProxyModel->mapFromSource(m_domModel->currentDayIndex());
    if (currentDay.isValid())
    {
        m_ui->tableView->showRow(0);
        m_ui->tableView->setRootIndex(currentDay);
    }
    else
        m_ui->tableView->hideRow(0);

}

void Whch::on_lineEdit_returnPressed()
{
    on_StopButton_clicked();
}

void Whch::on_actionHistory_View_triggered(bool checked)
{

    if(checked)
    {
        m_ui->tableView_2->reset();
        m_ui->tableView_2->hideRow(0);
        m_ui->stackedWidget->setCurrentWidget(m_ui->page_2);
    }
    else
    {
        m_ui->stackedWidget->setCurrentWidget(m_ui->page);
        m_ui->tableView->scrollToBottom();
    }
}
