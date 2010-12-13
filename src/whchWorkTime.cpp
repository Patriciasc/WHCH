#include "whchWorkTime.h"
#include <iostream>
#include <QDate>

whchWorkTime::whchWorkTime():
        m_hours(0),
        m_minutes(0),
        m_seconds(0)
{
}

int whchWorkTime::hours()
{
    return m_hours;
}

int whchWorkTime::minutes()
{
    return m_minutes;
}

int whchWorkTime::seconds()
{
    return m_seconds;
}

/* Returns the total time worked in a period. */
void  whchWorkTime::workedTime(Period timePeriod,   QDomDocument domFile)
{
    QString currentDate (QDate::currentDate().toString("yyyy/MM/dd"));

    switch (timePeriod)
    {
        case PERIOD_DAY:
            return dayWorkedTime(currentDate, domFile);
        case PERIOD_WEEK:
            return weekWorkedTime(currentDate, domFile);
        case PERIOD_MONTH:
            return monthWorkedTime(currentDate, domFile);
        case PERIOD_YEAR:
            return yearWorkedTime(currentDate, domFile);
        default:
            std::cout << "Failed calculating workedTime period" << std::endl;
    }
}

/* FIXME: Optimize these workedTime functions. */
/* Returns the total time worked in a day. */
void whchWorkTime::dayWorkedTime(QString currentDate,  QDomDocument domFile)
{
    // Serach for the element in memory
    for (QDomElement domRoot = domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            for (QDomElement dayElement = domRoot.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                if (dayElement.attribute("date").compare(currentDate) == 0)
                {
                    for (QDomElement element = dayElement.firstChildElement("task");
                    !element.isNull(); element = element.nextSiblingElement("task"))
                    {
                        //TODO: function to calculate duration.
                        QVariant start = element.attribute("start");
                        QVariant end = element.attribute("end");
                        m_seconds += start.toDateTime().secsTo(end.toDateTime());
                    }
                }
            }
        }
    }
    m_minutes = m_seconds / 60;
    m_seconds %= 60;
    m_hours = m_minutes / 60;
    m_minutes %= 60;
}

/* Returns the total time worked in a week. */
void whchWorkTime::weekWorkedTime(QString currentDate, QDomDocument domFile)
{
    int currentWeek = QDate::currentDate().weekNumber();

    // Serach for the element in memory
    for (QDomElement domRoot = domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            for (QDomElement dayElement = domRoot.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                if (dayElement.attribute("week").toInt() == currentWeek)
                {
                    for (QDomElement element = dayElement.firstChildElement("task");
                    !element.isNull(); element = element.nextSiblingElement("task"))
                    {
                        //TODO: function to calculate duration.
                        QVariant start = element.attribute("start");
                        QVariant end = element.attribute("end");
                        m_seconds = start.toDateTime().secsTo(end.toDateTime());
                    }
                }
            }
        }
    }
    m_minutes = m_seconds / 60;
    m_seconds %= 60;
    m_hours = m_minutes / 60;
    m_minutes %= 60;
}

/* Returns the total time worked in a month. */
void whchWorkTime::monthWorkedTime(QString currentDate, QDomDocument domFile)
{
    QString currentMonth = currentDate.section("/", 1,1);

    // Serach for the element in memory
    for (QDomElement domRoot = domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            for (QDomElement dayElement = domRoot.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                QString dayMonth = dayElement.attribute("date").section("/", 1,1);
                if (dayMonth.compare(currentMonth) == 0)
                {
                    for (QDomElement element = dayElement.firstChildElement("task");
                    !element.isNull(); element = element.nextSiblingElement("task"))
                    {
                        //TODO: function to calculate duration.
                        QVariant start = element.attribute("start");
                        QVariant end = element.attribute("end");
                        m_seconds = start.toDateTime().secsTo(end.toDateTime());
                    }
                }
            }
        }
    }
    m_minutes = m_seconds / 60;
    m_seconds %= 60;
    m_hours = m_minutes / 60;
    m_minutes %= 60;
}

/* Returns the total time worked in a year. */
void whchWorkTime::yearWorkedTime(QString currentDate, QDomDocument domFile)
{
    // Serach for the element in memory
    for (QDomElement domRoot = domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            for (QDomElement dayElement = domRoot.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                for (QDomElement element = dayElement.firstChildElement("task");
                !element.isNull(); element = element.nextSiblingElement("task"))
                {
                    //TODO: function to calculate duration.
                    QVariant start = element.attribute("start");
                    QVariant end = element.attribute("end");
                    m_seconds = start.toDateTime().secsTo(end.toDateTime());
                }
            }
        }
    }
    m_minutes = m_seconds / 60;
    m_seconds %= 60;
    m_hours = m_minutes / 60;
    m_minutes %= 60;
}

/* Returns total time to be worked in a period. */
void whchWorkTime::TotalTime(Period timePeriod, int hours)
{
    switch (timePeriod)
    {
        case PERIOD_DAY:
            return dayTotalTime(hours);
        case PERIOD_WEEK:
            return weekTotalTime(hours);
        case PERIOD_MONTH:
            return monthTotalTime(hours);
        case PERIOD_YEAR:
            return yearTotalTime(hours);
        default:
            std::cout << "Failed calculating workedTime period" << std::endl;
    }
}

/* Returns total time to be worked in a day. */
void whchWorkTime::dayTotalTime(int hours)
{
    m_hours = hours;
}

/* Returns total time to be worked in a week. */
void whchWorkTime::weekTotalTime(int hours)
{
    m_hours = hours * 5;
}

/* Returns total time to be worked in a month. */
void whchWorkTime::monthTotalTime(int hours)
{
    QDate currentDate(QDate::currentDate());
    QDate endDate(currentDate.year(), currentDate.month(), currentDate.daysInMonth());
    int totalDays = currentDate.daysTo(endDate) + 1;
    int totalWeekendDays = weekendDays(currentDate, endDate);

    m_hours = (totalDays - totalWeekendDays) * hours;
}

/* Returns total time to be worked in a year. */
void whchWorkTime::yearTotalTime(int hours)
{
    QDate currentDate(QDate::currentDate());
    QDate endDate(currentDate.year(), 12, 31);
    int totalDays = currentDate.daysTo(endDate) + 1;
    int totalWeekendDays = weekendDays(currentDate, endDate);

    m_hours = (totalDays-totalWeekendDays)*hours;
}

/* Start: TEST function*/
int whchWorkTime::weekendDays(QDate currentDate, QDate endDate)
{
    int sunday = 7;
    int saturday = 6;
    int totalWeeks = currentDate.daysTo(endDate)/7;
    int saturdays = totalWeeks;
    int sundays = totalWeeks;
    int currentDay = currentDate.dayOfWeek();
    int endDay = endDate.dayOfWeek();

    if (currentDay > endDay)
    {
        sundays++;
        saturdays += (currentDay < sunday) ? 1 : 0;
    }
    else
    {
        if (currentDay < endDay)
            saturdays += (endDay == sunday) ? 1 : 0;
        else
        {
            if ((currentDay == endDay) &&
               ((currentDay == sunday) || (currentDay == saturday)))
                saturdays++;
        }
    }
    return saturdays + sundays;
}
/* End: TEST function*/
