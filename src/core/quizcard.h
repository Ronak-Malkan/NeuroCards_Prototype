#ifndef QUIZCARD_H
#define QUIZCARD_H

#include "flashcard.h"
#include <QStringList>

/**
 * @brief A multiple choice quiz card with options and a correct answer
 */
class QuizCard : public Flashcard {
public:
    QuizCard(const QString& question, const QStringList& options, int correctIndex);
    virtual ~QuizCard();
    
    // Implement abstract methods from Flashcard
    QString getFrontText() const override;
    QString getBackText() const override;
    bool isQuizCard() const override;
    
    // Quiz-specific functionality
    QStringList getOptions() const;
    int getCorrectIndex() const;
    bool checkAnswer(int selectedIndex) const;
    
    // JSON persistence
    QJsonObject toJson() const override;
    
private:
    QString m_question;
    QStringList m_options;
    int m_correctIndex;
};

#endif // QUIZCARD_H