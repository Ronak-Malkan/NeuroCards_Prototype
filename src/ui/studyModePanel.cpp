#include "studyModePanel.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <random>

StudyModePanel::StudyModePanel(DeckManager* manager,
                               const QString& deckName,
                               QWidget *parent)
    : QWidget(parent)
    , m_deckManager(manager)
    , m_deckName(deckName)
    , m_currentMode(Flip)
    , m_reviewPanel(nullptr)
{
    // Main vertical layout
    m_mainLayout = new QVBoxLayout(this);

    // Header bar
    setupHeader();

    // Mode selector dropdown
    m_modeSelector = new QComboBox(this);
    m_modeSelector->addItems({ tr("Flip"), tr("Quiz") });
    connect(m_modeSelector, &QComboBox::currentTextChanged,
            this, &StudyModePanel::switchMode);
    m_mainLayout->addWidget(m_modeSelector);

    // Dynamic content area
    m_contentWidget = new QWidget(this);
    m_contentWidget->setLayout(new QVBoxLayout());
    m_mainLayout->addWidget(m_contentWidget);

    // Review mode button
    m_reviewModeButton = new QPushButton(tr("Enter Review Mode"), this);
    connect(m_reviewModeButton, &QPushButton::clicked,
            this, &StudyModePanel::enterReviewMode);
    m_mainLayout->addWidget(m_reviewModeButton);

    setLayout(m_mainLayout);

    // Initialize in Flip mode
    switchMode(tr("Flip"));
}

void StudyModePanel::setupHeader() {
    auto* headerLayout = new QHBoxLayout();
    m_backButton = new QPushButton(tr("← Back"), this);
    connect(m_backButton, &QPushButton::clicked,
            this, &StudyModePanel::onBackClicked);
    m_titleLabel = new QLabel(tr("Study: %1").arg(m_deckName), this);
    m_titleLabel->setStyleSheet("font-weight: bold;");
    headerLayout->addWidget(m_backButton);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    m_mainLayout->addLayout(headerLayout);
}

void StudyModePanel::setDeck(const QString& deckName) {
    m_deckName = deckName;
    m_titleLabel->setText(tr("Study: %1").arg(deckName));
    if (m_reviewPanel) {
        m_reviewPanel->reloadDeck();
    }
}

void StudyModePanel::onBackClicked() {
    emit exitStudy();
}

void StudyModePanel::switchMode(const QString& mode) {
    clearContent();
    if (mode == tr("Flip")) {
        m_currentMode = Flip;
        setupFlipUI();
    } else {
        m_currentMode = Quiz;
        setupQuizUI();
    }
}

void StudyModePanel::setupFlipUI() {
    auto* layout = qobject_cast<QVBoxLayout*>(m_contentWidget->layout());
    m_flipQuestionInput = new QLineEdit(this);
    m_flipQuestionInput->setPlaceholderText(tr("Enter question..."));
    m_flipAnswerInput = new QLineEdit(this);
    m_flipAnswerInput->setPlaceholderText(tr("Enter answer..."));
    QPushButton* addBtn = new QPushButton(tr("Add Flip Card"), this);
    connect(addBtn, &QPushButton::clicked, this, &StudyModePanel::addFlipCard);
    layout->addWidget(new QLabel(tr("Question:"), this));
    layout->addWidget(m_flipQuestionInput);
    layout->addWidget(new QLabel(tr("Answer:"), this));
    layout->addWidget(m_flipAnswerInput);
    layout->addWidget(addBtn);
}

void StudyModePanel::setupQuizUI() {
    auto* layout = qobject_cast<QVBoxLayout*>(m_contentWidget->layout());
    m_quizQuestionInput = new QLineEdit(this);
    m_quizQuestionInput->setPlaceholderText(tr("Enter question..."));
    layout->addWidget(new QLabel(tr("Question:"), this));
    layout->addWidget(m_quizQuestionInput);
    for (int i = 0; i < 4; ++i) {
        m_quizOptionInputs[i] = new QLineEdit(this);
        m_quizOptionInputs[i]->setPlaceholderText(tr("Option %1").arg(i+1));
        layout->addWidget(m_quizOptionInputs[i]);
    }
    m_quizCorrectSelector = new QComboBox(this);
    m_quizCorrectSelector->addItems({ tr("Option 1"), tr("Option 2"),
                                     tr("Option 3"), tr("Option 4") });
    layout->addWidget(new QLabel(tr("Correct Option:"), this));
    layout->addWidget(m_quizCorrectSelector);
    QPushButton* addBtn = new QPushButton(tr("Add Quiz Card"), this);
    connect(addBtn, &QPushButton::clicked, this, &StudyModePanel::addQuizCard);
    layout->addWidget(addBtn);
}

void StudyModePanel::addFlipCard() {
    QString q = m_flipQuestionInput->text().trimmed();
    QString a = m_flipAnswerInput->text().trimmed();
    if (q.isEmpty() || a.isEmpty()) {
        QMessageBox::warning(this, tr("Missing Data"),
                             tr("Fill both question and answer."));
        return;
    }
    Flashcard card(q, a);
    m_deckManager->addFlashcardToDeck(m_deckName, card);
    emit cardAdded(m_deckName);
    m_flipQuestionInput->clear();
    m_flipAnswerInput->clear();
}

void StudyModePanel::addQuizCard() {
    QString q = m_quizQuestionInput->text().trimmed();
    if (q.isEmpty()) {
        QMessageBox::warning(this, tr("Missing Data"),
                             tr("Enter a question."));
        return;
    }
    QStringList opts;
    for (int i = 0; i < 4; ++i) {
        QString o = m_quizOptionInputs[i]->text().trimmed();
        if (o.isEmpty()) {
            QMessageBox::warning(this, tr("Missing Data"),
                                 tr("Fill all options."));
            return;
        }
        opts << o;
    }
    int idx = m_quizCorrectSelector->currentIndex();
    Flashcard card(q, opts, idx);
    m_deckManager->addFlashcardToDeck(m_deckName, card);
    emit cardAdded(m_deckName);
    m_quizQuestionInput->clear();
    for (auto* w: m_quizOptionInputs) w->clear();
    m_quizCorrectSelector->setCurrentIndex(0);
}

void StudyModePanel::enterReviewMode() {
    m_contentWidget->hide();
    m_modeSelector->hide();
    m_reviewModeButton->hide();
    if (!m_reviewPanel) {
        m_reviewPanel = new ReviewPanel(m_deckManager, m_deckName, this);
        connect(m_reviewPanel, &ReviewPanel::reviewExited,
                this, &StudyModePanel::exitReviewMode);
        m_mainLayout->addWidget(m_reviewPanel);
    }
    m_reviewPanel->reloadDeck();
    m_reviewPanel->show();
}

void StudyModePanel::exitReviewMode() {
    if (m_reviewPanel) m_reviewPanel->hide();
    m_modeSelector->show();
    m_contentWidget->show();
    m_reviewModeButton->show();
}

void StudyModePanel::clearContent() {
    auto* layout = m_contentWidget->layout();
    QLayoutItem* it;
    while ((it = layout->takeAt(0))) {
        if (it->widget()) delete it->widget();
        delete it;
    }
}
