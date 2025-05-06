#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QDate>

// Forward declarations
class SpacedRepetitionData;
class CardStatistics;

/**
 * @brief Abstract base class for all flashcard types
 */
class Flashcard {
public:
    virtual ~Flashcard();
    
    // Common interface for all card types
    virtual QString getFrontText() const = 0;
    virtual QString getBackText() const = 0;
    virtual bool isQuizCard() const = 0;
    
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
    virtual QJsonObject toJson() const;
    static Flashcard* fromJson(const QJsonObject& obj);
    
protected:
    Flashcard();
    
    // JSON serialization helpers for derived classes
    void populateJson(QJsonObject& obj) const;
    void populateFromJson(const QJsonObject& obj);
    
private:
    // Performance tracking
    int m_attempts;
    int m_correctCount;
    
    // SM-2 fields
    int m_repetition;
    int m_interval;
    double m_easeFactor;
    QDate m_nextReview;
};

#endif // FLASHCARD_H
