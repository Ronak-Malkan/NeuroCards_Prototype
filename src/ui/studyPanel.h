#ifndef STUDYPANEL_H
#define STUDYPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QHBoxLayout>
#include "../core/deckmanager.h"
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
    explicit StudyPanel(DeckManager* manager,
                        const QString& deckName,
                        QWidget* parent = nullptr);

    // Swap in a different deck at runtime
    void setDeck(const QString& deckName);

    // Reload and reshuffle the current deck
    void reloadDeck();

signals:
    // Sent when the user hits "Exit"
    void exitStudy();

private slots:
    void showPrevCard();
    void showNextCard();
    void flipCard();
    void markCorrect();
    void markWrong();
    void studyExit();

private:
    DeckManager*       m_deckManager;
    QString            m_deckName;
    QVector<Flashcard> m_cards;
    QVector<int>       m_orderIndices;     // maps display order to original indices
    int                m_currentIndex;
    bool               m_showingFront;

    // UI widgets
    QVBoxLayout*       m_layout;
    ClickableLabel*    m_cardLabel;
    QHBoxLayout*       m_responseLayout;
    QPushButton*       m_correctButton;
    QPushButton*       m_wrongButton;
    QHBoxLayout*       m_navLayout;
    QPushButton*       m_prevButton;
    QPushButton*       m_nextButton;
    QPushButton*       m_exitButton;

    void setupUI();
    void loadCurrentCard();
};

#endif // STUDYPANEL_H
