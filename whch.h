#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whchTableModel.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QDialog>

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

private:
    Ui::whch *m_ui;
    Ui::Dialog *m_uiDialog;
    WhchTableModel *m_model;
    QStringList m_SessionClients; /* New clients added in the current session. */

private slots:
    /* Slots. */
    void setCurrentTaskParameters();
    void onLineEditReturn();
    void showClientTasksInTable(const QString &client);
    void updateSessionClients();

    /* Action slots. */
    void on_actionAbout_whch_triggered();
    void on_actionTasks_triggered();
    void on_actionQuit_triggered();
};

#endif // WHCH_H
