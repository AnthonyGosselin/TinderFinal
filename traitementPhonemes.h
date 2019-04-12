//Contient les constantes, les classes et les prototypes des fonctions du programme de d�tection de phon�mes

#ifndef APP_PROJ_H
#define APP_PROJ_H

//Externe
#include <iostream>
#include <string>
//FPGA
#include "CommunicationFPGA.h"
//Qt
#include <QApplication>

const int NUM_FILTERS = 4;
const int NUM_PHONEMES = 4;
const double MAX_VALEUR = 255;
const double DETECTION_TOLERANCE = 0.05; // (en %) Si le signal pour un filtre a un �cart plus grand que ce pourcentage, il n'y aura pas de match
const int NUM_READS = 5;

const string PHONEMES[NUM_PHONEMES] = {
	"AH", // 0
	"EUH", // 1
	"Ii", // 2
	"OH", // 3
};


//Input est un objet qui va �tre cr�� pour chaque lecture (peut en avoir plusieurs par secondes)
class Input
{
public:
	double filterVals[NUM_FILTERS] = { 0,0,0,0 }; //Valeurs en pourcentage (sur 1)

	Input(double newFilterVals[NUM_FILTERS]);
	Input(int newFilterVals[NUM_FILTERS]); //Au cas ou l'entree sont des entiers... (un des constructeurs pourrait etre enlev� � l'avenir
};

//Un objet qui fait la moyenne des �Input� pour un phon�me, est utilis� pour d�tecter les phon�mes
class PhonemeRef
{
public:
	double referenceTab[NUM_FILTERS];
	int numInputs = 0;

	PhonemeRef();
	PhonemeRef(double forcedTab[NUM_FILTERS]);

	void addInput(Input newInput); //Ajouter par un input
	void addInput(PhonemeRef newPhonemeRef); //Ajouter par un autre PhonemeRef existant (d�j� une composition d'inputs) 
	void reset();

private:
	void compileInput(double newRefTab[NUM_FILTERS]);
};

//On g�n�re un �CustomSignature� propre � chaque utilisateur lors de la calibration
//�CustomSignature� regroupe un �PhonemeRef� par phon�me
class CustomSoundSignature
{
public:
	PhonemeRef phonemeRefTab[NUM_PHONEMES];

	CustomSoundSignature();
	CustomSoundSignature(PhonemeRef newPhonemeRefTab[NUM_PHONEMES]);
};

double randRange(int low, int high);
Input generateInputTest(int n);
Input getInputFromPort(CommunicationFPGA &port, bool test = false);
PhonemeRef readPhonemeFromPort(CommunicationFPGA &port, int p = 0);

//Pourrait en sort, durant la calibration, que tu ne peux pas enregistrer un phon�me s'il est trop similaire � un phon�me d�j� enregistr� (s'il y a des probl�mes de collisions)
CustomSoundSignature calibration(CommunicationFPGA &port);

//Retourne le num�ro du phon�me d�tect�
int identifyPhoneme(CustomSoundSignature refSignature, PhonemeRef phonemeInput);

#endif //APP_PROJ