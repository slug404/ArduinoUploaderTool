#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T09:03:26
#
#-------------------------------------------------

QT       += core
QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QMAKE_CXXFLAGS += -std=gnu++0x
equals(QT_MAJOR_VERSION, 5): CONFIG += c++11

TARGET = ArduinoUploader_DF
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
