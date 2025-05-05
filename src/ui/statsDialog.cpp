#include "statsDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>

StatsDialog::StatsDialog(DeckManager* manager,
                         const QString& deckName,
                         QWidget* parent)
    : QDialog(parent),
    m_deckManager(manager),
    m_deckName(deckName)
{
    setWindowTitle(tr("Statistics - %1").arg(deckName));
    resize(600, 400);
    setupUI();
    populateStats();
}

void StatsDialog::setupUI() {
    auto* layout = new QVBoxLayout(this);
    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({ tr("Question"),
                                        tr("Attempts"),
                                        tr("Correct"),
                                        tr("Percent") });
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_table);
}

void StatsDialog::populateStats() {
    auto cards = m_deckManager->getFlashcards(m_deckName);
    m_table->setRowCount(cards.size());
    for (int i = 0; i < cards.size(); ++i) {
        const Flashcard& c = cards.at(i);
        int attempts = c.getAttempts();
        int correct  = c.getCorrectCount();
        double pct   = attempts > 0 ? (correct * 100.0 / attempts) : 0.0;
        m_table->setItem(i, 0, new QTableWidgetItem(c.getFrontText()));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(attempts)));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(correct)));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(pct, 'f', 1) + "%"));
    }
}
