#include "deckmanager.h"
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
        m_decks.insert("Default", QVector<Flashcard>{});
        saveToFile();
    }
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

bool DeckManager::recordCardResult(const QString& deckName, int index, bool wasCorrect) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    vec[index].recordResult(wasCorrect);
    saveToFile();
    return true;
}

// Cards
QVector<Flashcard> DeckManager::getFlashcards(const QString& deckName) const {
    return m_decks.value(deckName);
}

bool DeckManager::addFlashcardToDeck(const QString& deckName, const Flashcard& card) {
    if (!m_decks.contains(deckName)) return false;
    m_decks[deckName].append(card);
    saveToFile();
    return true;
}

bool DeckManager::updateFlashcardInDeck(const QString& deckName, int index, const Flashcard& card) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    vec[index] = card;
    saveToFile();
    return true;
}

bool DeckManager::removeFlashcardFromDeck(const QString& deckName, int index) {
    if (!m_decks.contains(deckName)) return false;
    auto& vec = m_decks[deckName];
    if (index < 0 || index >= vec.size()) return false;
    vec.removeAt(index);
    saveToFile();
    return true;
}

// Persistence
void DeckManager::loadFromFile() {
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

    m_decks.clear();
    for (auto it = root.begin(); it != root.end(); ++it) {
        QString deckName = it.key();
        QJsonArray arr = it.value().toArray();
        QVector<Flashcard> cards;
        for (auto v : arr)
            cards.append(Flashcard::fromJson(v.toObject()));
        m_decks.insert(deckName, cards);
    }
}

void DeckManager::saveToFile() const {
    QJsonObject root;
    for (auto it = m_decks.begin(); it != m_decks.end(); ++it) {
        const QString& deckName = it.key();
        const QVector<Flashcard>& vec = it.value();
        QJsonArray arr;
        for (const Flashcard& card : vec)
            arr.append(card.toJson());
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

