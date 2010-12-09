#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

TARGET = whch
TEMPLATE = app


SOURCES += src/main.cpp\
           src/whch.cpp \
           src/whchTableModel.cpp \
           src/whchTask.cpp \
           src/configDialog.cpp \
    src/whchSettings.cpp

HEADERS += src/whch.h \
           src/whchTableModel.h \
           src/whchTask.h \
           src/configDialog.h \
    src/whchSettings.h

FORMS   += ui/whch.ui \
    ui/configDialog.ui \
    ui/dialogTasksClients.ui
