#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whch_tablemodel.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include "whch_task.h"

namespace Ui {
    class whch;
}

class whch : public QMainWindow
{
    Q_OBJECT

public:
    explicit whch(QWidget *parent = 0);
    ~whch();

private:
    Ui::whch *ui;
    whch_TableModel *m_model;

    int read_in_xml_file();
    int write_in_xml_file();
    QDomElement TaskToNode( QDomDocument &d, const whch_task &c );
};

#endif // WHCH_H
