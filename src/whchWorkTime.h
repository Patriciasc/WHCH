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
