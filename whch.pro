#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

include(../qt-solutions/qtsingleapplication/src/qtsingleapplication.pri)

QT       += core gui xml xmlpatterns

CONFIG += qt warn_on debug

VERSION = -0.2

TEMPLATE = app

TARGET = whch
target.path = $${PREFIX}/bin
icon.files = data/icons/16x16/whch.png
icon.path = $${PREFIX}/share/icons
desktop.files = whch.desktop
desktop.path = $${PREFIX}/share/applications

INSTALLS += target icon desktop

SOURCES += src/main.cpp \
           src/whch.cpp \
           src/whchTableModel.cpp \
           src/whchTask.cpp

HEADERS += src/whch.h \
           src/whchTableModel.h \
           src/whchTask.h

FORMS   += data/ui/whch.ui \
           data/ui/dialogTasksClients.ui

OTHER_FILES += data/ui/images/process_start.png \
               data/ui/images/process_stop.png \
               whch_log.xslt

DISTFILES += data/ui/images/process_start.png \
             data/ui/images/process_stop.png \
             whch.desktop \
             data/icons/16x16/whch.png \
             whch_log.xslt \
             README \
             COPYING \
             AUTHORS \
             ChangeLog

RESOURCES += whch.qrc
