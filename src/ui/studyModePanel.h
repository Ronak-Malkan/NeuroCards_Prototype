#ifndef STUDYMODEPANEL_H
#define STUDYMODEPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "../core/deckmanager.h"
#include "reviewPanel.h"

class StudyModePanel : public QWidget {
    Q_OBJECT

public:
    explicit StudyModePanel(DeckManager* manager,
                            const QString& deckName,
                            QWidget *parent = nullptr);

    // Change the deck and update UI context
    void setDeck(const QString& deckName);

signals:
    // Emitted when a new card is added (to refresh detail view)
    void cardAdded(const QString& deckName);
    // Emitted to signal exiting study mode
    void exitStudy();

private slots:
    void onBackClicked();
    void switchMode(const QString& mode);
    void addFlipCard();
    void addQuizCard();
    void enterReviewMode();
    void exitReviewMode();

private:
    enum Mode { Flip, Quiz };

    DeckManager*    m_deckManager;
    QString         m_deckName;
    Mode            m_currentMode;

    // Header
    QPushButton*    m_backButton;
    QLabel*         m_titleLabel;

    // Main layout
    QVBoxLayout*    m_mainLayout;
    QComboBox*      m_modeSelector;
    QWidget*        m_contentWidget;

    // Flip inputs
    QLineEdit*      m_flipQuestionInput;
    QLineEdit*      m_flipAnswerInput;

    // Quiz inputs
    QLineEdit*      m_quizQuestionInput;
    QLineEdit*      m_quizOptionInputs[4];
    QComboBox*      m_quizCorrectSelector;

    // Review mode
    QPushButton*    m_reviewModeButton;
    ReviewPanel*    m_reviewPanel;

    void setupHeader();
    void setupFlipUI();
    void setupQuizUI();
    void clearContent();
};

#endif // STUDYMODEPANEL_H
