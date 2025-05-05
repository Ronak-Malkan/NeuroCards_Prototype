#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QDate>

class Flashcard {
public:
    // Flip-card constructor
    Flashcard(const QString& front, const QString& back);

    // Quiz-card constructor
    Flashcard(const QString& front, const QStringList& options, int correctIndex);

    bool isQuizCard() const;
    QString getFrontText() const;
    QString getBackText() const;
    QStringList getOptions() const;
    int getCorrectOptionIndex() const;

    // Performance stats
    void recordResult(int quality);
    int getAttempts() const;
    int getCorrectCount() const;

    // Spaced-repetition scheduling
    int getRepetition() const;
    int getInterval() const;
    double getEaseFactor() const;
    QDate getNextReview() const;

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

    // SM-2 fields
    int         m_repetition;
    int         m_interval;
    double      m_easeFactor;
    QDate       m_nextReview;
};

#endif // FLASHCARD_H
