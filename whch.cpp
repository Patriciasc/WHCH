#include "whch.h"
#include "ui_whch.h"
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <QMessageBox>
#include "whch_task.h"
#include <QDate>

whch::whch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::whch)
{
    ui->setupUi(this);
    write_in_xml_file();
    /* Set model/view. */
    m_model = new whch_TableModel();
    ui->tableView->setModel(m_model);
    ui->tableView->resizeRowsToContents();
    //ui->tableView->resizeColumnsToContents();
}

QDomElement TaskToNode( QDomDocument &d, const whch_task &c )
{
   QDomElement cn = d.createElement( "task" );

   cn.setAttribute("start", c.start.currentTime().toString("hh:mm"));
   cn.setAttribute("end", c.end.currentTime().toString("hh:mm"));
   // I will need to make use of restart()/start()/elapse here.
   cn.setAttribute("duration", "duration");
   cn.setAttribute("name", "name");
   cn.setAttribute("client", c.client);
   QDomElement tn = d.createElement("details");
   QDomText text = d.createTextNode("????????????????????????????????????????????????????????????????????????????????????????????????????????????????????");
   tn.appendChild(text);
   cn.appendChild(tn);

   return cn;
}

int whch::write_in_xml_file()
{
    // Create a DOM structure.
    QDomDocument doc("DOMtest");

    // Create a root element for the DOM.
    QDomElement root = doc.createElement("day");
    root.setAttribute("date",QDate::currentDate().toString("yyyy/MM/dd"));
    doc.appendChild(root);

    // Add data to the DOM.
    whch_task c;
    c.client = "Openismus";
    root.appendChild( TaskToNode( doc, c ) );

    c.client = "Nokia";
    root.appendChild( TaskToNode( doc, c ) );

    c.client = "Nokia";
    root.appendChild( TaskToNode( doc, c ) );

    c.client = "Nokia";
    root.appendChild( TaskToNode( doc, c ) );


    // Write result to an .xml file.
    QFile file( "test.xml" );
    if( !file.open(QIODevice::WriteOnly) )
        return -1;

    QTextStream ts( &file );
    ts << doc.toString();

    file.close();

    return 0;
}

whch::~whch()
{
    delete ui;
}
