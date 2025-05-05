#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QShowEvent>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

class StatsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatsDialog(DeckManager* manager,
                         const QString& deckName,
                         QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* ev) override;

private:
    DeckManager*    m_deckManager;
    QString         m_deckName;
    QTableWidget*   m_table;

    void setupUI();
    void populateStats();
};

#endif // STATSDIALOG_H
