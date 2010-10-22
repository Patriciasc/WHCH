#include "whch.h"
#include "ui_whch.h"
#include <QDomDocument>
#include <iostream>

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);

    /* Set model/view with test data. */
    QList<QString> mylist;
    mylist << "one" << "two" << "three";

    m_model = new whch_TableModel(mylist);
    ui->tableView->setModel(m_model);
}

void whch::load_xml_file_to_dom (QString file_name)
{
    QDomDocument doc(file_name);
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    //TEST
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            std::cout << qPrintable(e.tagName()) << std::endl;
        }
        n = n.nextSibling();
    }
}

whch::~whch()
{
    delete ui;
}
