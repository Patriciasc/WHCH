#include "whchDomModel.h"
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QDebug>

static const QString XML_FILENAME = "whch_log.xml";

WhchDomModel::WhchDomModel(QObject *parent) :
              QAbstractItemModel(parent)
{
    m_domDocument = QDomDocument("WHCH");
    QString fileName(QDir::homePath() + "/" + "." + XML_FILENAME);
    QFile file(fileName);

    m_rootNode = new WhchDomNode(m_domDocument, 0);

    // Load .xml file's content in memory.
    loadXmlFile(XML_FILENAME);
}


WhchDomModel::~WhchDomModel()
{
    delete m_rootNode;
}

QVariant WhchDomModel::data(const QModelIndex &index,
                            int role) const
{
    if ((index.isValid()) && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        WhchDomNode *indexNode = static_cast<WhchDomNode*>(index.internalPointer());

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
             default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags WhchDomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    int columnNumber = index.column();
    if ((columnNumber == 1) || (columnNumber == 2) || (columnNumber == 6))
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled;
}

QVariant WhchDomModel::headerData(int section,
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

QModelIndex WhchDomModel::index(int row,
                                int column,
                                const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WhchDomNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<WhchDomNode*>(parent.internalPointer());

    WhchDomNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex WhchDomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    WhchDomNode *childNode = static_cast<WhchDomNode*>(child.internalPointer());
    WhchDomNode *parentNode = childNode->parent();

    if (!parentNode || parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int WhchDomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    WhchDomNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<WhchDomNode*>(parent.internalPointer());

    return parentNode->node().childNodes().count();
}

int WhchDomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 7;
}

// Auxiliary functions.
// Load .xml file's content in memory.
void WhchDomModel::loadXmlFile(const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error. Could not open file: " << fileName
                << "in" << QDir::current().absolutePath();
        return;
    }

    QString setContentError;
    if (!m_domDocument.setContent(&file, &setContentError))
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
void WhchDomModel::writeInXmlFile (const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error. Could not write in file: " << fileName;
        return;
    }

    QTextStream ts(&file);
    ts << m_domDocument.toString();

    file.close();
}


// Debugging functions.
void WhchDomModel::printModelIndexTree()
{
    qDebug() << "----------------------------------- Tree Model Index -----------------------------------";
    qDebug() <<"";

    QModelIndex rootIndex = index(0, 0, QModelIndex());
    qDebug() << "root" << rootIndex;

    int numDomElements = (m_rootNode->node().childNodes().count()) - 1;
    for (QDomElement domRoot = m_domDocument.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        QModelIndex domIndex = index(numDomElements--, 0, rootIndex);
        qDebug() << "|";
        qDebug() << "|_" << domRoot.nodeName() << domIndex;

        int numWeekElements = domRoot.childNodes().count() - 1;
        for (QDomElement weekElement = domRoot.firstChildElement("week");
        !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
        {
            QModelIndex weekIndex = index(numWeekElements--, 0, domIndex);
            qDebug() << "   |";
            qDebug() << "   |_" << weekElement.nodeName() << weekIndex;
            qDebug() << "   |  |";

            int numDayElements = weekElement.childNodes().count() - 1;
            for (QDomElement dayElement = weekElement.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                QModelIndex dayIndex = index(numDayElements--, 0, weekIndex);
                qDebug() << "   |  |_" << dayElement.nodeName() << dayIndex;
            }
        }
    }

    qDebug() <<"";
    qDebug() << "----------------------------------------------------------------------------------------";
}
