#include "deckmanager.h"

DeckManager::DeckManager(QObject* parent)
    : QObject(parent)
{
    // Optionally create a default deck:
    m_decks.insert("Default", {});
}

QStringList DeckManager::getDeckNames() const {
    return m_decks.keys();
}

bool DeckManager::createDeck(const QString& name) {
    if (name.isEmpty() || m_decks.contains(name)) return false;
    m_decks.insert(name, {});
    return true;
}

bool DeckManager::renameDeck(const QString& oldName, const QString& newName) {
    if (!m_decks.contains(oldName) || newName.isEmpty() || m_decks.contains(newName))
        return false;
    m_decks.insert(newName, m_decks.take(oldName));
    return true;
}

bool DeckManager::deleteDeck(const QString& name) {
    return m_decks.remove(name) > 0;
}

QVector<Flashcard> DeckManager::getFlashcards(const QString& deckName) const {
    return m_decks.value(deckName);
}

bool DeckManager::addFlashcardToDeck(const QString& deckName, const Flashcard& card) {
    if (!m_decks.contains(deckName)) return false;
    m_decks[deckName].append(card);
    return true;
}

bool DeckManager::removeFlashcardFromDeck(const QString& deckName, int index) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    vec.removeAt(index);
    return true;
}

