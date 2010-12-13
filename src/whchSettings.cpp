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

void whchSettings::hours(int hours)
{
    m_hours = hours;
}

void whchSettings::period(QString period)
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

/* Write new selected settings. */
void whchSettings::writeSettings()
{
    setValue("hours", QString::number(m_hours));
    setValue("period", m_period);
    setValue("break_msg", m_setBreakMsg);
    setValue("overtime_msg", m_setOverTimeMsg);
}

/* Read settings. */
void whchSettings::readSettings()
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

/* Still TESTING. */
/* Set settings and update UI/Session components.*/
void whchSettings::setSettings(QLabel *statusText, WhchTableModel *model)
{
    /* Read content of already existent setting files. */
    if (contains("hours"))
        readSettings();

    /* TODO: Update dialog UI. */

    /* Calculate total worked and left time. */
    whchWorkTime totalWorkTime;
    totalWorkTime.workedTime(m_period, model->domFile());
    whchWorkTime workTimeLeft;
    workTimeLeft.TotalTime(m_period, m_hours);

    /* Set status bar. */
    /* TODO: calculate correctly the minutes/seconds. */
    QString total = QString("%1h%2m%3s").
                    arg(totalWorkTime.hours()).
                    arg(totalWorkTime.minutes()).
                    arg(totalWorkTime.seconds());
    QString left = QString("%1h%2m%3s").
                   arg(workTimeLeft.hours() - totalWorkTime.hours()).
                   arg(workTimeLeft.minutes() - totalWorkTime.minutes()).
                   arg(workTimeLeft.seconds() - totalWorkTime.seconds());

    statusText->setText(QDate::currentDate().toString("ddMMM") + ", Total: "+ total + ", Left: " + left);
}

