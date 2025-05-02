#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QStringList>
#include <QJsonObject>

class Flashcard {
public:
    enum class CardType { Flip, Quiz };

    // Flip constructor
    Flashcard(const QString& front, const QString& back);

    // Quiz constructor
    Flashcard(const QString& question, const QStringList& options, int correctIndex);

    // JSON serialization
    QJsonObject toJson() const;
    static Flashcard fromJson(const QJsonObject& obj);

    // Getters
    QString getFrontText() const;
    QString getBackText() const;
    CardType getType() const;
    QStringList getOptions() const;
    int getCorrectOptionIndex() const;
    bool isQuizCard() const;

private:
    QString frontText_;
    QString backText_;
    CardType type_;

    // For Quiz cards
    QStringList options_;
    int correctOptionIndex_;
};

#endif // FLASHCARD_H

