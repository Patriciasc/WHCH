/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchSettings.h        #
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

#ifndef WHCHSETTINGS_H
#define WHCHSETTINGS_H

#include "configDialog.h"
#include "whchTableModel.h"
#include <QSettings>
#include <QLabel>

enum Period
{
        PERIOD_DAY,
        PERIOD_WEEK,
        PERIOD_MONTH,
        PERIOD_YEAR
};

class whchSettings: public QSettings
{
public:
    whchSettings();
    void setHours(int hours);
    void setPeriod(QString period);
    void setBreakMsg(bool setBreakMsg);
    void setOverTimeMsg(bool setOverTimeMsg);
    int hours();
    Period period();
    QString breakMsg();
    QString overTimeMsg();
    void write();
    void read();
    //void set(QLabel *statusText, WhchTableModel *model);

private:
    int m_hours;
    Period m_period;
    QString m_setBreakMsg;
    QString m_setOverTimeMsg;
};

#endif // WHCHSETTINGS_H
