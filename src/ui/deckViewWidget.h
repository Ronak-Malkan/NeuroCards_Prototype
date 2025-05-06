#ifndef DECKVIEWWIDGET_H
#define DECKVIEWWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include "../core/cardservice.h"
#include "../core/flashcard.h"

class DeckViewWidget : public QWidget {
    Q_OBJECT

public:
    // Now takes a deckName so it knows which deck to show
    explicit DeckViewWidget(CardService* cardService,
                            const QString& deckName,
                            QWidget* parent = nullptr);

    // Refresh list of cards in the current deck
    void refreshList();

signals:
    // Emitted when a card is clicked for preview
    void cardClicked(int cardIndex);

private:
    CardService* m_cardService;
    QString      m_deckName;
    QListWidget* m_listWidget;

    void setupUI();
    void connectSignals();
};

#endif // DECKVIEWWIDGET_H

