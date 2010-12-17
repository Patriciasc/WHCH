#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

TARGET = whch
TEMPLATE = app

CONFIG += qt warn_on debug

SOURCES += src/main.cpp\
           src/whch.cpp \
           src/whchTableModel.cpp \
           src/whchTask.cpp \
           src/whchConfigDialog.cpp \
           src/whchSettings.cpp \
           src/whchWorkTime.cpp

HEADERS += src/whch.h \
           src/whchTableModel.h \
           src/whchTask.h \
           src/whchConfigDialog.h \
           src/whchSettings.h \
           src/whchWorkTime.h

FORMS   += ui/whch.ui \
           ui/configDialog.ui \
           ui/dialogTasksClients.ui

OTHER_FILES += ui/images/process_start.png \
               ui/imames/process_stop.png

RESOURCES += whch.qrc
