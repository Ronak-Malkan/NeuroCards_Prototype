#include "deckViewWidget.h"
#include "cardPreviewDialog.h"
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QString>

DeckViewWidget::DeckViewWidget(DeckManager *deckManager, QWidget *parent)
    : QWidget(parent), m_deckManager(deckManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_listWidget = new QListWidget(this);
    connect(m_listWidget, &QListWidget::itemClicked,
        this, [this](QListWidgetItem* item){
    int row = m_listWidget->row(item);
    const Flashcard& card = m_deckManager->getFlashcards().at(row);
    CardPreviewDialog dlg(card, this);
    dlg.exec();
});

    layout->addWidget(m_listWidget);
    refreshList();

}

void DeckViewWidget::refreshList() {
    m_listWidget->clear();
    const auto& cards = m_deckManager->getFlashcards();

    for (const Flashcard& card : cards) {
        // Always add only the front text
        m_listWidget->addItem(card.getFrontText());
    }
}

