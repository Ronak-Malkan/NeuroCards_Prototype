#ifndef FLIPCARD_H
#define FLIPCARD_H

#include "flashcard.h"

/**
 * @brief A simple two-sided flashcard with front and back text
 */
class FlipCard : public Flashcard {
public:
    FlipCard(const QString& front, const QString& back);
    virtual ~FlipCard();
    
    // Implement abstract methods from Flashcard
    QString getFrontText() const override;
    QString getBackText() const override;
    bool isQuizCard() const override;
    
    // JSON persistence
    QJsonObject toJson() const override;
    
private:
    QString m_front;
    QString m_back;
};

#endif // FLIPCARD_H