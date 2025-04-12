#include "deckViewWidget.h"
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
    layout->addWidget(m_listWidget);
    refreshView();

    connect(m_deckManager, &DeckManager::deckUpdated, this, &DeckViewWidget::refreshView);
}

void DeckViewWidget::refreshView() {
    m_listWidget->clear();
    const QVector<Flashcard> &cards = m_deckManager->flashcards();
    for (const Flashcard &card : cards) {
        m_listWidget->addItem("Front: " + card.getFront() + "\nBack: " + card.getBack());
    }
}
