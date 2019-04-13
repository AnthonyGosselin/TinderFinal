#include "TinderGUI.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("fusion");



	CommunicationFPGA port;

	/*while (true)
	{
		Sleep(50);
		getInputFromPort(port, true);

	}/**/
	

	//Manuellement 3 phonemes (jé)
	/*double refA[4] = {0.43, 0.42, 1.0};
	double refE[4] = {0.04, 0.97, 1.0};
	double refI[4] = {0.0,  1.0,  0.125};
	double refO[4] = {0.7,  1,   0.3};/**/

	//Manuellement Vince
	/*double refA[4] = {0.634, 0.356, 1.0};
	double refE[4] = {0.125, 0.772, 0.969};
	double refI[4] = {0.028, 1.0, 0.267};
	double refO[4] = {1.0, 0.315, 0.140};/**/


	//Manuellement Tony
	double refA[4] = {0.699, 0.302, 1.000};
	double refE[4] = {0.006, 0.925, 0.848};
	double refI[4] = {0.000, 1.000, 0.003};
	double refO[4] = {0.012, 0.1, 0.072};/**/
	
	PhonemeRef newRefA(refA);
	PhonemeRef newRefE(refE);
	PhonemeRef newRefI(refI);
	PhonemeRef newRefO(refO);

	PhonemeRef phonemeTab[4] = { newRefA, newRefE, newRefI, newRefO };
	CustomSoundSignature signature(phonemeTab);

	//loopReadPhoneme(port, signature);/**/

	MainWindow w(port);
	w.show();
  
	return a.exec();
}