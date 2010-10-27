#ifndef WHCH_TASK_H
#define WHCH_TASK_H

#include <QString>
#include <QTime>


class whch_task
{
public:
    whch_task();

    QTime start;
    QTime end;
    QTime duration;
    QString client;
    QString name;
    QString details;
};

#endif // WHCH_TASK_H
