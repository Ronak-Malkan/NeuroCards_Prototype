#include "deckmanager.h"
#include "flipcard.h"
#include "quizcard.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

DeckManager::DeckManager(QObject* parent)
    : QObject(parent)
{
    // Determine storage path
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    m_dataFilePath = dataDir + QDir::separator() + "decks.json";

    loadFromFile();

    if (m_decks.isEmpty() || !m_decks.contains("Default")) {
        m_decks.insert("Default", QVector<Flashcard*>{});
        saveToFile();
    }
}

DeckManager::~DeckManager() {
    clearDecks();
}

// Clear all flashcard objects to prevent memory leaks
void DeckManager::clearDecks() {
    for (auto& deck : m_decks) {
        for (Flashcard* card : deck) {
            delete card;
        }
        deck.clear();
    }
    m_decks.clear();
}

// Deck names
QStringList DeckManager::getDeckNames() const {
    return m_decks.keys();
}

bool DeckManager::createDeck(const QString& name) {
    if (name.isEmpty() || m_decks.contains(name)) return false;
    m_decks.insert(name, {});
    saveToFile();
    return true;
}

bool DeckManager::renameDeck(const QString& oldName, const QString& newName) {
    if (!m_decks.contains(oldName) || newName.isEmpty() || m_decks.contains(newName))
        return false;
    m_decks.insert(newName, m_decks.take(oldName));
    saveToFile();
    return true;
}

bool DeckManager::deleteDeck(const QString& name) {
    bool removed = m_decks.remove(name) > 0;
    if (removed) saveToFile();
    return removed;
}

bool DeckManager::recordCardResult(const QString& deckName, int index, int quality) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;

    vec[index]->recordResult(quality);

    saveToFile();
    return true;
}

QVector<Flashcard*> DeckManager::getFlashcards(const QString& deckName) const {
    return m_decks.value(deckName);
}


QVector<Flashcard*> DeckManager::getDueFlashcards(const QString& deckName) const {
    QVector<Flashcard*> due;
    for (Flashcard* card : m_decks.value(deckName)) {
        if (card->getNextReview() <= QDate::currentDate())
            due.append(card);
    }
    return due;
}

bool DeckManager::addFlashcardToDeck(const QString& deckName, Flashcard* card) {
    if (!m_decks.contains(deckName) || !card) return false;
    m_decks[deckName].append(card);
    saveToFile();
    return true;
}

bool DeckManager::updateFlashcardInDeck(const QString& deckName, int index, Flashcard* card) {
    if (!m_decks.contains(deckName) || !card) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    
    // Delete old card and replace with new one
    delete vec[index];
    vec[index] = card;
    saveToFile();
    return true;
}

bool DeckManager::removeFlashcardFromDeck(const QString& deckName, int index) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    
    // Delete the flashcard to prevent memory leak
    delete vec[index];
    vec.removeAt(index);
    saveToFile();
    return true;
}

// Factory methods for creating flashcards
Flashcard* DeckManager::createFlipCard(const QString& front, const QString& back) const {
    return new FlipCard(front, back);
}

Flashcard* DeckManager::createQuizCard(const QString& question, const QStringList& options, int correctIndex) const {
    return new QuizCard(question, options, correctIndex);
}

// Persistence
void DeckManager::loadFromFile() {
    // Clear any existing data first
    clearDecks();
    
    QFile file(m_dataFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No existing data file, starting fresh:" << m_dataFilePath;
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;
    QJsonObject root = doc.object();

    for (auto it = root.begin(); it != root.end(); ++it) {
        QString deckName = it.key();
        QJsonArray arr = it.value().toArray();
        QVector<Flashcard*> cards;
        for (auto v : arr) {
            Flashcard* card = Flashcard::fromJson(v.toObject());
            if (card) {
                cards.append(card);
            }
        }
        m_decks[deckName] = std::move(cards);
    }
}

void DeckManager::saveToFile() const {
    QJsonObject root;
    for (auto it = m_decks.begin(); it != m_decks.end(); ++it) {
        const QString& deckName = it.key();
        const QVector<Flashcard*>& vec = it.value();
        QJsonArray arr;
        for (const Flashcard* card : vec) {
            if (card) {
                arr.append(card->toJson());
            }
        }
        root[deckName] = arr;
    }
    QJsonDocument doc(root);
    QFile file(m_dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open data file for writing:" << m_dataFilePath;
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

