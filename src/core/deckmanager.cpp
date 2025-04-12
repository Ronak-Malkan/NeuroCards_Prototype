#include "deckmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QDebug>

DeckManager::DeckManager(QObject *parent)
    : QObject(parent)
{
    // Save the deck in the application's current directory.
    m_filePath = QDir::currentPath() + "/deck.json";
    loadDeck();
}

QVector<Flashcard> DeckManager::flashcards() const {
    return m_flashcards;
}

bool DeckManager::addFlashcard(const Flashcard &card) {
    m_flashcards.append(card);
    if (!saveDeck()) {
        return false;
    }
    emit deckUpdated();
    return true;
}

bool DeckManager::loadDeck() {
    QFile file(m_filePath);
    if (!file.exists())
        return true; // No deck yet; this is not an error.

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open deck file for reading.");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
        return false;

    QJsonArray arr = doc.array();
    m_flashcards.clear();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            m_flashcards.append(Flashcard::fromJson(val.toObject()));
        }
    }
    return true;
}

bool DeckManager::saveDeck() {
    QJsonArray arr;
    for (const Flashcard &card : m_flashcards) {
        arr.append(card.toJson());
    }

    QJsonDocument doc(arr);
    qDebug() << "File path is:" << m_filePath;
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open deck file for writing.");
        return false;
    }
    file.write(doc.toJson());
    file.close();
    return true;
}
