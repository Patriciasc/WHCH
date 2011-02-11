/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- main.cpp              #
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

#include "whch.h"
#include <QtGui/QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QDebug>
#include "qtsingleapplication/qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

    if (app.isRunning())
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Sorry, an other instance of WHCH is running already."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return 1;
    }

    Whch w;
    app.setActiveWindow(&w);
    w.show();

    return app.exec();
}
