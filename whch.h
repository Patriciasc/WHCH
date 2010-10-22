#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>
#include "whch_tablemodel.h"

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
};

#endif // WHCH_H
