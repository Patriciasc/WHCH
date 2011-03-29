#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

QT += core gui xml xmlpatterns network

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

INCLUDEPATH += src/qtsingleapplication
SOURCES += src/main.cpp \
           src/whch.cpp \
           src/whchTableModel.cpp \
           src/whchTask.cpp \
           src/qtsingleapplication/qtsingleapplication.cpp \
           src/qtsingleapplication/qtlocalpeer.cpp \
           src/qtsingleapplication/qtlockedfile.cpp \
           src/qtsingleapplication/qtlockedfile_win.cpp \
           src/qtsingleapplication/qtlockedfile_unix.cpp

HEADERS += src/whch.h \
           src/whchTableModel.h \
           src/whchTask.h \
           src/qtsingleapplication/qtsingleapplication.h \
           src/qtsingleapplication/qtlocalpeer.h \
           src/qtsingleapplication/qtlockedfile.h

FORMS   += data/ui/whch.ui \
           data/ui/dialogTasksClients.ui

OTHER_FILES += data/ui/images/process_start.png \
               data/ui/images/process_stop.png \
               data/icons/16x16/whch.png \
               whch_log.xslt \

DISTFILES += data/ui/images/process_start.png \
             data/ui/images/process_stop.png \
             src/qtsingleapplication/qtlockedfile.cpp \
             src/qtsingleapplication/qtlockedfile_win.cpp \
             src/qtsingleapplication/qtlockedfile_unix.cpp \
             whch.desktop \
             data/icons/16x16/whch.png \
             whch_log.xslt \
             README \
             COPYING \
             AUTHORS \
             ChangeLog

RESOURCES += whch.qrc
