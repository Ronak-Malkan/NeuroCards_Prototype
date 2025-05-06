#include "statsDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include "../core/flashcard.h"


StatsDialog::StatsDialog(CardService* cardService,
                         const QString& deckName,
                         QWidget* parent)
    : QDialog(parent),
      m_cardService(cardService),
      m_deckName(deckName),
      m_table(nullptr)
{
    setupUI();
    populateStats();
}

void StatsDialog::showEvent(QShowEvent* ev) {
    QDialog::showEvent(ev);
    populateStats();  // refresh whenever the dialog is shown
}

void StatsDialog::setupUI() {
    setWindowTitle(tr("Statistics - %1").arg(m_deckName));
    resize(600, 400);

    auto* layout = new QVBoxLayout(this);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels(
        { tr("Question"), tr("Attempts"), tr("Correct"), tr("Percent") }
    );
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    m_table->setSelectionMode(QTableWidget::NoSelection);

    layout->addWidget(m_table);
    setLayout(layout);
}

void StatsDialog::populateStats() {
    auto cards = m_cardService->getFlashcards(m_deckName);
    const int rowCount = cards.size();

    m_table->clearContents();
    m_table->setRowCount(rowCount);

    for (int i = 0; i < rowCount; ++i) {
        const Flashcard* c = cards.at(i);

        // Question
        m_table->setItem(i, 0,
            new QTableWidgetItem(c->getFrontText())
        );
        // Attempts
        m_table->setItem(i, 1,
            new QTableWidgetItem(QString::number(c->getAttempts()))
        );
        // Correct
        m_table->setItem(i, 2,
            new QTableWidgetItem(QString::number(c->getCorrectCount()))
        );
        // Percent
        int pct = (c->getAttempts() > 0)
                  ? qRound(100.0 * c->getCorrectCount() / c->getAttempts())
                  : 0;
        m_table->setItem(i, 3,
            new QTableWidgetItem(QString::number(pct) + "%")
        );
    }
}
