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
    refreshList();

}

void DeckViewWidget::refreshList() {
    m_listWidget->clear();
    const auto& cards = m_deckManager->getFlashcards();

    for (const Flashcard& card : cards) {
        QString display;

        if (card.isQuizCard()) {
            display += "Quiz: " + card.getFrontText();
            display += "\nOptions: ";
            QStringList opts = card.getOptions();
            for (int i = 0; i < opts.size(); ++i) {
                if (i > 0) display += " | ";
                display += QString("%1. %2").arg(i + 1).arg(opts[i]);
            }
            display += "\n→  " + card.getOptions().at(card.getCorrectOptionIndex());
        } else {
            display += "Flip: " + card.getFrontText();
            display += "\n→ " + card.getBackText();
        }

        m_listWidget->addItem(display);
    }
}

