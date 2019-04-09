TEMPLATE     = vcapp
TARGET       = TinderGUI
HEADERS     += TinderGUI.h vecteur.h fluxy.h calibrationWindow.h CommunicationFPGA.h traitementPhonemes.h
SOURCES     += TinderGUI.cpp main.cpp fluxy.cpp calibrationWindow.cpp traitementPhonemes.cpp 
INCLUDEPATH += ../tinderApp
LIBS += ./VSC2017/CommunicationFPGA.lib
CONFIG += warn_on qt debug windows console
QT += widgets