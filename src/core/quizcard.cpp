#include "quizcard.h"
#include <QJsonArray>

QuizCard::QuizCard(const QString& question, const QStringList& options, int correctIndex)
    : m_question(question)
    , m_options(options)
    , m_correctIndex(correctIndex)
{
}

QuizCard::~QuizCard() {
}

QString QuizCard::getFrontText() const {
    return m_question;
}

QString QuizCard::getBackText() const {
    if (m_correctIndex >= 0 && m_correctIndex < m_options.size())
        return m_options[m_correctIndex];
    return QString();
}

bool QuizCard::isQuizCard() const {
    return true;
}

QStringList QuizCard::getOptions() const {
    return m_options;
}

int QuizCard::getCorrectIndex() const {
    return m_correctIndex;
}

bool QuizCard::checkAnswer(int selectedIndex) const {
    return selectedIndex == m_correctIndex;
}

QJsonObject QuizCard::toJson() const {
    QJsonObject obj = Flashcard::toJson();
    obj["type"] = "quiz";
    obj["front"] = m_question;
    obj["correctIndex"] = m_correctIndex;
    
    QJsonArray optionsArray;
    for (const QString& option : m_options) {
        optionsArray.append(option);
    }
    obj["options"] = optionsArray;
    
    return obj;
}