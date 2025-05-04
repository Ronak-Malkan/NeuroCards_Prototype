#ifndef ADDCARDDIALOG_H
#define ADDCARDDIALOG_H

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

class AddCardDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCardDialog(DeckManager* manager,
                           const QString& initialDeck,
                           QWidget* parent = nullptr);

signals:
    // Emitted after a card is successfully added
    void cardAdded(const QString& deckName);

private slots:
    void onTypeChanged(const QString& type);
    void onAddClicked();
    void onCloseClicked();

private:
    DeckManager*    m_deckManager;
    QString         m_initialDeck;

    QComboBox*      m_typeSelector;
    QComboBox*      m_deckSelector;
    QStackedWidget* m_stack;

    // Flip page
    QWidget*        m_flipPage;
    QLineEdit*      m_flipQuestionInput;
    QLineEdit*      m_flipAnswerInput;

    // Quiz page
    QWidget*        m_quizPage;
    QLineEdit*      m_quizQuestionInput;
    QLineEdit*      m_quizOptionInputs[4];
    QComboBox*      m_quizCorrectSelector;

    QPushButton*    m_addButton;
    QPushButton*    m_closeButton;

    void setupUI();
    void clearFlipInputs();
    void clearQuizInputs();
};

#endif // ADDCARDDIALOG_H

