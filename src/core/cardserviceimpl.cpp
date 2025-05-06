#include "cardserviceimpl.h"
#include "flashcard.h"
#include "flipcard.h"
#include "quizcard.h"

CardServiceImpl::CardServiceImpl(DeckManager* deckManager)
    : m_deckManager(deckManager)
{
}

// Deck management
QStringList CardServiceImpl::getDeckNames() const {
    return m_deckManager->getDeckNames();
}

bool CardServiceImpl::createDeck(const QString& name) {
    return m_deckManager->createDeck(name);
}

bool CardServiceImpl::renameDeck(const QString& oldName, const QString& newName) {
    return m_deckManager->renameDeck(oldName, newName);
}

bool CardServiceImpl::deleteDeck(const QString& name) {
    return m_deckManager->deleteDeck(name);
}

// Card retrieval
QVector<Flashcard*> CardServiceImpl::getFlashcards(const QString& deckName) const {
    return m_deckManager->getFlashcards(deckName);
}

QVector<Flashcard*> CardServiceImpl::getDueFlashcards(const QString& deckName) const {
    return m_deckManager->getDueFlashcards(deckName);
}

// Card management
bool CardServiceImpl::addFlipCard(const QString& deckName, const QString& front, const QString& back) {
    Flashcard* card = m_deckManager->createFlipCard(front, back);
    if (!m_deckManager->addFlashcardToDeck(deckName, card)) {
        // Clean up if adding fails
        delete card;
        return false;
    }
    return true;
}

bool CardServiceImpl::addQuizCard(const QString& deckName, 
                                const QString& question, 
                                const QStringList& options,
                                int correctIndex) {
    Flashcard* card = m_deckManager->createQuizCard(question, options, correctIndex);
    if (!m_deckManager->addFlashcardToDeck(deckName, card)) {
        // Clean up if adding fails
        delete card;
        return false;
    }
    return true;
}

bool CardServiceImpl::removeCard(const QString& deckName, int index) {
    return m_deckManager->removeFlashcardFromDeck(deckName, index);
}

bool CardServiceImpl::updateFlipCard(const QString& deckName, int index, 
                                   const QString& front, const QString& back) {
    Flashcard* card = m_deckManager->createFlipCard(front, back);
    if (!m_deckManager->updateFlashcardInDeck(deckName, index, card)) {
        // Clean up if updating fails
        delete card;
        return false;
    }
    return true;
}

bool CardServiceImpl::updateQuizCard(const QString& deckName, int index,
                                   const QString& question, 
                                   const QStringList& options,
                                   int correctIndex) {
    Flashcard* card = m_deckManager->createQuizCard(question, options, correctIndex);
    if (!m_deckManager->updateFlashcardInDeck(deckName, index, card)) {
        // Clean up if updating fails
        delete card;
        return false;
    }
    return true;
}

// Study functionality
bool CardServiceImpl::recordCardResult(const QString& deckName, int index, int quality) {
    return m_deckManager->recordCardResult(deckName, index, quality);
}