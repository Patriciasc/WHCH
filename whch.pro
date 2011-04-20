#-------------------------------------------------
#
# Project created by QtCreator 2010-10-20T13:15:31
#
#-------------------------------------------------

QT += core gui xml xmlpatterns network

CONFIG += qt warn_on debug

VERSION = -0.3

TEMPLATE = app

TARGET = whch
target.path = $${PREFIX}/bin
icon16.files = data/icons/16x16/whch.png
icon16.path = $${PREFIX}/share/icons/hicolor/16x16/apps
icon24.files = data/icons/24x24/whch.png
icon24.path = $${PREFIX}/share/icons/hicolor/24x24/apps
icon48.files = data/icons/48x48/whch.png
icon48.path = $${PREFIX}/share/icons/hicolor/46x46/apps
icon64.files = data/icons/64x64/whch.png
icon64.path = $${PREFIX}/share/icons/hicolor/64x64/apps
icon.files = data/icons/scalable/whch.svg
icon.path = $${PREFIX}/share/icons/hicolor/scalable/apps
desktop.files = whch.desktop
desktop.path = $${PREFIX}/share/applications

INSTALLS += target icon icon16 icon24 icon48 icon64 desktop

INCLUDEPATH += src/qtsingleapplication
SOURCES += src/main.cpp \
           src/whch.cpp \
           src/whchTask.cpp \
           src/qtsingleapplication/qtsingleapplication.cpp \
           src/qtsingleapplication/qtlocalpeer.cpp \
           src/qtsingleapplication/qtlockedfile.cpp \
           src/qtsingleapplication/qtlockedfile_win.cpp \
           src/qtsingleapplication/qtlockedfile_unix.cpp \
           src/whchDomNode.cpp \
           src/whchDomModel.cpp \
           src/whchTreeProxyModel.cpp \
           src/whchTableProxyModel.cpp

HEADERS += src/whch.h \
           src/whchTask.h \
           src/qtsingleapplication/qtsingleapplication.h \
           src/qtsingleapplication/qtlocalpeer.h \
           src/qtsingleapplication/qtlockedfile.h \
           src/whchDomNode.h \
           src/whchDomModel.h \
           src/whchTreeProxyModel.h \
           src/whchTableProxyModel.h

FORMS   += data/ui/whch.ui \
           data/ui/dialogTasksClients.ui

OTHER_FILES += data/ui/images/process_start.png \
               data/ui/images/process_stop.png \
               data/icons/16x16/whch.png \
	       data/icons/24x24/whch.png \
	       data/icons/48x48/whch.png \
	       data/icons/64x64/whch.png \
	       data/icons/scalable/whch.svg \
               whch_log.xslt

DISTFILES += data/ui/images/process_start.png \
             data/ui/images/process_stop.png \
             data/icons/16x16/whch.png \
	     data/icons/24x24/whch.png \
	     data/icons/48x48/whch.png \
	     data/icons/64x64/whch.png \
	     data/icons/scalable/whch.svg \
             src/qtsingleapplication/qtlockedfile.cpp \
             src/qtsingleapplication/qtlockedfile_win.cpp \
             src/qtsingleapplication/qtlockedfile_unix.cpp \
             whch.desktop \
             whch_log.xslt \
             README \
             COPYING \
             AUTHORS \
             ChangeLog

RESOURCES += whch.qrc
