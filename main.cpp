#include <QtGui/QApplication>
#include "whch.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Whch w;
    w.show();

    return a.exec();
}
