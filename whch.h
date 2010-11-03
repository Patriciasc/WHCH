#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whch_tablemodel.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include "whch_task.h"
#include <QDialog>

namespace Ui {
    class whch;
    class Dialog;
}

class whch : public QMainWindow
{
    Q_OBJECT

public:
    explicit whch(QWidget *parent = 0);
    ~whch();

private:
    Ui::whch *ui;
    Ui::Dialog *ui_dialog;
    whch_TableModel *m_model;

private slots:
    void set_current_task_parameters();

    void on_actionAbout_whch_triggered();
    void on_actionTasks_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_file_triggered();
    void on_actionNew_file_triggered();
};

#endif // WHCH_H
