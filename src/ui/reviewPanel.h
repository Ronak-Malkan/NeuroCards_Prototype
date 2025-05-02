#ifndef REVIEWPANEL_H
#define REVIEWPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include "../core/deckmanager.h"

class ReviewPanel : public QWidget {
    Q_OBJECT

public:
    explicit ReviewPanel(DeckManager* manager, QWidget *parent = nullptr);

signals:
    void reviewExited();  // to return to StudyModePanel

private slots:
    void showNextCard();
    void flipCard();               // for Flip mode
    void checkQuizAnswer();        // for Quiz mode
    void exitReview();             // return

private:
    DeckManager* deckManager_;
    QVector<Flashcard> cards_;
    int currentIndex_;
    bool showingBack_;

    // Common UI
    QVBoxLayout* layout_;
    QLabel* cardLabel_;
    QPushButton* nextButton_;
    QPushButton* exitButton_;

    // Flip
    QPushButton* flipButton_;

    // Quiz
    QVector<QPushButton*> optionButtons_;
    QString correctAnswer_;

    void setupUI();
    void loadCurrentCard();
    void clearLayout(QLayout* layout);
};

#endif // REVIEWPANEL_H

