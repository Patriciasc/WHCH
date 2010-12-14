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
    void hours(int hours);
    void period(QString period);
    void setBreakMsg(bool setBreakMsg);
    void setOverTimeMsg(bool setOverTimeMsg);
    void write();
    void read();
    void set(QLabel *statusText, WhchTableModel *model);

private:
    int m_hours;
    Period m_period;
    QString m_setBreakMsg;
    QString m_setOverTimeMsg;
};

#endif // WHCHSETTINGS_H
