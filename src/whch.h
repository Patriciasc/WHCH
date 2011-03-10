/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whch.h                #
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

#ifndef WHCH_H
#define WHCH_H


#include "whchTableModel.h"
#include "whchTreeModel.h"
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

class Whch : public QMainWindow
{
    Q_OBJECT

public:
    explicit Whch(QWidget *parent = 0);
    ~Whch();

private slots:
    void on_actionHistory_View_triggered(bool checked);
    void onUiComboboxItemActivated(const QString &task);
    void onDialogComboboxItemActivated(const QString &client);
    void onDialogTableCellChanged(QTableWidgetItem *item);
    void onDialogTableItemChanged(QTableWidgetItem *item);
    void on_lineEdit_returnPressed();
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void onDialogComboboxLineEditReturn();

    void on_actionAbout_whch_triggered();
    void on_actionTasks_triggered();
    void on_actionQuit_triggered();
    void on_actionExport_to_wiki_format_triggered();

    void onTimerTimeOut();
    QStringList sessionTasks();
    QString sessionClientOfTask(const QString &task);
    QStringList sessionClientTasks(const QString &client);
    bool isSessionClient(const QString &client);
    QStringList clientTotalTasks(const QString &client);
    QStringList totalTasks();
    void loadSessionData();
    void saveSessionData();
    void setComboboxTasks();
    void setCurrentTask();
    void createTrayIconMenuActions();
    void createTrayIcon();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void setTryIcon();
    void showMessage();
    void closeEvent(QCloseEvent *event);

private:
    Ui::whch *m_ui;
    Ui::Dialog *m_uiDialog;
    WhchTableModel *m_tableModel;
    WhchTreeModel *m_treeModel;
    /* New tasks/client added by the user in the current session */
    typedef QMap<QString, QStringList> MapQStringToList;
    MapQStringToList m_sessionData;

    /* Tray icon. */
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QAction *restoreAction;
    QAction *quitAction;

    /* Track spent time on current task */
    int m_seconds;
    QTimer *m_timer;
};

#endif // WHCH_H
