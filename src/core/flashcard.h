#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QStringList>
#include <QJsonObject>

class Flashcard {
public:
    // Flip card constructor
    Flashcard(const QString& front, const QString& back);

    // Quiz card constructor
    Flashcard(const QString& front, const QStringList& options, int correctIndex);

    bool isQuizCard() const;
    QString getFrontText() const;
    QString getBackText() const;
    QStringList getOptions() const;
    int getCorrectOptionIndex() const;

    // Stats
    void recordResult(bool correct);
    int getAttempts() const;
    int getCorrectCount() const;

    // JSON persistence
    QJsonObject toJson() const;
    static Flashcard fromJson(const QJsonObject& obj);

private:
    QString     m_front;
    QString     m_back;
    QStringList m_options;
    int         m_correctIndex;

    // Performance tracking
    int         m_attempts;
    int         m_correctCount;
};
#endif // FLASHCARD_H
