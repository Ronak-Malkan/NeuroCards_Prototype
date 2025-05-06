# Design Principles and Patterns in NeuroCards

This document outlines the key design principles and patterns used in the NeuroCards application, with code examples showing their implementation.

## Design Principles

### 1. Single Responsibility Principle (SRP)

Each class in the NeuroCards application has a single responsibility, which helps in maintaining and extending the code.

**Example:** The separation of `Flashcard` (data model) from `CardDisplayWidget` (visualization)

```cpp
// Flashcard has the responsibility of managing card data
class Flashcard {
public:
    virtual QString getFrontText() const = 0;
    virtual QString getBackText() const = 0;
    virtual bool isQuizCard() const = 0;
    // ...
};

// CardDisplayWidget has the responsibility of displaying cards
class CardDisplayWidget {
private:
    // UI components for display
    QLabel* m_frontText;
    QLabel* m_backText;
    // ...
};
```

### 2. Open/Closed Principle (OCP)

The application's architecture allows for extension without modification of existing code.

**Example:** The `Flashcard` base class and its derived classes

```cpp
// Base class is open for extension but closed for modification
class Flashcard {
public:
    virtual QString getFrontText() const = 0;
    virtual QString getBackText() const = 0;
    virtual bool isQuizCard() const = 0;
    // ...
};

// Adding a new card type doesn't require modifying the Flashcard class
class FlipCard : public Flashcard {
    // Implementation specific to FlipCard
};

class QuizCard : public Flashcard {
    // Implementation specific to QuizCard
};

// Potential future extension without modifying existing code
class ImageCard : public Flashcard {
    // New card type added without changing base class
};
```

### 3. Liskov Substitution Principle (LSP)

Derived classes can be substituted for their base classes without affecting the correctness of the program.

**Example:** Using `Flashcard` polymorphically in `CardService`

```cpp
// From cardservice.h
class CardService {
public:
    virtual QVector<Flashcard*> getFlashcards(const QString& deckName) = 0;
    // ...
};

// From AddCardDialog.cpp
void AddCardDialog::setupUI() {
    // ...
    auto cards = m_cardService->getFlashcards(m_initialDeck);
    if (m_editIndex >= 0 && m_editIndex < cards.size()) {
        Flashcard* card = cards.at(m_editIndex);
        // CardService returns Flashcard*, but we can work with any derived type
        if (card->isQuizCard()) {
            // Special handling for QuizCard
            QuizCard* quizCard = dynamic_cast<QuizCard*>(card);
            // ...
        } else {
            // General handling for any Flashcard
            // ...
        }
    }
    // ...
}
```

### 4. Interface Segregation Principle (ISP)

Interfaces are client-specific rather than general-purpose, preventing classes from depending on methods they don't use.

**Example:** The `CardService` interface provides specific operations for card management

```cpp
// CardService interface with specific card operations
class CardService {
public:
    virtual QStringList getDeckNames() = 0;
    virtual bool createDeck(const QString& name) = 0;
    virtual bool addFlipCard(const QString& deckName, const QString& front, const QString& back) = 0;
    virtual bool addQuizCard(const QString& deckName, const QString& front, const QStringList& options, int correctIndex) = 0;
    // ...
};
```

### 5. Dependency Inversion Principle (DIP)

High-level modules are not dependent on low-level modules; both depend on abstractions.

**Example:** UI components depending on the `CardService` interface rather than concrete implementations

```cpp
// MainWindow depends on CardService interface, not concrete implementation
class MainWindow : public QMainWindow {
private:
    CardService* m_cardService; // Interface reference, not concrete class
    // ...
public:
    MainWindow(CardService* cardService) : m_cardService(cardService) {
        // ...
    }
};

// From main.cpp (hypothetical implementation)
int main(int argc, char *argv[]) {
    // ...
    DeckManager* deckManager = new DeckManager();
    CardService* cardService = new CardServiceImpl(deckManager);
    MainWindow window(cardService);
    // ...
}
```

## Design Patterns

### 1. Factory Method Pattern

Used to create objects without specifying the exact class of object that will be created.

**Example:** `Flashcard::fromJson` static method

```cpp
// From flashcard.h
class Flashcard {
public:
    // ...
    static Flashcard* fromJson(const QJsonObject& obj);
    // ...
};

// Hypothetical implementation in flashcard.cpp
Flashcard* Flashcard::fromJson(const QJsonObject& obj) {
    QString type = obj["type"].toString();

    if (type == "flip") {
        return new FlipCard(obj["front"].toString(), obj["back"].toString());
    } else if (type == "quiz") {
        QStringList options;
        QJsonArray optionsArray = obj["options"].toArray();
        for (const auto& opt : optionsArray) {
            options << opt.toString();
        }
        return new QuizCard(obj["front"].toString(), options, obj["correctIndex"].toInt());
    }

    return nullptr; // Unknown card type
}
```

