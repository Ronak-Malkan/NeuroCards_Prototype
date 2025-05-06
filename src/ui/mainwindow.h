#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// Forward declarations
class DeckManager;
class CardService;
class DeckListPanel;
class DeckDetailPanel;
class FlashcardEditorWidget;
class StudyPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDeckSelected(const QString &deckName);
    void onBackToDeckList();
    void onAddCardRequested(const QString &deckName);
    void onStudyDue(const QString &deckName);
    void onStudyAll(const QString &deckName);
    void onExitStudy();

private:
    QStackedWidget *m_stackedWidget;
    DeckManager *m_deckManager;  // We keep this as a member for ownership
    CardService *m_cardService;
    DeckListPanel *m_deckListPanel;
    DeckDetailPanel *m_deckDetailPanel;
    FlashcardEditorWidget *m_cardEditorWidget;
    StudyPanel *m_studyPanel;
};

#endif // MAINWINDOW_H

