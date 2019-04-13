TEMPLATE     = vcapp
TARGET       = TinderGUI
HEADERS     += TinderGUI.h fluxy.h vecteur.h
SOURCES     += TinderGUI.cpp TinderImplementaton.cpp fluxy.cpp
INCLUDEPATH += ../tinderApp
CONFIG += warn_on qt debug windows console
QT += widgets