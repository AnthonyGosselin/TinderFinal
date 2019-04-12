#include "traitementPhonemes.h"
#include "calibrationWindow.h"

using namespace std;

/*****************************************************************************
						IMPLÉMENTATION CLASSES
******************************************************************************/

//____________INPUT____________

Input::Input(double newFilterVals[NUM_FILTERS]) {
	for (int i = 0; i < NUM_FILTERS; i++) {
		double newVal = newFilterVals[i] / MAX_VALEUR; // Conversion se fait ici!
		//cout << "New val for filter #" << i << ": " << newVal << endl;
		filterVals[i] = newVal;
	}
}

//Au cas ou l'entree sont des entiers... (un des constructeurs pourrait etre enlevé à l'avenir
Input::Input(int newFilterVals[NUM_FILTERS]) {
	for (int i = 0; i < NUM_FILTERS; i++) {
		double newVal = (double)newFilterVals[i] / MAX_VALEUR; // Conversion se fait ici!
		//cout << "New val for filter #" << i << ": " << newVal << endl;
		filterVals[i] = newVal;
	}
}

//____________PHONEME REF____________

PhonemeRef::PhonemeRef()
{}

PhonemeRef::PhonemeRef(double forcedTab[NUM_FILTERS]) {
	for (int i = 0; i < NUM_FILTERS; i++) {
		referenceTab[i] = forcedTab[i];
	}
}

//Ajouter par un input
void PhonemeRef::addInput(Input newInput) {
	compileInput(newInput.filterVals);
}

//Ajouter par un autre PhonemeRef existant (déjà une composition d'inputs)
void PhonemeRef::addInput(PhonemeRef newPhonemeRef) {
	cout << "Adding PhonemeRef to PhonemeRef--------------------------------------------\n";
	compileInput(newPhonemeRef.referenceTab);
}

void PhonemeRef::compileInput(double newRefTab[NUM_FILTERS]) {
	//Ajouter nouvelle valeur pour trouver une valeur moyenne pour chaque phoneme
	//Pourrait calculer si c'est une valeur rejet et ne pas le prendre en compte
	//Pourrait etre une bonne idee que chaque phoneme ait deja une configuration de base comme minimum

	int currNum = ++numInputs;

	if (currNum > 1) {
		for (int i = 0; i < NUM_FILTERS; i++) {
			double nouvMoy = (referenceTab[i] * (currNum - 1) / currNum) + (newRefTab[i] / currNum); //Calcul de la moyenne à fur et a mesure
			cout << "Nouvelle moyenne: " << nouvMoy << " (filtre: " << i << ") \t";
			cout << "sommeAvant: " << referenceTab[i] << " * " << ((currNum - 1) / currNum) << " + nouv somme: " << (newRefTab[i] / currNum) << endl;
			referenceTab[i] = nouvMoy;
		}
	}
	else {
		for (int i = 0; i < NUM_FILTERS; i++) {
			double nouvVal = newRefTab[i];
			cout << "Moyenne initiale: " << nouvVal << " (filtre: " << i << ")\n";
			referenceTab[i] = nouvVal;
		}

	}

}

void PhonemeRef::reset() {
	for (int i = 0; i < NUM_FILTERS; i++) {
		referenceTab[i] = 0;
	}
	numInputs = 0;
}

//____________CUSTOM SOUND SIGNATURE_______________

CustomSoundSignature::CustomSoundSignature()
{}

CustomSoundSignature::CustomSoundSignature(PhonemeRef newPhonemeRefTab[NUM_PHONEMES]) {
	for (int i = 0; i < NUM_PHONEMES; i++) {
		phonemeRefTab[i] = newPhonemeRefTab[i];
	}
}

/*****************************************************************************
						IMPLÉMENTATION FONCTIONS
******************************************************************************/

double randRange(int low, int high) {
	return low + (rand() % (high - low + 1));
}

Input generateInputTest(int n) {
	double randomTab[NUM_FILTERS];
	for (int i = 0; i < NUM_FILTERS; i++) {
		randomTab[i] = (randRange(0, (255) * (n + 1) / 4));
		cout << "Random val: " << randomTab[i] << "  (n: " << n << ")\n";
	}
	Input newInput(randomTab);

	return newInput;
}

Input getInputFromPort(CommunicationFPGA &port, bool test) {
	int filterInputTab[4];
	
	bool success0 = port.lireRegistre(LECT_CAN0, filterInputTab[0]);
	bool success1 = port.lireRegistre(LECT_CAN1, filterInputTab[1]);
	bool success2 = port.lireRegistre(LECT_CAN2, filterInputTab[2]);
	bool success3 = port.lireRegistre(LECT_CAN3, filterInputTab[3]);

	//La conversion devrait se faire dans le constructeur de la class Input
	Input newInput(filterInputTab);

	if (test) {
		for (int i = 0; i < NUM_FILTERS; i++) {
			cout << filterInputTab[i] << "\t";
		}

		bool successes[NUM_FILTERS] = {success0, success1, success2, success3};
		bool gotFail = false;
		for (int i = 0; i < NUM_FILTERS; i++) {
			if (!successes[i]) {
				cout << (!gotFail? "(Filter read fail: ": "") << i  << (i == NUM_FILTERS-1? ")\n": ", ");
				gotFail = true;
			}	
		}

		if (!gotFail)
			cout << endl;
	}

	return newInput;
}

PhonemeRef readPhonemeFromPort(CommunicationFPGA &port, int p) {

	PhonemeRef newRef;
	//Quelques lectures par phoneme
	for (int i = 0; i < NUM_READS; i++) {
		newRef.addInput(getInputFromPort(port, true)); //Vraies valeurs
		//newRef.addInput(generateInputTest(p)); //Valeurs générées aléatoirement pour tester
		Sleep(10); //... 
	}

	return newRef;
}


//Retourne le numéro du phonème détecté
int identifyPhoneme(CustomSoundSignature refSignature, PhonemeRef phonemeInput) {

	double diffTab[NUM_PHONEMES][NUM_FILTERS] = { DETECTION_TOLERANCE * 2 };

	//Sets difference between input signal and signature for each filter of each phoneme
	for (int p = 0; p < NUM_PHONEMES; p++) {
		for (int f = 0; f < NUM_FILTERS; f++) {
			double diff = abs(refSignature.phonemeRefTab[p].referenceTab[f] - phonemeInput.referenceTab[f]);
			diffTab[p][f] = (diff);
		}
	}

	//C'est un match si tous les signaux des filtres sont en dessous de la tolérance d'écart pour un phonème
	bool gotMatch[NUM_PHONEMES] = { false };
	int matchedPhoneme = -1;
	bool multipleMatches = false;
	for (int p = 0; p < NUM_PHONEMES; p++) {
		bool phonemeMatch = true;
		for (int f = 0; f < NUM_FILTERS; f++) {
			if (diffTab[p][f] > DETECTION_TOLERANCE) {
				phonemeMatch = false;
				break; // Arrete de checker ce phoneme
			}
		}
		gotMatch[p] = phonemeMatch;
		if (phonemeMatch) {
			cout << "Got match: " << p << endl;
			if (matchedPhoneme == -1)
				matchedPhoneme = p;
			else
				multipleMatches = true;
		}
	}

	return multipleMatches ? -1 : matchedPhoneme; // Si l'on détecte plus d'un phonème, on retourne qu'il n'y a pas de match
}
