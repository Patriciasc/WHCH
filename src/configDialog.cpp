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
    QCoreApplication::setApplicationName("WHCH");

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

/* Read settings. */
void configDialog::readSettings()
{
    QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("WHCH");

    QSettings settings;

    int hours = settings.value("hours").toInt();
    QString period = settings.value("period").toString();
    bool setBreakMsg = settings.value("break_msg").toBool();
    bool setOvertimeMsg = settings.value("break_msg").toBool();
}

void configDialog::setSettings()
{
    /* readSettings() */
    /* set statusbar */
    /* set notification messages */
}
