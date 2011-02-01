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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // START Check for another instance of whch
    // GUID: http://www.guidgenerator.com/online-guid-generator.aspx
    QSharedMemory shared("c53e2474-1d05-4351-bd18-4f4e46490437");
    if( !shared.create( 512, QSharedMemory::ReadWrite) )
    {
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("Can't start more than one instance of the application.") );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        exit(0);
    }
    // END Check for another instance of whch

    Whch w;
    w.show();

    return a.exec();
}
