#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QShowEvent>
#include "../core/cardservice.h"

class StatsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatsDialog(CardService* cardService,
                         const QString& deckName,
                         QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* ev) override;

private:
    CardService*    m_cardService;
    QString         m_deckName;
    QTableWidget*   m_table;

    void setupUI();
    void populateStats();
};

#endif // STATSDIALOG_H
