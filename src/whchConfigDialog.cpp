/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- configDialog.cpp      #
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

#include "whchConfigDialog.h"
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
    settings.setHours(ui->spinBox->value());
    settings.setPeriod(ui->comboBox->currentText());
    settings.setBreakMsg(ui->checkBox->isChecked());
    settings.setOverTimeMsg(ui->checkBox_2->isChecked());
    settings.write();

    accept();
}
