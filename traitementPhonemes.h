//Contient les constantes, les classes et les prototypes des fonctions du programme de détection de phonèmes

#ifndef APP_PROJ_H
#define APP_PROJ_H

//Externe
#include <iostream>
#include <string>
#include <chrono>
//FPGA
#include "CommunicationFPGA.h"
//Qt
#include <QApplication>

const int NUM_FILTERS = 3;
const int NUM_PHONEMES = 4;
const double MAX_VALEUR = 255;
const double DETECTION_TOLERANCE = 0.1; // (en %) Si le signal pour un filtre a un écart plus grand que ce pourcentage, il n'y aura pas de match
const int NUM_READS = 50;
const int READ_TIME = 0.05;


const string PHONEMES[NUM_PHONEMES] = {
	"AH", // 0
	"EUH", // 1
	"UH", // 2
	"OH", // 3
};


//Input est un objet qui va être créé pour chaque lecture (peut en avoir plusieurs par secondes)
class Input
{
public:
	double filterVals[NUM_FILTERS] = { 0,0,0 }; //0}; //Valeurs en pourcentage (sur 1)

	Input(double newFilterVals[NUM_FILTERS]);
	Input(int newFilterVals[NUM_FILTERS]); //Au cas ou l'entree sont des entiers... (un des constructeurs pourrait etre enlevé à l'avenir
};

//Un objet qui fait la moyenne des «Input» pour un phonème, est utilisé pour détecter les phonèmes
class PhonemeRef
{
public:
	double referenceTab[NUM_FILTERS];
	int numInputs = 0;

	PhonemeRef();
	PhonemeRef(double forcedTab[NUM_FILTERS]);

	void addInput(Input newInput); //Ajouter par un input
	void addInput(PhonemeRef newPhonemeRef); //Ajouter par un autre PhonemeRef existant (déjà une composition d'inputs) 
	void reset();

private:
	void compileInput(double newRefTab[NUM_FILTERS]);
};

//On génère un «CustomSignature» propre à chaque utilisateur lors de la calibration
//«CustomSignature» regroupe un «PhonemeRef» par phonème
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

//Retourne le numéro du phonème détecté
int identifyPhoneme(CustomSoundSignature refSignature, PhonemeRef phonemeInput);

void activeWait(double time);
void loopReadPhoneme(CommunicationFPGA &port, CustomSoundSignature &newSignature); //Lecture

#endif //APP_PROJ