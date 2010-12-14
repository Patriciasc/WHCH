#include "whchSettings.h"
#include "whchTableModel.h"
#include "whchWorkTime.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QLabel>

whchSettings::whchSettings():
        m_hours(7),
        m_period(PERIOD_DAY),
        m_setBreakMsg("false"),
        m_setOverTimeMsg("false")
{
    /*QCoreApplication::setOrganizationName("Openismus");
    QCoreApplication::setOrganizationDomain("openismus.com");
    QCoreApplication::setApplicationName("WHCH");*/
}

void whchSettings::setHours(int hours)
{
    m_hours = hours;
}

void whchSettings::setPeriod(QString period)
{
    if (period.compare("day") == 0)
        m_period = PERIOD_DAY;
    if (period.compare("week") == 0)
        m_period = PERIOD_WEEK;
    if (period.compare("month") == 0)
        m_period = PERIOD_MONTH;
    if (period.compare("year") == 0)
        m_period = PERIOD_YEAR;
}

void whchSettings::setBreakMsg(bool setBreakMsg)
{
    if (setBreakMsg == true)
         m_setBreakMsg = "true";
    else
         m_setBreakMsg = "false";
}

void whchSettings::setOverTimeMsg(bool setOverTimeMsg)
{
    if (setOverTimeMsg == true)
        m_setOverTimeMsg = "true";
    else
        m_setOverTimeMsg = "false";
}

int whchSettings::hours()
{
    return m_hours;
}

Period whchSettings::period()
{
    return m_period;
}

QString whchSettings::breakMsg()
{
    return m_setBreakMsg;
}

QString whchSettings::overTimeMsg()
{

    return m_setOverTimeMsg;
}

/* Write new selected settings. */
void whchSettings::write()
{
    setValue("hours", QString::number(m_hours));
    setValue("period", m_period);
    setValue("break_msg", m_setBreakMsg);
    setValue("overtime_msg", m_setOverTimeMsg);
}

/* Read settings. */
void whchSettings::read()
{
    int p;

    m_hours = value("hours").toInt();
    p = value("period").toInt();

    switch (p)
    {
        case 0:
            m_period = PERIOD_DAY;
            break;
        case 1:
            m_period = PERIOD_WEEK;
            break;
        case 2:
            m_period = PERIOD_MONTH;
            break;
        case 3:
            m_period = PERIOD_YEAR;
            break;
        default:
            std::cout << "Problem reading period settings" << std::endl;
    }

    m_setBreakMsg = value("break_msg").toString();
    m_setOverTimeMsg = value("break_msg").toString();
}
