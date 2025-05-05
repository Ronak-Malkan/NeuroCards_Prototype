#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QJsonObject>
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
    bool recordCardResult(const QString& deckName, int index, bool wasCorrect);

    // Card management
    QVector<Flashcard> getFlashcards(const QString& deckName) const;
    bool addFlashcardToDeck(const QString& deckName, const Flashcard& card);
    bool removeFlashcardFromDeck(const QString& deckName, int index);
    bool updateFlashcardInDeck(const QString& deckName, int index, const Flashcard& card);

private:
    QMap<QString, QVector<Flashcard>> m_decks;
    QString m_dataFilePath;

    void loadFromFile();
    void saveToFile() const;
};

#endif // DECKMANAGER_H

