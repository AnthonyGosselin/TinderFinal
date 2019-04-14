#include <calibrationWindow.h>
#include <iostream>
#include <string>

using namespace std;

calibWindow::calibWindow(CommunicationFPGA &port, fluxy &core)
{
	setWindowModality(Qt::ApplicationModal);
	ptr_port = &port;
	ptr_core = &core;

	createObjects();
	createLayout();
	connectFPGA();
}

calibWindow::~calibWindow()
{
	/*delete mainLabel;
	delete output;
	delete mainButton;
	delete layout;
	//delete port;*/
}

void calibWindow::connectFPGA() {
	//FPGA setup
	if (!ptr_port->estOk()) {
		cout << "Erreur: " << ptr_port->messageErreur() << endl;
		writeToOutput("Echec de la connection a la carte FPGA\n");
		if (!TESTING)
			mainButton->setEnabled(false);
	}
	else {
		isConnection = true;
		writeToOutput("Connection a la carte FPGA reussie\n\n");
	}
}
 
void calibWindow::createObjects()
{
	mainLabel = new QLabel("Calibration");
	mainLabel->setFont(QFont("Arial", 20));

	output = new QTextEdit;
	output->setReadOnly(true);
	output->setMinimumWidth(400);

	mainButton = new QPushButton("Commencer");
	connect(mainButton, &QPushButton::clicked, this, &calibWindow::mainButtonClicked);

	cancelButton = new QPushButton("Quitter");
	connect(cancelButton, &QPushButton::clicked, this, &calibWindow::cancelButtonClicked);
}

void calibWindow::createLayout()
{
	layout = new QGridLayout;

	layout->addWidget(mainLabel, 0, 1, 1, 2, Qt::AlignCenter);
	layout->addWidget(output, 1, 0, 1, 4);
	layout->addWidget(mainButton, 2, 1);
	layout->addWidget(cancelButton, 2, 2);

	layout->setRowStretch(1, 10);
	layout->setColumnStretch(0, 10);
	layout->setColumnStretch(3, 10);

	setLayout(layout);

}

void calibWindow::createMenu()
{

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

	output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum()); // Replacer la fenêtre pour qu'elle toujours le dernier élément écrit

	delete[]newCharStr;
}

void calibWindow::cancelButtonClicked()
{
	bool closed = false;
	canceledLastPhoneme = true;
	if (currRep > 0)
		currRep--;
	else if (currRep == 0 && currPhoneme > 0) {
		currPhoneme--;
		currRep = NUM_REP - 1;
	}
	else if (currRep == 0 && currPhoneme == 0) {
		closed = true;
		close(); // Même pas commencé, donc on ferme
	}

	calibrationDone = false; //Au cas où c'était la dernière mesure
	output->clear();

	if (!closed)
		mainButtonClicked(); // Exécute le reste du code (reprise)
}

//Fonction de calibration
void calibWindow::mainButtonClicked()
{
	mainButton->setEnabled(false);
	cancelButton->setEnabled(false);

	cout << "Main button fired\n";

	if (!calibrationDone) {

		if (currPhoneme == 0 && currRep == 0 && !canceledLastPhoneme) {
			mainButton->setText("Suivant");
			cancelButton->setText("Reprendre");

			recordingPhoneme = new PhonemeRef; //La synthèse de tous les «lastRecordedPhoenme» enregistrés pour le phoneme actuel
			lastRecordedPhoneme = new PhonemeRef;
		}

		if (TESTING)
			isConnection = true;

		if (isConnection) {
			if (currRep == 0) {
				if (currPhoneme != 0)
					output->clear();
				writeToOutput("\n--- CALIBRATION: " + PHONEMES[currPhoneme] + " ---\n");
				writeToOutput("Calibration: " + PHONEMES[currPhoneme], true);
			}
			
			cout << "LOOP STATUS: (p: " << currPhoneme << ", r: " << currRep << ", c: " << canceledLastPhoneme << ")\n";
			if ( !(currPhoneme == 0 && currRep == 0) ) {
				if (!canceledLastPhoneme) {
					recordingPhoneme->addInput(*lastRecordedPhoneme); //On sauvegarde le dernier phoneme enregistré, car l'utilisateur a appuyé sur suivant

					//Vérifier si c'était la dernière lecture pour ce phonème
					if (currRep == 0) {
						phonemeRefTab[currPhoneme-1] = *recordingPhoneme; //On sauvegarder les données dans le tableau de la signature
						recordingPhoneme->reset();
					}
				}

				lastRecordedPhoneme->reset();
			}
			canceledLastPhoneme = false;

			//Decompte...
			activeWait(500);
			writeToOutput("   Dites: " + PHONEMES[currPhoneme] + " dans...\n");
			activeWait(250);
			writeToOutput("   3\n");
			activeWait(100);
			writeToOutput("   2\n");
			activeWait(100);
			writeToOutput("   1\n");
			activeWait(100);
			writeToOutput("   GO!\n");
			//...

			int numProgBars = 10;
			int currNumBars = 0;
			double currProg = 0.0;
			double waitTime = CALIB_READ_TIME / NUM_READS;
			double newWaitTime = 0;
			writeToOutput("   Lecture en cours: ");

			//Quelques lectures par phoneme
			for (int i = 0; i < NUM_READS; i++) {
				auto initialTime = chrono::high_resolution_clock::now(); //Commence le timer

				lastRecordedPhoneme->addInput(getInputFromPort(*ptr_port, true)); //Prend une lecture
				//lastRecordedPhoneme->addInput(generateInputTest(currPhoneme)); //Valeurs générées aléatoirement pour tester
				
				auto endTime = chrono::high_resolution_clock::now(); //Arrête le timer
				chrono::duration<double> elapsed = endTime - initialTime;

				//cout << "Elapsed: " << elapsed.count() << endl;
				newWaitTime = (waitTime - elapsed.count() > 0) ? (waitTime - elapsed.count()) : 0;
				Sleep(newWaitTime); //... 

				//Bar de progrès
				QCoreApplication::processEvents();
				currProg = ((i + 1) / (double)NUM_READS);
				int newNumBars = (int)(numProgBars * currProg);  //Va arrondir vers le bas, car le type est int
				
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

				currRep = 0;
				currPhoneme++;
				
				if (currPhoneme >= NUM_PHONEMES) {
					calibrationDone = true;
					mainButton->setText("Terminer");
				}
			}

		}

		mainButton->setEnabled(true);
		cancelButton->setEnabled(true);
	}
	else {

		writeToOutput("--- Calibration terminee ---\n");
		writeToOutput("Calibration terminee", true);

		recordingPhoneme->addInput(*lastRecordedPhoneme); //On sauvegarde le dernier phoneme enregistré après que l'utilisateur ait appuyé sur Terminer
		phonemeRefTab[currPhoneme - 1] = *recordingPhoneme; //On sauvegarder les données dans le tableau de la signature

		double newTabPhoneme[NUM_PHONEMES][NUM_FILTERS];
		for (int p = 0; p < NUM_PHONEMES; p++) {
			cout << "Phoneme #" << p << " ( ";
			for (int f = 0; f < NUM_FILTERS; f++) {
				newTabPhoneme[p][f] = phonemeRefTab[p].referenceTab[f];
				cout << phonemeRefTab[p].referenceTab[f] << (f < NUM_FILTERS - 1 ? ", " : ")\n");
			}
		}

		ptr_core->setFiltre(newTabPhoneme[0], newTabPhoneme[1], newTabPhoneme[2], newTabPhoneme[3]);

		this->close();
	}
}
		