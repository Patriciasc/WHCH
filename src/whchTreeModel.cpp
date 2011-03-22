#include "whchTreeModel.h"
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QDebug>

static const QString XML_FILENAME = "whch_log.xml";

//FIXME: refactorize WRITE and LOAD functions.
WhchTreeModel::WhchTreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    m_domFile = QDomDocument("WHCH");
    QString fileName(QDir::homePath() + "/" + "." + XML_FILENAME);
    QFile file(fileName);

    // Create a new .xml file.
    if (!file.exists())
    {
        // Initialize root element in memory.
        QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));
        QDomElement root = m_domFile.createElement("year");
        root.setAttribute("year",currentDate.section("/", 0, 0));
        m_root = new WhchTreeNode(root, 0);
        m_domFile.appendChild(m_root->node());

        // Write memory data in the .xml file.
        writeInXmlFile(XML_FILENAME);
    }
    else
        m_root = new WhchTreeNode(m_domFile, 0);

    // Load .xml file's content in memory.
    loadXmlFile(XML_FILENAME);
}

WhchTreeModel::~WhchTreeModel()
{
    delete m_root;
}

QVariant WhchTreeModel::data(const QModelIndex &index,
                             int role) const
{
    if ((index.isValid()) && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        WhchTreeNode *indexNode = static_cast<WhchTreeNode*>(index.internalPointer());

        QDomNode node = indexNode->node();
        QStringList attributes;
        QDomNamedNodeMap attributeMap = node.attributes();

        switch (index.column())
        {
        case 0:
            for (int i = 0; i < attributeMap.count(); ++i)
            {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + " " + attribute.nodeValue();
            }
            return attributes;
        case 1:
            {
                QString time = attributeMap.namedItem("start").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
        case 2:
            {
                QString time = attributeMap.namedItem("end").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
        case 3:
            {
                QVariant start = attributeMap.namedItem("start").nodeValue();
                QVariant end = attributeMap.namedItem("end").nodeValue();
                int seconds = start.toDateTime().secsTo(end.toDateTime());
                int minutes = seconds / 60;
                seconds %= 60;
                int hours = minutes / 60;
                minutes %= 60;
                return QString("%1h%2m%3s").arg(hours).arg(minutes).arg(seconds);
            }
        case 4:
            return attributeMap.namedItem("client").nodeValue();
        case 5:
            return attributeMap.namedItem("name").nodeValue();;
        case 6:
            return node.firstChildElement("details").text();
            return "";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags WhchTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if ((index.column() == 1) || (index.column() == 2) || (index.column() == 6))
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    else
    {
        return Qt::ItemIsEnabled;
    }
}

QVariant WhchTreeModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("History");
        case 1:
            return tr("Start");
        case 2:
            return tr("End");
        case 3:
            return tr("Duration");
        case 4:
            return tr("Client");
        case 5:
            return tr("Task");
        case 6:
            return tr("Details");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex WhchTreeModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WhchTreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<WhchTreeNode*>(parent.internalPointer());

    WhchTreeNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex WhchTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    WhchTreeNode *childNode = static_cast<WhchTreeNode*>(child.internalPointer());
    WhchTreeNode *parentNode = childNode->parent();

    if (!parentNode || parentNode == m_root)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int WhchTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    WhchTreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<WhchTreeNode*>(parent.internalPointer());

    return parentNode->node().childNodes().count();
}

int WhchTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

bool WhchTreeModel::setData(const QModelIndex &index,
                            const QVariant &value,
                            int role)
{
    bool changed = false;

    if (index.isValid() && role == Qt::EditRole)
    {

        WhchTreeNode *indexNode = static_cast<WhchTreeNode*>(index.internalPointer());
        QDomNode node = indexNode->node();
        QDomNamedNodeMap attributeMap = node.attributes();

        switch (index.column())
        {
        case 1:
            attributeMap.namedItem("start").setNodeValue(value.toDateTime().toString(Qt::ISODate));
            emit dataChanged(index, index);
            changed = true;
            break;
        case 2:
            attributeMap.namedItem("end").setNodeValue(value.toDateTime().toString(Qt::ISODate));
            emit dataChanged(index, index);
            changed = true;
            break;
        case 6:
            node.firstChildElement("details").firstChild().toText().setNodeValue(value.toString());
            emit dataChanged(index, index);
            changed = true;
            break;
        default:
            changed = false;
            break;
        }

        if (changed)
            writeInXmlFile(XML_FILENAME);
    }
    return changed;
}

/* -------------------- */
/* --- PUBLIC SLOTS --- */
/* -------------------- */
void WhchTreeModel::addNewTaskElement(WhchTask currentTask)
{
    QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));

    QDomElement yearNode = m_root->node().lastChildElement("year");
    QString yearDate(currentDate.section("/", 0, 0));
    if (yearNode.attribute("year").compare(yearDate) != 0)
    {
        yearNode = m_domFile.createElement("year");
        yearNode.setAttribute("year", yearDate);
        m_domFile.appendChild(yearNode);
    }

    QDomElement weekNode = yearNode.lastChildElement("week");
    int weekNumber = QDate::currentDate().weekNumber();
    if(weekNode.attribute("week").toInt() != weekNumber)
    {
        //Create week node and assign it as week node.
        weekNode = m_domFile.createElement("year");
        weekNode.setAttribute("week",  weekNumber);
        m_domFile.appendChild(weekNode);
    }

    QDomElement dayNode = weekNode.lastChildElement("day");
    if(dayNode.attribute("date").compare(currentDate) != 0)
    {
        //Create day node and assign it as day node.
        dayNode = m_domFile.createElement("year");
        dayNode.setAttribute("date", currentDate);
        m_domFile.appendChild(dayNode);
    }

    //Create a new task node.
    m_taskNode.setAttribute("end", QDateTime::currentDateTime().toString(Qt::ISODate));
    m_taskNode.setAttribute("name", currentTask.m_name.section(" (", 0,0));
    m_taskNode.setAttribute("client", currentTask.m_client);

    QDomElement detailsTag = m_domFile.createElement("details");
    QDomText detailsText = m_domFile.createTextNode(currentTask.m_details);
    detailsTag.appendChild(detailsText);
    m_taskNode.appendChild(detailsTag);

    dayNode.appendChild(m_taskNode);

    //Write data in .xml file.
    writeInXmlFile (XML_FILENAME);

    //Reset view.
    reset();
    loadXmlFile(XML_FILENAME);
}

/* --------------------------- */
/* --- AUXILIARY FUNCTIONS --- */
/* --------------------------- */

// Load .xml file's content in memory.
void WhchTreeModel::loadXmlFile(const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error. Could not open file: " << fileName
                << "in" << QDir::current().absolutePath();
        return;
    }

    QString setContentError;
    if (!m_domFile.setContent(&file, &setContentError))
    {
        qDebug() << "Error. Could not set content for file: " << fileName
                << "in" << QDir::current().absolutePath();
        qDebug() << "Error message: " << setContentError;

        file.close();
        return;
    }

    file.close();
}

// Write memory data in the .xml file.
void WhchTreeModel::writeInXmlFile (const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error. Could not write in file: " << fileName;
        return;
    }

    QTextStream ts(&file);
    ts << m_domFile.toString();

    file.close();
 }
