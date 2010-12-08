#include "configDialog.h"
#include "ui_configDialog.h"
#include <QSettings>

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
    writeSettings();

    accept();
}

/* Write new selected settings. */
void configDialog::writeSettings()
{
    QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("WHCH (Working Hours Counter for Humans)");

    QSettings settings;
    settings.setValue("hours", QString::number(this->ui->spinBox->value()));
    settings.setValue("period", this->ui->comboBox->currentText());

    if (this->ui->checkBox->isChecked() == true)
        settings.setValue("break_msg", "true");
    else
        settings.setValue("break_msg", "false");

    if (this->ui->checkBox_2->isChecked() == true)
        settings.setValue("overtime_msg", "true");
    else
        settings.setValue("overtime_msg", "false");
}
