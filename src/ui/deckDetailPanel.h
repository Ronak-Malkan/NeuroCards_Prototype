#ifndef DECKDETAILPANEL_H
#define DECKDETAILPANEL_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../core/deckmanager.h"
#include "cardPreviewDialog.h"

class DeckDetailPanel : public QWidget {
    Q_OBJECT
public:
    explicit DeckDetailPanel(DeckManager* manager, QWidget* parent = nullptr);

    // Set which deck to display
    void setDeck(const QString& deckName);

signals:
    void backToDeckList();
    void startStudy(const QString& deckName);
    void addCardRequested(const QString& deckName);

private slots:
    void onBackClicked();
    void onStudyClicked();
    void onAddCardClicked();
    void onItemClicked(QListWidgetItem* item);

private:
    DeckManager*    m_deckManager;
    QString         m_deckName;

    // UI Elements
    QPushButton*    m_backButton;
    QLabel*         m_titleLabel;
    QLabel*         m_countLabel;
    QPushButton*    m_studyButton;
    QPushButton*    m_addCardButton;
    QListWidget*    m_listWidget;

    void setupUI();
    void connectSignals();
    void refreshList();
};

#endif // DECKDETAILPANEL_H
