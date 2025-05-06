#ifndef FLASHCARD_EDITOR_WIDGET_H
#define FLASHCARD_EDITOR_WIDGET_H

#include <QWidget>

class QStackedWidget;
class QTextEdit;
class QPushButton;
class CardService;  // Changed from DeckManager to CardService

class FlashcardEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit FlashcardEditorWidget(CardService* cardService, QWidget *parent = nullptr);

private slots:
    void onFlipButtonClicked();
    void onAddButtonClicked();

private:
    bool m_isFrontVisible;
    CardService* m_cardService;  // Changed from DeckManager to CardService
    
    QStackedWidget *m_stack;
    QTextEdit *m_frontEditor;
    QTextEdit *m_backEditor;
    QPushButton *m_flipButton;
    QPushButton *m_addButton;
};

#endif // FLASHCARD_EDITOR_WIDGET_H
