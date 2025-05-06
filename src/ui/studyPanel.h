#ifndef STUDYPANEL_H
#define STUDYPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QHBoxLayout>
#include "../core/cardservice.h"
#include "../core/flashcard.h"

// Clickable label to flip
class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    using QLabel::QLabel;
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent*) override {
        emit clicked();
    }
};

class StudyPanel : public QWidget {
    Q_OBJECT

public:
    explicit StudyPanel(CardService* cardService,
                        const QString& deckName,
                        QWidget* parent = nullptr);

    // Swap in a different deck at runtime
    void setDeck(const QString& deckName);

    // Reload and reshuffle the current deck
    void reloadDeck();
    void setDueOnly(bool dueOnly);

signals:
    // Sent when the user hits "Exit"
    void exitStudy();

private slots:
    void showPrevCard();
    void showNextCard();
    void flipCard();
    void onGradeClicked();
    void studyExit();

private:
    CardService*       m_cardService;
    QString            m_deckName;
    QVector<int>       m_orderIndices;     // maps display order to original indices
    int                m_currentIndex;
    bool               m_showingFront;
    bool               m_dueOnly;

    // UI widgets
    QVBoxLayout*       m_layout;
    ClickableLabel*    m_cardLabel;
    QPushButton*       m_gradeButtons[4];  // Again, Hard, Good, Easy
    QHBoxLayout*       m_navLayout;
    QPushButton*       m_prevButton;
    QPushButton*       m_nextButton;
    QPushButton*       m_exitButton;

    void setupUI();
    void loadCurrentCard();
};

#endif // STUDYPANEL_H
