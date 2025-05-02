#include "studyModePanel.h"
#include <QHBoxLayout>
#include <QMessageBox>

StudyModePanel::StudyModePanel(DeckManager* manager, QWidget *parent)
    : QWidget(parent), deckManager_(manager), currentMode_(Flip), reviewPanel_(nullptr)
{
    mainLayout_ = new QVBoxLayout(this);

    // Mode selector dropdown
    modeSelector_ = new QComboBox(this);
    modeSelector_->addItems({"Flip", "Quiz"});
    connect(modeSelector_, &QComboBox::currentTextChanged, this, &StudyModePanel::switchMode);

    // Dynamic input container
    contentWidget_ = new QWidget(this);
    contentWidget_->setLayout(new QVBoxLayout());

    mainLayout_->addWidget(modeSelector_);
    mainLayout_->addWidget(contentWidget_);

    // Add Review Mode button
    reviewModeButton_ = new QPushButton("Enter Review Mode", this);
    connect(reviewModeButton_, &QPushButton::clicked, this, &StudyModePanel::enterReviewMode);
    mainLayout_->addWidget(reviewModeButton_);

    setLayout(mainLayout_);

    switchMode("Flip");  // default
}

void StudyModePanel::switchMode(const QString& mode) {
    clearContent();

    if (mode == "Flip") {
        currentMode_ = Flip;
        setupFlipUI();
    } else {
        currentMode_ = Quiz;
        setupQuizUI();
    }
}

void StudyModePanel::setupFlipUI() {
    auto layout = qobject_cast<QVBoxLayout*>(contentWidget_->layout());

    flipQuestionInput_ = new QLineEdit(this);
    flipQuestionInput_->setPlaceholderText("Enter question...");

    flipAnswerInput_ = new QLineEdit(this);
    flipAnswerInput_->setPlaceholderText("Enter answer...");

    QPushButton* addButton = new QPushButton("Add Flip Card", this);
    connect(addButton, &QPushButton::clicked, this, &StudyModePanel::addFlipCard);

    layout->addWidget(new QLabel("Question:"));
    layout->addWidget(flipQuestionInput_);
    layout->addWidget(new QLabel("Answer:"));
    layout->addWidget(flipAnswerInput_);
    layout->addWidget(addButton);
}

void StudyModePanel::setupQuizUI() {
    auto layout = qobject_cast<QVBoxLayout*>(contentWidget_->layout());

    quizQuestionInput_ = new QLineEdit(this);
    quizQuestionInput_->setPlaceholderText("Enter question...");

    layout->addWidget(new QLabel("Question:"));
    layout->addWidget(quizQuestionInput_);

    for (int i = 0; i < 4; ++i) {
        quizOptionInputs_[i] = new QLineEdit(this);
        quizOptionInputs_[i]->setPlaceholderText(QString("Option %1").arg(i + 1));
        layout->addWidget(quizOptionInputs_[i]);
    }

    correctOptionSelector_ = new QComboBox(this);
    correctOptionSelector_->addItems({"Option 1", "Option 2", "Option 3", "Option 4"});

    layout->addWidget(new QLabel("Select Correct Option:"));
    layout->addWidget(correctOptionSelector_);

    QPushButton* addButton = new QPushButton("Add Quiz Card", this);
    connect(addButton, &QPushButton::clicked, this, &StudyModePanel::addQuizCard);
    layout->addWidget(addButton);
}

void StudyModePanel::addFlipCard() {
    QString question = flipQuestionInput_->text().trimmed();
    QString answer = flipAnswerInput_->text().trimmed();

    if (question.isEmpty() || answer.isEmpty()) {
        QMessageBox::warning(this, "Missing Data", "Please fill out both question and answer.");
        return;
    }

    Flashcard card(question, answer);  // standard constructor
    deckManager_->addFlashcard(card);
    emit cardAdded();

    flipQuestionInput_->clear();
    flipAnswerInput_->clear();
}

void StudyModePanel::addQuizCard() {
    QString question = quizQuestionInput_->text().trimmed();
    QStringList options;

    // Collect options
    for (int i = 0; i < 4; ++i) {
        QString opt = quizOptionInputs_[i]->text().trimmed();
        if (opt.isEmpty()) {
            QMessageBox::warning(this, "Missing Data", "Please fill in all 4 options.");
            return;
        }
        options.append(opt);
    }

    // Check question
    if (question.isEmpty()) {
        QMessageBox::warning(this, "Missing Question", "Please enter a question.");
        return;
    }

    int correctIndex = correctOptionSelector_->currentIndex();

    Flashcard card(question, options, correctIndex);

    deckManager_->addFlashcard(card);
    emit cardAdded();

    // Reset form
    quizQuestionInput_->clear();
    for (auto* opt : quizOptionInputs_) opt->clear();
    correctOptionSelector_->setCurrentIndex(0);
}

void StudyModePanel::enterReviewMode() {
    // Hide input UI
    contentWidget_->hide();
    modeSelector_->hide();
    reviewModeButton_->hide();

    // Create review panel if not already
    if (!reviewPanel_) {
        reviewPanel_ = new ReviewPanel(deckManager_, this);
        connect(reviewPanel_, &ReviewPanel::reviewExited, this, &StudyModePanel::exitReviewMode);
        mainLayout_->addWidget(reviewPanel_);
    }

    reviewPanel_->show();
}

void StudyModePanel::exitReviewMode() {
    if (reviewPanel_) {
        reviewPanel_->hide();
    }

    contentWidget_->show();
    modeSelector_->show();
    reviewModeButton_->show();
}

void StudyModePanel::clearContent() {
    QLayout* layout = contentWidget_->layout();
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) delete widget;
        delete item;
    }
}

