#include "flashcard.h"
#include <QJsonArray>

// Flip-card
Flashcard::Flashcard(const QString& front, const QString& back)
    : m_front(front)
    , m_back(back)
    , m_options()
    , m_correctIndex(-1)
    , m_attempts(0)
    , m_correctCount(0)
    , m_repetition(0)
    , m_interval(1)
    , m_easeFactor(2.5)
    , m_nextReview(QDate::currentDate())
{
}

// Quiz-card
Flashcard::Flashcard(const QString& front,
                     const QStringList& options,
                     int correctIndex)
    : m_front(front)
    , m_back(QString())
    , m_options(options)
    , m_correctIndex(correctIndex)
    , m_attempts(0)
    , m_correctCount(0)
    , m_repetition(0)
    , m_interval(1)
    , m_easeFactor(2.5)
    , m_nextReview(QDate::currentDate())
{
}

bool Flashcard::isQuizCard() const {
    return m_correctIndex >= 0 && m_correctIndex < m_options.size();
}

QString Flashcard::getFrontText() const { return m_front; }
QString Flashcard::getBackText() const  { return m_back;  }
QStringList Flashcard::getOptions() const { return m_options; }
int Flashcard::getCorrectOptionIndex() const { return m_correctIndex; }


void Flashcard::recordResult(int quality) {
    // Basic stats
    m_attempts++;
    if (quality >= 3) m_correctCount++;

    qDebug() << "[DEBUG MODEL] recordResult front='" << m_front
         << "' quality=" << quality
         << " → rep="    << m_repetition
         << " int="      << m_interval
         << " ef="       << m_easeFactor
         << " attempts=" << m_attempts
         << " correct="  << m_correctCount;

    // SM-2 scheduling
    if (quality < 3) {
        m_repetition = 0;
        m_interval   = 1;
    } else {
        m_repetition++;
        if (m_repetition == 1)
            m_interval = 1;
        else if (m_repetition == 2)
            m_interval = 6;
        else
            m_interval = qRound(m_interval * m_easeFactor);

        m_easeFactor += (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
        if (m_easeFactor < 1.3) m_easeFactor = 1.3;
    }

    m_nextReview = QDate::currentDate().addDays(m_interval);
}

int Flashcard::getAttempts() const    { return m_attempts;    }
int Flashcard::getCorrectCount() const { return m_correctCount; }

// Spaced repetition getters
int Flashcard::getRepetition() const   { return m_repetition;   }
int Flashcard::getInterval() const     { return m_interval;     }
double Flashcard::getEaseFactor() const { return m_easeFactor;   }
QDate Flashcard::getNextReview() const  { return m_nextReview;  }

// JSON
QJsonObject Flashcard::toJson() const {
    QJsonObject obj;
    if (isQuizCard()) {
        obj["type"] = "quiz";
        obj["front"] = m_front;
        QJsonArray arr;
        for (const QString& opt : m_options)
            arr.append(opt);
        obj["options"] = arr;
        obj["correctIndex"] = m_correctIndex;
    } else {
        obj["type"] = "flip";
        obj["front"] = m_front;
        obj["back"]  = m_back;
    }
    // Stats
    QJsonObject stats;
    stats["attempts"]     = m_attempts;
    stats["correctCount"] = m_correctCount;
    obj["stats"] = stats;

    // Scheduling
    QJsonObject sched;
    sched["repetition"]  = m_repetition;
    sched["interval"]    = m_interval;
    sched["easeFactor"]  = m_easeFactor;
    sched["nextReview"]  = m_nextReview.toString(Qt::ISODate);
    obj["schedule"] = sched;

    return obj;
}

Flashcard Flashcard::fromJson(const QJsonObject& obj) {
    QString type = obj.value("type").toString();
    Flashcard card = [&]() {
        if (type == "quiz") {
            QString front = obj.value("front").toString();
            QJsonArray arr = obj.value("options").toArray();
            QStringList options;
            for (const QJsonValue& v : arr)
                options.append(v.toString());
            int correct = obj.value("correctIndex").toInt();
            return Flashcard(front, options, correct);
        } else {
            QString front = obj.value("front").toString();
            QString back  = obj.value("back").toString();
            return Flashcard(front, back);
        }
    }();
    // Stats
    if (obj.contains("stats") && obj.value("stats").isObject()) {
        QJsonObject s = obj.value("stats").toObject();
        card.m_attempts     = s.value("attempts").toInt(0);
        card.m_correctCount = s.value("correctCount").toInt(0);
    }
    // Scheduling
    if (obj.contains("schedule") && obj.value("schedule").isObject()) {
        QJsonObject s = obj.value("schedule").toObject();
        card.m_repetition  = s.value("repetition").toInt(0);
        card.m_interval    = s.value("interval").toInt(1);
        card.m_easeFactor  = s.value("easeFactor").toDouble(2.5);
        card.m_nextReview  = QDate::fromString(
            s.value("nextReview").toString(), Qt::ISODate);
    }
    return card;
}
