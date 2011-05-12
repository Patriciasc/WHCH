#include "whchClientsTasksDialog.h"
#include <QLineEdit>
#include <QDebug>

static const QString NEW_CLIENT(QObject::tr("Add new client"));

WhchClientsTasksDialog::WhchClientsTasksDialog(WhchDomModel *domModel,
                                               const MapQStringToList &sessionData) :
        m_hLayout(new QHBoxLayout),
        m_vLayout(new QVBoxLayout (this)),
        m_label(new QLabel ("Client", this)),
        m_comboBox(new QComboBox (this)),
        m_hSpacer(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum)),
        m_tableWidget(new QTableWidget (this)),
        m_domModel(domModel),
        m_sessionData(sessionData)
{
    setUpUi();
    setUpInitData();
    setUpInitConnections();
    show();
}

void WhchClientsTasksDialog::setUpUi()
{
    setWindowModality(Qt::ApplicationModal);
    resize(289, 277);
    setMaximumSize(QSize(16777144, 16770000));
    setModal(true);

    m_hLayout->addWidget(m_label);

    m_comboBox->setEditable(false);
    m_comboBox->setMaxVisibleItems(20);
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_comboBox->setFrame(true);
    m_hLayout->addWidget(m_comboBox);

    m_hLayout->addItem(m_hSpacer);
    m_vLayout->addLayout(m_hLayout);

    if (m_tableWidget->columnCount() < 1)
        m_tableWidget->setColumnCount(1);

    QTableWidgetItem *newItem = new QTableWidgetItem();
    m_tableWidget->setHorizontalHeaderItem(0, newItem);
    m_tableWidget->setFrameShape(QFrame::StyledPanel);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->verticalHeader()->setVisible(false);
    QStringList tasks ("Tasks");
    m_tableWidget->setHorizontalHeaderLabels(tasks);
    m_vLayout->addWidget(m_tableWidget);
}

void WhchClientsTasksDialog::setUpInitData()
{
    // Insert new added clients always at the top.
    m_comboBox->setInsertPolicy(QComboBox::InsertAtTop);

    // Load list of clients.
    QStringList clients(m_domModel->AttributesList("client"));
    if (!m_sessionData.isEmpty())
    {
        QStringList sessionClients (m_sessionData.keys());
        for (int i=0; i < sessionClients.size(); ++i)
        {
            const QString sessionClientItem(sessionClients.at(i));
            if(!clients.contains(sessionClientItem))
                clients << sessionClientItem;
        }
    }

    clients << NEW_CLIENT;
    m_comboBox->addItems(clients);

    // Load list of tasks for initial client
    const QString currentClient (m_comboBox->currentText());
    if (currentClient.compare(NEW_CLIENT) != 0)
        onComboboxItemActivated(currentClient);
}

void WhchClientsTasksDialog::setUpInitConnections()
{
    // Load list of related tasks to the selected client.
    QObject::connect(m_comboBox, SIGNAL(activated(QString)),
                     this, SLOT(onComboboxItemActivated(QString)));

    // Makes last row editable.
    QObject::connect(m_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
                     this, SLOT(onTableCellChanged(QTableWidgetItem *)));

    // Manages data if any cell of the last row is edited.
    QObject::connect(m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)),
                     this, SLOT(onTableItemChanged(QTableWidgetItem *)));
}


// Shows the related tasks of the selected client.
void WhchClientsTasksDialog::onComboboxItemActivated(const QString &client)
{
    // Make it editable for adding a new client.
    if (client.compare(NEW_CLIENT) == 0)
    {
        m_comboBox->setEditable(true);
        m_comboBox->clearEditText();
        QObject::connect(m_comboBox->lineEdit(), SIGNAL(returnPressed()),
                         this, SLOT(onComboboxLineEditReturn()));
    }
    else
        m_comboBox->setEditable(false);

    // Set current available tasks and new line for new task.
    QStringList clientTasks(m_domModel->xmlClientTasks(client));
    if (m_sessionData.contains(client))
    {
        QStringList sessionClientTasks(m_sessionData.value(client));
        for (int i = 0; i < sessionClientTasks.size(); ++i)
        {
            const QString sessionTaskItem(sessionClientTasks.at(i));
            if (!clientTasks.contains(sessionTaskItem))
                clientTasks << sessionTaskItem;
        }
    }

    clientTasks << "";

    // Set number of rows.
    m_tableWidget->setRowCount(clientTasks.size());

    // Show list of tasks.
    for (int i=0; i< clientTasks.size(); ++i)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(clientTasks.at(i));
        m_tableWidget->setItem(i, 0, newItem);
    }
}

