#include "TinderGUI.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QApplication::setStyle("fusion");

   CommunicationFPGA port;
   std::cout << "Created port\n";
   MainWindow w(port);
   w.show();
  
   return a.exec();
}