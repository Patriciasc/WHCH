#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whchTableModel.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QDialog>
#include <QTableWidgetItem>

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
    void onLineEditReturn();
    void onDialogComboboxItemActivated(const QString &client);
    void onDialogLineEditReturn();
    void onDialogTableCellChanged(QTableWidgetItem* item);
    void test(QTableWidgetItem* item);

    /* Action slots. */
    void on_actionAbout_whch_triggered();
    void on_actionTasks_triggered();
    void on_actionQuit_triggered();
};

#endif // WHCH_H