// Updates session data.
void WhchClientsTasksDialog::onComboboxLineEditReturn()
{
    QString newClient = m_comboBox->currentText();

    if (newClient.compare("") != 0)
    {
        QStringList clients(m_domModel->AttributesList("client"));

        if (!clients.contains(newClient))
        {
            QStringList tasks;
            m_sessionData.insert(newClient, tasks);
            emit sessionDataUpdated(newClient, tasks);
        }
    }
}

// Makes last row editable.
void WhchClientsTasksDialog::onTableCellChanged(QTableWidgetItem *item)
{
    if ((item->column() == 0) && (item->row() == m_tableWidget->rowCount()-1))
        m_tableWidget->editItem(item);
}

// Saves user's new task into the session's structure.
void WhchClientsTasksDialog::onTableItemChanged(QTableWidgetItem *item)
{
    const QString itemText (item->text());
    QString selectedClient (m_comboBox->currentText());
    if (selectedClient.compare("") != 0)
    {
        if (itemText.compare("") != 0)
        {
            // Update task's combobox.
            if (!totalTasks().contains(itemText))
            {
                QStringList comboboxTasks;
                QStringList totalListTasks (totalTasks());
                QStringList taskClientList;
                for (int i=0; i < totalListTasks.size(); ++i)
                {
                    QString currentTask (totalListTasks.at(i));
                    QString taskClient (m_domModel->xmlClientOfTask(currentTask));
                    if (taskClient.compare("") == 0)
                        taskClient = sessionClientOfTask(currentTask);

                    taskClientList << currentTask + " (" + taskClient + ")";
                }

                comboboxTasks << itemText + " (" + selectedClient + ")" << taskClientList <<  NEW_TASK;
                emit newTasksAdded(comboboxTasks);
            }

            // Save data into session structure.
            const QString currentClient(m_comboBox->currentText());

            // Do not repeat tasks.
            if(!clientTasks(currentClient).contains(itemText))
            {
                // Add new task as session data.
                QStringList tasks(m_sessionData.value(currentClient));
                tasks << itemText;
                m_sessionData.insert(currentClient, tasks);
                emit sessionDataUpdated(currentClient, tasks);

                // Insert new empty element.
                m_tableWidget->setRowCount(clientTasks(currentClient).count()+1);
                QTableWidgetItem *newItem = new QTableWidgetItem("");
                m_tableWidget->setItem(clientTasks(currentClient).count(), 0, newItem);
            }

        }
    }
}

// Returns total tasks (session and .xml file ones).
QStringList WhchClientsTasksDialog::totalTasks() const
{
    QStringList xmlTotalTasks (m_domModel->AttributesList("name"));
    QStringList sessionTotalTasks (sessionTasks());

    for (int i = 0; i < sessionTotalTasks.count(); ++i)
    {
        const QString sessionTaskItem(sessionTotalTasks.at(i));
        if (!xmlTotalTasks.contains(sessionTaskItem))
            xmlTotalTasks << sessionTaskItem;
    }
    return xmlTotalTasks;
}

// List of new added tasks.
QStringList WhchClientsTasksDialog::sessionTasks() const
{
    QStringList sessionTasks;
    MapQStringToList::const_iterator i = m_sessionData.constBegin();

    while (i != m_sessionData.constEnd())
    {
        QStringList itemTasks = i.value();
        for (int j = 0; j < itemTasks.size(); ++j)
        {
            // Do not repeat elements.
            const QString item(itemTasks.at(j));
            if (!sessionTasks.contains(item))
                sessionTasks << item;
        }
        ++i;
    }
    return sessionTasks;
}

// Returns a list of the total tasks of a client. Those saved
// in the .xml file and those that are session data.
QStringList WhchClientsTasksDialog::clientTasks(const QString &client) const
{
    QStringList clientTasks;

    if (m_domModel->isXmlClient(client))
        clientTasks << m_domModel->xmlClientTasks(client);

    if (m_sessionData.contains(client))
    {
        QStringList sessionClientTasks(m_sessionData.value(client));
        for (int i = 0; i < sessionClientTasks.size(); ++i)
        {
            const QString sessionTaskItem(sessionClientTasks.at(i));
            if (!clientTasks.contains(sessionTaskItem))
                clientTasks << sessionTaskItem;
        }
    }
    return clientTasks;
}

// Gets related client of a task.
QString WhchClientsTasksDialog::sessionClientOfTask(const QString &task) const
{
    MapQStringToList::const_iterator i = m_sessionData.constBegin();
    QString sessionClient;

    while (i != m_sessionData.constEnd())
    {
        QStringList itemTasks = i.value();
        for (int j = 0; j < itemTasks.size(); ++j)
        {
            if(task.contains(itemTasks.at(j)))
                return sessionClient = i.key();
        }
        ++i;
    }
    return sessionClient;
}
