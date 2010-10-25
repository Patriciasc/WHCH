#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whch_tablemodel.h"
#include <QFile>

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
};

#endif // WHCH_H
