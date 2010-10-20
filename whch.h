#ifndef WHCH_H
#define WHCH_H

#include <QMainWindow>

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
};

#endif // WHCH_H
