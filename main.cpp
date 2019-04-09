#include "TinderGUI.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

#include <iostream>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   //...
   std::cout << "Testing\n"; //test2
   return a.exec();
}
