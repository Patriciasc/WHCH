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

    /* Set model/view. */
    m_model = new whch_TableModel();
    ui->tableView->setModel(m_model);
}

int whch::read_in_xml_file ()
{
    QDomDocument doc( "DOMtest" );
    QFile file( "test.xml" );
    if( !file.open(QIODevice::ReadOnly) )
    {
        std::cout << "Could not open file" << std::endl;
        return -1;
    }
    if( !doc.setContent( &file ) )
    {
        std::cout << "Problem setting content" << std::endl;
        file.close();
        return -2;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( root.tagName() != "day" )
    {
      std::cout << "Did not find DOM root" << std::endl;
      return -3;
    }

    QDomNode n = root.firstChild();
    while( !n.isNull() )
    {
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {
            if( e.tagName() == "task" )
            {
                whch_task c;

                c.start = e.attribute( "start", "" );
                c.end = e.attribute( "end", "" );
                c.client = e.attribute( "client", "" );

                QMessageBox msgBox;
                //msgBox.information( 0, "Task", c.start + "\n" + c.end + "\n" + c.client );
            }
        }

        n = n.nextSibling();
    }
    return 0;
}


QDomElement TaskToNode( QDomDocument &d, const whch_task &c )
{
   QDomElement cn = d.createElement( "task" );

   cn.setAttribute( "start", c.start );
   cn.setAttribute( "end", c.end );
   cn.setAttribute( "client", c.client );

   QDomElement tn = d.createElement("details");
   QDomText text = d.createTextNode("something");
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
    c.start = "10:30";
    c.end = "18:30";
    c.client = "Openismus";

    root.appendChild( TaskToNode( doc, c ) );

    c.start = "10:00";
    c.end = "17:00";
    c.client = "Nokia";

    root.appendChild( TaskToNode( doc, c ) );

    c.start = "10:00";
    c.end = "17:00";
    c.client = "Nokia";

    root.appendChild( TaskToNode( doc, c ) );

    c.start = "10:00";
    c.end = "17:00";
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
