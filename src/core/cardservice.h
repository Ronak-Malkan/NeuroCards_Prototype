#ifndef CARDSERVICE_H
#define CARDSERVICE_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>

// Forward declarations
class Flashcard;

/**
 * @brief Service interface to provide UI with access to card operations
 * 
 * This interface decouples UI components from direct dependencies on
 * the core domain model, making it easier to change implementations.
 */
class CardService {
public:
    virtual ~CardService() = default;

    // Deck management
    virtual QStringList getDeckNames() const = 0;
    virtual bool createDeck(const QString& name) = 0;
    virtual bool renameDeck(const QString& oldName, const QString& newName) = 0;
    virtual bool deleteDeck(const QString& name) = 0;
    
    // Card retrieval
    virtual QVector<Flashcard*> getFlashcards(const QString& deckName) const = 0;
    virtual QVector<Flashcard*> getDueFlashcards(const QString& deckName) const = 0;
    
    // Card management
    virtual bool addFlipCard(const QString& deckName, const QString& front, const QString& back) = 0;
    virtual bool addQuizCard(const QString& deckName, 
                           const QString& question, 
                           const QStringList& options,
                           int correctIndex) = 0;
    virtual bool removeCard(const QString& deckName, int index) = 0;
    virtual bool updateFlipCard(const QString& deckName, int index, 
                              const QString& front, const QString& back) = 0;
    virtual bool updateQuizCard(const QString& deckName, int index,
                              const QString& question, 
                              const QStringList& options,
                              int correctIndex) = 0;
                              
    // Study functionality
    virtual bool recordCardResult(const QString& deckName, int index, int quality) = 0;
};

#endif // CARDSERVICE_H