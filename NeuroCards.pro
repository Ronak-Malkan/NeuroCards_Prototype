
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
    src/ui/deckViewWidget.cpp \
    src/ui/studyPanel.cpp \
    src/ui/cardPreviewDialog.cpp \
    src/ui/cardDisplayWidget.cpp \
    src/ui/deckListPanel.cpp \
    src/ui/deckDetailPanel.cpp \
    src/ui/addCardDialog.cpp 

HEADERS += \
    src/core/flashcard.h \
    src/core/deckmanager.h \
    src/ui/mainwindow.h \
    src/ui/flashCardEditorWidget.h \
    src/ui/deckViewWidget.h \
    src/ui/studyPanel.h \
    src/ui/cardPreviewDialog.h \
    src/ui/cardDisplayWidget.h \
    src/ui/deckListPanel.h \
    src/ui/deckDetailPanel.h \
    src/ui/addCardDialog.h
