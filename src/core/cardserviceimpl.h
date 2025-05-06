#ifndef CARDSERVICEIMPL_H
#define CARDSERVICEIMPL_H

#include "cardservice.h"
#include "deckmanager.h"

/**
 * @brief Implementation of CardService that delegates to DeckManager
 */
class CardServiceImpl : public CardService {
public:
    explicit CardServiceImpl(DeckManager* deckManager);
    virtual ~CardServiceImpl() = default;

    // Deck management
    QStringList getDeckNames() const override;
    bool createDeck(const QString& name) override;
    bool renameDeck(const QString& oldName, const QString& newName) override;
    bool deleteDeck(const QString& name) override;
    
    // Card retrieval
    QVector<Flashcard*> getFlashcards(const QString& deckName) const override;
    QVector<Flashcard*> getDueFlashcards(const QString& deckName) const override;
    
    // Card management
    bool addFlipCard(const QString& deckName, const QString& front, const QString& back) override;
    bool addQuizCard(const QString& deckName, 
                    const QString& question, 
                    const QStringList& options,
                    int correctIndex) override;
    bool removeCard(const QString& deckName, int index) override;
    bool updateFlipCard(const QString& deckName, int index, 
                       const QString& front, const QString& back) override;
    bool updateQuizCard(const QString& deckName, int index,
                       const QString& question, 
                       const QStringList& options,
                       int correctIndex) override;
                              
    // Study functionality
    bool recordCardResult(const QString& deckName, int index, int quality) override;

private:
    DeckManager* m_deckManager;
};

#endif // CARDSERVICEIMPL_H