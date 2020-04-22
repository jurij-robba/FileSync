QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
	file_manager.cpp \
	main.cpp \
	mainwindow.cpp \
	messagedialog.cpp

HEADERS += \
	file_manager.hpp \
	mainwindow.hpp \
	messagedialog.hpp

FORMS += \
	mainwindow.ui \
	messagedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	res.qrc
