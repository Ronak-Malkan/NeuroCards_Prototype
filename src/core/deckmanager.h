#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include "flashcard.h"

class DeckManager : public QObject {
    Q_OBJECT
public:
    explicit DeckManager(QObject *parent = nullptr);
    
    QVector<Flashcard> getFlashcards() const;
    bool addFlashcard(const Flashcard &card);
    bool loadDeck();
    bool saveDeck();

signals:
    void deckUpdated();

private:
    QVector<Flashcard> m_flashcards;
    QString m_filePath;
};

#endif // DECKMANAGER_H
