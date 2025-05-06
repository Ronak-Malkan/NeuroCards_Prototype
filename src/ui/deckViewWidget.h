#ifndef DECKVIEWWIDGET_H
#define DECKVIEWWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

class DeckViewWidget : public QWidget {
    Q_OBJECT

public:
    // Now takes a deckName so it knows which deck to show
    explicit DeckViewWidget(DeckManager* deckManager,
                            const QString& deckName,
                            QWidget* parent = nullptr);

    // Refresh list of cards in the current deck
    void refreshList();

signals:
    // Emitted when a card is clicked for preview
    void cardClicked(Flashcard* card);

private:
    DeckManager* m_deckManager;
    QString      m_deckName;
    QListWidget* m_listWidget;

    void setupUI();
    void connectSignals();
};

#endif // DECKVIEWWIDGET_H

