#ifndef REVIEWPANEL_H
#define REVIEWPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

class ReviewPanel : public QWidget {
    Q_OBJECT

public:
    // Now takes deckName so it can fetch the right cards
    explicit ReviewPanel(DeckManager* manager,
                         const QString& deckName,
                         QWidget *parent = nullptr);

    // Reload the deck (e.g. when cards have changed)
    void reloadDeck();

signals:
    void reviewExited();  // tells StudyModePanel to switch back

private slots:
    void showNextCard();
    void checkQuizAnswer();
    void exitReview();

private:
    DeckManager*   deckManager_;
    QString        deckName_;
    QVector<Flashcard> cards_;
    int            currentIndex_;

    // UI elements
    QVBoxLayout*   layout_;
    QLabel*        cardLabel_;
    QPushButton*   exitButton_;    // Exit Review Mode
    QPushButton*   nextButton_;
    QVector<QPushButton*> optionButtons_;
    QString        correctAnswer_;

    void setupUI();
    void loadCurrentCard();
};

#endif // REVIEWPANEL_H

