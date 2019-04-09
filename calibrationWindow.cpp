#include <calibrationWindow.h>
#include <iostream>
#include <string>

using namespace std;

calibWindow::calibWindow()
{
	cout << "Window created\n";

	createObjects();
	createLayout();
}

calibWindow::~calibWindow()
{
	delete mainLabel;
	delete output;
	delete mainButton;
	delete layout;
	delete port;
}

void calibWindow::createObjects()
{
	mainLabel = new QLabel("Calibration");
	mainLabel->setFont(QFont("Arial", 20));

	output = new QTextEdit;
	output->setReadOnly(true);
	output->setMinimumWidth(400);

	mainButton = new QPushButton("Continuer");
	connect(mainButton, &QPushButton::clicked, this, &calibWindow::mainButtonClicked);
}

void calibWindow::createLayout()
{
	layout = new QGridLayout;

	layout->addWidget(mainLabel, 0, 0, Qt::AlignCenter);
	layout->addWidget(output, 1, 0);
	layout->addWidget(mainButton, 2, 0, Qt::AlignCenter);

	layout->setRowStretch(1, 10);

	setLayout(layout);

}

void calibWindow::writeToOutput(string msg, bool writeToTitle)
{
	cout << msg << endl;
	char* newCharStr = new char[msg.length() + 1];
	strcpy(newCharStr, msg.c_str());
	if (!writeToTitle)
		output->insertPlainText(newCharStr);
	else
		mainLabel->setText(newCharStr);

	delete[]newCharStr;
}

void activeWait(double time) {
	double count = 0;
	double inc = 10; //ms

	while (count < time) {
		Sleep(inc);
		QCoreApplication::processEvents();
		count += inc;
	}
}


//Fonction de calibration
void calibWindow::mainButtonClicked()
{
	mainButton->setEnabled(false);

	cout << "Main button clicked\n";

	if (!calibrationDone) {

		if (currPhoneme == 0) {
			//FPGA setup
			port = new CommunicationFPGA;   // Instance du port de communication
			if (!port->estOk()) {
				cout << "Erreur: " << port->messageErreur() << endl;
				writeToOutput("Echec de la connection a la carte FPGA\n");
			}
			else {
				isConnection = true;
				writeToOutput("Connection a la carte FPGA reussie\n\n");
			}
		}
		isConnection = true; ///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ enlever
		if (isConnection) {
			if (currRep == 0) {
				recordingPhoneme = new PhonemeRef;
				if (currPhoneme != 0)
					output->clear();
				writeToOutput("\n--- CALIBRATION: " + PHONEMES[currPhoneme] + " ---\n");
				writeToOutput("Calibration: " + PHONEMES[currPhoneme], true);
			}

			//Decompte...
			activeWait(50);
			writeToOutput("   Dites: " + PHONEMES[currPhoneme] + " dans...\n");
			activeWait(25);
			writeToOutput("   3\n");
			activeWait(100);
			writeToOutput("   2\n");
			activeWait(100);
			writeToOutput("   1\n");
			activeWait(100);
			writeToOutput("   GO!\n");
			//...

			//recordingPhoneme->addInput(readPhonemeFromPort(*port, currPhoneme)); // ****ANCIEN****

			int numProgBars = 10;
			int currNumBars = 0;
			double currProg = 0.0;
			writeToOutput("   Lecture en cours: ");
			//Quelques lectures par phoneme
			for (int i = 0; i < NUM_READS; i++) {
				//recordingPhoneme->addInput(getInputFromPort(*port, true)); //Prend une lecture
				recordingPhoneme->addInput(generateInputTest(currPhoneme)); //Valeurs générées aléatoirement pour tester
				Sleep(200); //... 

				//Bar de progrès
				QCoreApplication::processEvents();
				currProg = ((i + 1) / (double)NUM_READS);
				int newNumBars = (int)(numProgBars * currProg);  //Va arrondir vers le bas, car le type est int
				//cout << i << " : " << numProgBars << " * " << currProg << " = " << newNumBars << endl;
				
				while (newNumBars > currNumBars) {
					writeToOutput("=");
					currNumBars++;
				}
				
			}

			writeToOutput(" |  Lecture terminee\n");

			//Incrementations...
			if (currRep < NUM_REP - 1) 
				currRep++;
			else {
				writeToOutput("--- FIN: " + PHONEMES[currPhoneme] + "\n");
				
				phonemeRefTab[currPhoneme] = *recordingPhoneme;

				currRep = 0;
				currPhoneme++;
				
				if (currPhoneme >= NUM_PHONEMES)
					calibrationDone = true;
			}

		}

		mainButton->setEnabled(true);
	}
	else {

		writeToOutput("--- Calibration terminee ---\n");
		writeToOutput("Calibration terminee", true);

		//On regroupe le tout dans une signature
		CustomSoundSignature newSignature;
		for (int i = 0; i < NUM_PHONEMES; i++) {
			newSignature.phonemeRefTab[i] = phonemeRefTab[i];
			cout << "Phoneme #" << i << " ( ";
			for (int y = 0; y < NUM_FILTERS; y++) {
				cout << phonemeRefTab[i].referenceTab[y] << (y < NUM_FILTERS - 1? ", ": ")\n");
			}
		}

		delete recordingPhoneme;
		close();
	}
}
		