### 2. Strategy Pattern

Used to define a family of algorithms, encapsulate each one, and make them interchangeable.

**Example:** Different card types (FlipCard, QuizCard) providing different implementations of the same interface

```cpp
// Base strategy
class Flashcard {
public:
    virtual QString getFrontText() const = 0;
    virtual QString getBackText() const = 0;
    virtual bool isQuizCard() const = 0;
    // ...
};

// Concrete strategy 1
class FlipCard : public Flashcard {
public:
    QString getFrontText() const override { return m_front; }
    QString getBackText() const override { return m_back; }
    bool isQuizCard() const override { return false; }
    // ...
};

// Concrete strategy 2
class QuizCard : public Flashcard {
public:
    QString getFrontText() const override { return m_front; }
    QString getBackText() const override {
        // Return the correct answer text
        return m_options.at(m_correctIndex);
    }
    bool isQuizCard() const override { return true; }
    // ...
};
```

### 3. Observer Pattern

Used to define a one-to-many dependency between objects, where a change in one object results in the notification of many others.

**Example:** Signal-slot connections in Qt for UI updates

```cpp
// From addCardDialog.h (partial)
class AddCardDialog : public QDialog {
    Q_OBJECT
signals:
    void cardAdded(const QString& deckName);
    void cardEdited(const QString& deckName, int index);
    // ...
};

// From mainwindow.cpp (hypothetical implementation)
MainWindow::MainWindow(CardService* cardService) {
    // ...
    AddCardDialog* dialog = new AddCardDialog(cardService, deckName);
    connect(dialog, &AddCardDialog::cardAdded, this, &MainWindow::onCardAdded);
    connect(dialog, &AddCardDialog::cardEdited, this, &MainWindow::onCardEdited);
    // ...
}

void MainWindow::onCardAdded(const QString& deckName) {
    // Update UI when a card is added
    refreshDeckView(deckName);
}
```

### 4. Model-View-Controller (MVC) Pattern

Separates the application into three interconnected components: Model (data), View (UI), and Controller (business logic).

**Example:** Separation of concerns across the application

- **Model:** `Flashcard`, `FlipCard`, `QuizCard`, `DeckManager`
- **View:** UI classes like `MainWindow`, `DeckListPanel`, `StudyPanel`
- **Controller:** `CardService` and `CardServiceImpl` mediating between models and views

```cpp
// Model: Data representation
class FlipCard : public Flashcard {
private:
    QString m_front;
    QString m_back;
    // ...
};

// Controller: Business logic
class CardServiceImpl : public CardService {
private:
    DeckManager* m_deckManager;

public:
    bool addFlipCard(const QString& deckName, const QString& front, const QString& back) override {
        FlipCard* card = new FlipCard(front, back);
        return m_deckManager->addFlashcardToDeck(deckName, card);
    }
    // ...
};

// View: User interface
class StudyPanel : public QWidget {
private:
    CardService* m_cardService;
    QString m_deckName;
    // UI components
    // ...

public:
    void showNextCard() {
        // Get data from controller
        auto cards = m_cardService->getDueFlashcards(m_deckName);
        // Update view
        updateCardDisplay(cards.at(m_currentIndex));
    }
    // ...
};
```

### 5. Singleton Pattern (Implied)

Ensures a class has only one instance and provides a global point of access to it.

**Example:** While not explicitly shown in the provided code, the `DeckManager` might be implemented as a singleton to provide a single point of access to deck data throughout the application.

```cpp
// Hypothetical implementation of DeckManager as a singleton
class DeckManager {
private:
    static DeckManager* s_instance;
    DeckManager() { /* Private constructor */ }

public:
    static DeckManager* instance() {
        if (!s_instance) {
            s_instance = new DeckManager();
        }
        return s_instance;
    }

    // Rest of the class implementation
    // ...
};
```

## Clean Architecture Principles

The NeuroCards application follows clean architecture principles by separating concerns into distinct layers:

1. **Core Domain Layer:** `Flashcard`, `FlipCard`, `QuizCard` (business entities)
2. **Service Layer:** `CardService`, `CardServiceImpl` (use cases)
3. **UI Layer:** All UI components in the `ui` directory (presentation)
4. **Data Access Layer:** `DeckManager` (data persistence)

This separation allows for:

- Independent testing of business logic
- Flexibility to change UI without affecting core functionality
- Ability to switch persistence mechanisms without changing the application logic

## Conclusion

The NeuroCards application demonstrates a well-structured architecture that embraces numerous design principles and patterns. This approach results in code that is maintainable, extensible, and robust to changes, while providing a clean separation of concerns between different components of the system.
