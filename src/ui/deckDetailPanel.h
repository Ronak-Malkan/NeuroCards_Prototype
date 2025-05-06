#ifndef DECKDETAILPANEL_H
#define DECKDETAILPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include "../core/cardservice.h"

class DeckDetailPanel : public QWidget {
    Q_OBJECT

public:
    explicit DeckDetailPanel(CardService* cardService, QWidget* parent = nullptr);

    // Change to a different deck
    void setDeck(const QString& deckName);

    // Refresh list after external changes
    void refreshList();

signals:
    void backToDeckList();
    void studyAll(const QString& deckName);
    void studyDue(const QString& deckName);
    void addCardRequested(const QString& deckName);

private slots:
    void onStatsClicked();
    void onAddCardClicked();
    void onBackClicked();
    void onItemClicked(QListWidgetItem* item);
    void onListContextMenu(const QPoint& pos);

private:
    CardService*    m_cardService;  // Changed from DeckManager to CardService
    QString         m_deckName;
    
    QPushButton*    m_backButton;
    QLabel*         m_titleLabel;
    QLabel*         m_countLabel;
    QPushButton*    m_statsButton;
    QPushButton*    m_studyDueButton;
    QPushButton*    m_studyAllButton;
    QPushButton*    m_addCardButton;
    QListWidget*    m_listWidget;
};

#endif // DECKDETAILPANEL_H

