#ifndef WHCHCLIENTSTASKSDIALOG_H
#define WHCHCLIENTSTASKSDIALOG_H

#include "whchDomModel.h"
#include "whch.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>

class WhchClientsTasksDialog : public QDialog
{
    Q_OBJECT

public:
    WhchClientsTasksDialog(WhchDomModel *domModel, MapQStringToList sessionData);

private:
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    QLabel *m_label;
    QComboBox *m_comboBox;
    QSpacerItem *m_hSpacer;
    QTableWidget *m_tableWidget;

    WhchDomModel *m_domModel;
    MapQStringToList m_sessionData;

    void setUpUi();
    void setUpInitData();
    void setUpInitConnections();
    QStringList totalTasks() const;
    QStringList sessionTasks() const;
    QStringList clientTasks(const QString &client) const;
    QString sessionClientOfTask(const QString &task) const;

private slots:
    void onComboboxItemActivated(const QString &client);
    void onComboboxLineEditReturn();
    void onTableCellChanged(QTableWidgetItem *item);
    void onTableItemChanged(QTableWidgetItem *item);

signals:
    void newTasksAdded(QStringList tasks);
    void sessionDataUpdated (QString client, QStringList tasks);
};

#endif // WHCHCLIENTSTASKSDIALOG_H
