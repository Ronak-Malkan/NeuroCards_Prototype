#ifndef STUDYPANEL_H
#define STUDYPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

// A QLabel subclass that emits clicked() when pressed
class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    using QLabel::QLabel;
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* ev) override {
        emit clicked();
    }
};

class StudyPanel : public QWidget {
    Q_OBJECT

public:
    explicit StudyPanel(DeckManager* manager,
                         const QString& deckName,
                         QWidget* parent = nullptr);

    // Reloads and reshuffles the deck (e.g. after adding cards)
    void reloadDeck();
    void setDeck(const QString& deckName);

signals:
    // Emitted when the user clicks “Exit”
    void exitStudy();

private slots:
    void showPrevCard();
    void showNextCard();
    void flipCard();
    void onExitClicked();

private:
    DeckManager*       m_deckManager;
    QString            m_deckName;
    QVector<Flashcard> m_cards;
    int                m_currentIndex;
    bool               m_showingFront;
    QString            m_correctAnswer;

    // UI
    QVBoxLayout*       m_layout;
    ClickableLabel*    m_cardLabel;
    QPushButton*       m_prevButton;
    QPushButton*       m_nextButton;
    QPushButton*       m_exitButton;

    void setupUI();
    void loadCurrentCard();
};

#endif // STUDYPANEL_H

