#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <memory>
#include "flashcard.h"

class DeckManager : public QObject {
    Q_OBJECT

public:
    explicit DeckManager(QObject* parent = nullptr);
    ~DeckManager();

    // Deck management
    QStringList getDeckNames() const;
    bool createDeck(const QString& name);
    bool renameDeck(const QString& oldName, const QString& newName);
    bool deleteDeck(const QString& name);
    bool recordCardResult(const QString& deckName, int index, int quality);

    // Card management
    QVector<Flashcard*> getFlashcards(const QString& deckName) const;
    QVector<Flashcard*> getDueFlashcards(const QString& deckName) const;
    bool addFlashcardToDeck(const QString& deckName, Flashcard* card);
    bool removeFlashcardFromDeck(const QString& deckName, int index);
    bool updateFlashcardInDeck(const QString& deckName, int index, Flashcard* card);
    
    // Factory methods for creating cards
    Flashcard* createFlipCard(const QString& front, const QString& back) const;
    Flashcard* createQuizCard(const QString& question, const QStringList& options, int correctIndex) const;

private:
    QMap<QString, QVector<Flashcard*>> m_decks;
    QString m_dataFilePath;

    void loadFromFile();
    void saveToFile() const;
    void clearDecks();
};

#endif // DECKMANAGER_H

