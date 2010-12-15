/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchWorkTime.h        #
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

#ifndef WHCHWORKTIME_H
#define WHCHWORKTIME_H

#include "whchSettings.h"
#include <QDomDocument>

class whchWorkTime
{
public:
    whchWorkTime();
    int hours();
    int minutes();
    int seconds();
    void workedTime(Period timePeriod,  QDomDocument domFile);
    void TotalTime(Period timePeriod, int hours);

private:
    int m_hours;
    int m_minutes;
    int m_seconds;
    void dayWorkedTime(QString currentDate, QDomDocument domFile);
    void weekWorkedTime(QString currentDate, QDomDocument domFile);
    void monthWorkedTime(QString currentDate, QDomDocument domFile);
    void yearWorkedTime(QString currentDate, QDomDocument domFile);
    void dayTotalTime(int hours);
    void weekTotalTime(int hours);
    void monthTotalTime(int hours);
    void yearTotalTime(int hours);
    int weekendDays(QDate currentDate, QDate endDate);
};

#endif // WHCHWORKTIME_H
