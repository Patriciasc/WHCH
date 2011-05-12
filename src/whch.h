/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whch.h                #
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

#ifndef WHCH_H
#define WHCH_H

#include "whchDomModel.h"
#include "whchTreeProxyModel.h"
#include "whchTableProxyModel.h"
#include <QMainWindow>
#include <QCoreApplication>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QDialog>
#include <QTableWidgetItem>
#include <QProgressBar>
#include <QSystemTrayIcon>

namespace Ui {
    class whch;
    class Dialog;
}

const QString NEW_TASK(QObject::tr("Add new task"));
static const QString XML_FILENAME = "whch_log.xml";
static const QString WARNING(QObject::tr("Soo... What have you been doing in the last minutes?"));

typedef QMap<QString, QStringList> MapQStringToList;

class Whch : public QMainWindow
{
    Q_OBJECT

public:
    explicit Whch(QWidget *parent = 0);
    ~Whch();

private slots:
    void onUiComboboxItemActivated(const QString &task);

    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_lineEdit_returnPressed();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onTimerTimeOut();
    void onItemInTreeViewClicked(const QModelIndex &index);
    void onNewTasksAdded(const QStringList &tasks);
    void onSessionDataUpdated(const QString &client, const QStringList &tasks);

    void on_actionTasksClients_triggered();
    void on_actionHistory_View_triggered(bool checked);
    void on_actionAbout_whch_triggered();
    void on_actionQuit_triggered();
    void on_actionExport_to_wiki_format_triggered();

private:
    Ui::whch *m_ui;
    WhchDomModel *m_domModel;
    WhchTreeProxyModel *m_treeProxyModel;
    WhchTableProxyModel *m_tableProxyModel;
    WhchTableProxyModel *m_tableProxyModelHistory;

    // Session data.
    MapQStringToList m_sessionData;

    // Tray icon.
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QAction *restoreAction;
    QAction *quitAction;

    // Track spent time on current task
    int m_seconds;
    QTimer *m_timer;

    void customizeUi();
    void setupData();
    void setupModelView();
    void setCurrentDayIndex();
    QString sessionClientOfTask(const QString &task);
    void loadSessionData();
    void saveSessionData();
    void setComboboxTasks();
    void setCurrentTask();
    void createTrayIconMenuActions();
    void createTrayIcon();
    void setTryIcon();
    void showMessage();
    void closeEvent(QCloseEvent *event);
    void initializeHistoryViews();
};

#endif // WHCH_H
