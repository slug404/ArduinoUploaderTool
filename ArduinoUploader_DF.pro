#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T09:03:26
#
#-------------------------------------------------

QT       += core
QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QMAKE_CXXFLAGS += -std=c++0x
equals(QT_MAJOR_VERSION, 5): CONFIG += c++11

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
