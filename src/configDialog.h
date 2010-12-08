#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
    class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT

public:
    explicit configDialog(QWidget *parent = 0);
    ~configDialog();

private:
    Ui::configDialog *ui;

    void writeSettings();

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
};

#endif // CONFIGDIALOG_H
