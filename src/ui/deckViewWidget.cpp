#include "deckViewWidget.h"
#include "cardPreviewDialog.h"  // for preview on click

#include <QVBoxLayout>
#include <QListWidgetItem>

DeckViewWidget::DeckViewWidget(DeckManager* deckManager,
                               const QString& deckName,
                               QWidget* parent)
    : QWidget(parent),
      m_deckManager(deckManager),
      m_deckName(deckName)
{
    setupUI();
    connectSignals();
    refreshList();
}

void DeckViewWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    m_listWidget = new QListWidget(this);
    layout->addWidget(m_listWidget);
    setLayout(layout);
}

void DeckViewWidget::connectSignals() {
    // When the user clicks a question, show preview dialog
    connect(m_listWidget, &QListWidget::itemClicked,
            this, [this](QListWidgetItem* item){
        int row = m_listWidget->row(item);
        auto cards = m_deckManager->getFlashcards(m_deckName);
        if (row >= 0 && row < cards.size()) {
            const Flashcard& card = cards.at(row);
            // Emit a signal or show preview directly:
            CardPreviewDialog dlg(card, this);
            dlg.exec();
        }
    });
}

void DeckViewWidget::refreshList() {
    m_listWidget->clear();
    auto cards = m_deckManager->getFlashcards(m_deckName);
    for (const Flashcard& card : cards) {
        // Show only the front text
        m_listWidget->addItem(card.getFrontText());
    }
}

