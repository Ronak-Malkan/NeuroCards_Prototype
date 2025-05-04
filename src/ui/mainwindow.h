#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "../core/deckmanager.h"

class DeckListPanel;
class DeckDetailPanel;
class StudyModePanel;
class AddCardDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    DeckManager*      m_deckManager;
    QStackedWidget*   m_stack;

    DeckListPanel*    m_deckListPanel;    // page 0
    DeckDetailPanel*  m_deckDetailPanel;  // page 1
    StudyModePanel*   m_studyPanel;       // page 2

    void setupUi();
    void setupConnections();
};

#endif // MAINWINDOW_H

