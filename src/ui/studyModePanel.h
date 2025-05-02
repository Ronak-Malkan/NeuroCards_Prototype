#ifndef STUDYMODEPANEL_H
#define STUDYMODEPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include "../core/deckmanager.h"
#include "reviewPanel.h"
class StudyModePanel : public QWidget {
    Q_OBJECT

public:
    explicit StudyModePanel(DeckManager* manager, QWidget *parent = nullptr);

signals:
    void cardAdded();  // signal to refresh deck if needed

private slots:
    void switchMode(const QString& mode);
    void addFlipCard();
    void addQuizCard();
    void enterReviewMode();
    void exitReviewMode();

private:
    enum Mode { Flip, Quiz };
    Mode currentMode_;

    DeckManager* deckManager_;
    QVBoxLayout* mainLayout_;
    QComboBox* modeSelector_;
    QWidget* contentWidget_;  // container for dynamic inputs

    // Flip inputs
    QLineEdit* flipQuestionInput_;
    QLineEdit* flipAnswerInput_;

    // Quiz inputs
    QLineEdit* quizQuestionInput_;
    QLineEdit* quizOptionInputs_[4];
    QComboBox* correctOptionSelector_;

    // Review mode
    QPushButton* reviewModeButton_;
    ReviewPanel* reviewPanel_;

    void setupFlipUI();
    void setupQuizUI();
    void clearContent();
};

#endif // STUDYMODEPANEL_H

