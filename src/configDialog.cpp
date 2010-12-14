#include "configDialog.h"
#include "ui_configDialog.h"
#include "whchSettings.h"
#include <QSettings>
#include <iostream>


configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
}

configDialog::~configDialog()
{
    delete ui;
}

void configDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    Q_UNUSED(button);

    /* Save settings. */
    QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("WHCH");

    whchSettings settings;
    settings.hours(ui->spinBox->value());
    settings.period(ui->comboBox->currentText());
    settings.setBreakMsg(ui->checkBox->isChecked());
    settings.setOverTimeMsg(ui->checkBox_2->isChecked());
    settings.write();

    accept();
}
