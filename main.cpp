#include "TinderGUI.h"
#include <QApplication>
#include <qstylefactory.h>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QApplication::setStyle(QStyleFactory::create("Fusion"));

   MainWindow w;
   w.show();
   //SecondWindow w2;

   //w.setPartner(&w2);
   //w2.setPartner(&w);
   return a.exec();
}