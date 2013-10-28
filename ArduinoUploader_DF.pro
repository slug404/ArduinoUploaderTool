#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T09:03:26
#
#-------------------------------------------------

QT       += core
QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
equals(QT_MAJOR_VERSION, 4): QMAKE_CXXFLAGS += -std=c++0x

CONFIG(release, debug|release):DEFINES += USE_RELEASE
CONFIG(debug, debug|release):DEFINES += USE_DEBUG

TARGET = ArduinoUploader
TEMPLATE = app

INCLUDEPATH += ./QExtserialport
include(./QExtserialport/qextserialport.pri)

SOURCES += main.cpp \
	UploadFactory.cpp \
	UploadBase.cpp \
	Uploader_Mac.cpp \
	Uploader_Linux.cpp \
	Uploader_Windows.cpp \
	Sleep.cpp

HEADERS += \
	UploadFactory.h \
	UploadBase.h \
	Uploader_Mac.h \
	Uploader_Linux.h \
	Uploader_Windows.h \
	Sleep.h

win32{
MOC_DIR = _moc
OBJECTS_DIR = _obj
RCC_DIR = _rcc
UI_DIR = _ui
UI_HEADERS_DIR = _ui
UI_SOURCES_DIR = _ui
}else{
MOC_DIR = .moc
OBJECTS_DIR = .obj
RCC_DIR = .rcc
UI_DIR = .ui
UI_HEADERS_DIR = .ui
UI_SOURCES_DIR = .ui
}
