#include "TinderGUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   //SecondWindow w2;

   //w.setPartner(&w2);
   //w2.setPartner(&w);
   return a.exec();
}