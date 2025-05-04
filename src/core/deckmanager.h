#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include "flashcard.h"

class DeckManager : public QObject {
    Q_OBJECT

public:
    explicit DeckManager(QObject* parent = nullptr);

    // Deck management
    QStringList getDeckNames() const;
    bool createDeck(const QString& name);
    bool renameDeck(const QString& oldName, const QString& newName);
    bool deleteDeck(const QString& name);

    // Card management
    QVector<Flashcard> getFlashcards(const QString& deckName) const;
    bool addFlashcardToDeck(const QString& deckName, const Flashcard& card);
    bool removeFlashcardFromDeck(const QString& deckName, int index);

private:
    QMap<QString, QVector<Flashcard>> m_decks;
};

#endif // DECKMANAGER_H

