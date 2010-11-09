#ifndef WHCHSESSIONDATAITEM_H
#define WHCHSESSIONDATAITEM_H

#include <QString>
#include <QStringList>

class WhchSessionDataItem
{
public:
    WhchSessionDataItem();

private:
    QString m_name;
    QStringList m_tasks;
};

#endif // WHCHSESSIONDATAITEM_H
