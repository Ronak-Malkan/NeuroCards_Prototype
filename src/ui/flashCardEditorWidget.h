#ifndef FLASHCARDEDITORWIDGET_H
#define FLASHCARDEDITORWIDGET_H

#include <QWidget>

class QStackedWidget;
class QTextEdit;
class QPushButton;
class DeckManager;

class FlashcardEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit FlashcardEditorWidget(DeckManager *deckManager, QWidget *parent = nullptr);

private slots:
    void onFlipButtonClicked();
    void onAddButtonClicked();

private:
    QStackedWidget *m_stack;
    QTextEdit *m_frontEditor;
    QTextEdit *m_backEditor;
    QPushButton *m_flipButton;
    QPushButton *m_addButton;
    bool m_isFrontVisible;
    DeckManager *m_deckManager;
};

#endif // FLASHCARDEDITORWIDGET_H
