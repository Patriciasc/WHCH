#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

CONFIG += qt warn_on debug

VERSION = 0.1

TEMPLATE = app

TARGET = whch
target.path = $${PREFIX}/bin
INSTALLS += target

SOURCES += src/main.cpp \
           src/whch.cpp \
           src/whchTableModel.cpp \
           src/whchTask.cpp

HEADERS += src/whch.h \
           src/whchTableModel.h \
           src/whchTask.h

FORMS   += ui/whch.ui \
           ui/dialogTasksClients.ui

OTHER_FILES += ui/images/process_start.png \
               ui/images/process_stop.png \
               whch_log.xslt

DISTFILES += ui/images/process_start.png \
             ui/images/process_stop.png \
             whch_log.xslt \
             README

RESOURCES += whch.qrc
