#ifndef ADDCARDDIALOG_H
#define ADDCARDDIALOG_H

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include "../core/cardservice.h"

class AddCardDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCardDialog(CardService* cardService,
                           const QString& deckName,
                           int editIndex = -1,
                           QWidget* parent = nullptr);

signals:
    void cardAdded(const QString& deckName);
    void cardEdited(const QString& deckName, int index);

private slots:
    void onTypeChanged(const QString& type);
    void onAddClicked();
    void onCloseClicked();

private:
    CardService*     m_cardService;
    QString         m_initialDeck;
    int             m_editIndex;

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

