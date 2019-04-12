#include "TinderGUI.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

#include <iostream>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QApplication::setStyle("fusion");

   MainWindow w;
   w.show();

   return a.exec();
}
