QT       += core gui widgets
CONFIG   += c++17
TARGET   = NeuroCards
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/core/flashcard.cpp \
    src/core/deckmanager.cpp \
    src/ui/mainwindow.cpp \
    src/ui/flashCardEditorWidget.cpp \
    src/ui/deckViewWidget.cpp

HEADERS += \
    src/core/flashcard.h \
    src/core/deckmanager.h \
    src/ui/mainwindow.h \
    src/ui/flashCardEditorWidget.h \
    src/ui/deckViewWidget.h
