#include "traitementPhonemes.h"
#include "calibrationWindow.h"

using namespace std;

/*****************************************************************************
						IMPLÉMENTATION CLASSES
******************************************************************************/

//____________INPUT____________

//Pour random
Input::Input(double newFilterVals[NUM_FILTERS]) {
	for (int i = 0; i < NUM_FILTERS; i++) {
		double newVal = newFilterVals[i] / MAX_VALEUR; // Conversion se fait ici!
		//cout << "New val for filter #" << i << ": " << newVal << endl;
		filterVals[i] = newVal;
	}
}

//Pour normal
Input::Input(int newFilterVals[NUM_FILTERS]) {

	int biggestVal = -1;
	for (int i = 0; i < NUM_FILTERS; i++) {
		if (newFilterVals[i] > biggestVal)
			biggestVal = newFilterVals[i];
	}
	
	double mult = (biggestVal == 0)? 0 : (double)(MAX_VALEUR / biggestVal);
	//cout << "New val: \t( ";
	//cout << "biggest: " << biggestVal << " mult: " << mult <<  " = " << (double)(biggestVal * mult) << endl;
	for (int i = 0; i < NUM_FILTERS; i++) {
		double multVal = (double)(newFilterVals[i] * mult);
		double newVal = (double)(multVal/ MAX_VALEUR); // Conversion se fait ici!

		//cout  << newVal << (i == NUM_FILTERS-1? " )\n": ",     ");
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
	//cout << "Adding PhonemeRef to PhonemeRef--------------------------------------------\n";
	compileInput(newPhonemeRef.referenceTab);
}

void PhonemeRef::compileInput(double newRefTab[NUM_FILTERS]) {
	//Ajouter nouvelle valeur pour trouver une valeur moyenne pour chaque phoneme
	//Pourrait calculer si c'est une valeur rejet et ne pas le prendre en compte
	//Pourrait etre une bonne idee que chaque phoneme ait deja une configuration de base comme minimum

	int currNum = ++numInputs;

	if (currNum > 1) {
		//cout << "Nouv moyenne: ( ";
		for (int i = 0; i < NUM_FILTERS; i++) {
			double nouvMoy = (referenceTab[i] * (currNum - 1) / currNum) + (newRefTab[i] / currNum); //Calcul de la moyenne à fur et a mesure
			//cout << i << ": " << nouvMoy << " /// " << referenceTab[i] << (i == NUM_FILTERS-1? " )\n": ",     ");
			referenceTab[i] = nouvMoy;
		}
	}
	else {
		//cout << "Moyenne initiale: ( ";
		for (int i = 0; i < NUM_FILTERS; i++) {
			double nouvVal = newRefTab[i];
			//cout << i << ": " << nouvVal << (i == NUM_FILTERS-1? " )\n": ",     ");
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

//Fonction de test
Input generateInputTest(int n) {
	double randomTab[NUM_FILTERS];
	cout << "Random val: ( ";
	for (int i = 0; i < NUM_FILTERS; i++) {
		randomTab[i] = (randRange(0, (255) * (n + 1) / 4));
		cout << i << ": " << randomTab[i] << (i == NUM_FILTERS-1? " )\n" : ",     ");
	}
	Input newInput(randomTab);

	return newInput;
}

//Prend une mesure unitaire
Input getInputFromPort(CommunicationFPGA &port, bool test) {
	int filterInputTab[NUM_FILTERS];
	
	bool success0 = port.lireRegistre(LECT_CAN1, filterInputTab[0]);
	bool success1 = port.lireRegistre(LECT_CAN2, filterInputTab[1]);
	bool success2 = port.lireRegistre(LECT_CAN3, filterInputTab[2]);
	//bool success3 = port.lireRegistre(LECT_CAN3, filterInputTab[3]);

	//La conversion devrait se faire dans le constructeur de la class Input
	Input newInput(filterInputTab);

	if (test) {
		for (int i = 0; i < NUM_FILTERS; i++) {
			//cout << (float)(filterInputTab[i]/255.0) << "\t";
		}

		bool successes[NUM_FILTERS] = {success0, success1, success2};
		bool gotFail = false;
		for (int i = 0; i < NUM_FILTERS; i++) {
			if (!successes[i]) {
				cout << (!gotFail? "(Filter read fail: ": "") << i  << (i == NUM_FILTERS-1? ")\n": ", ");
				gotFail = true;
			}	
		}

		/*if (!gotFail)
			cout << endl;/**/
	}

	return newInput;
}

//Prend la moyenne de plusieurs mesures
PhonemeRef readPhonemeFromPort(CommunicationFPGA &port, int p) {

	PhonemeRef newRef;
	double waitTime = READ_TIME / NUM_READS;
	//Quelques lectures par phoneme
	//cout << "\n\n\n---------------------START--------------------------\n\n";
	for (int i = 0; i < NUM_READS; i++) {
		auto initialTime = chrono::high_resolution_clock::now(); //Commence le timer

		newRef.addInput(getInputFromPort(port, true)); //Vraies valeurs
		//newRef.addInput(generateInputTest(p)); //Valeurs générées aléatoirement pour tester

		auto endTime = chrono::high_resolution_clock::now(); //Arrête le timer
		chrono::duration<double> elapsed = endTime - initialTime;

		double newWaitTime = (waitTime - elapsed.count() > 0) ? (waitTime - elapsed.count()) : 0;

		Sleep(waitTime); //... 
	}
	//cout << "\n\n------------------END-------------------------------\n\n\n";
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

			cout << diff << ", ";
		}
		cout << " || ";
	}
	cout << endl;

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

//Permet de faire Sleep() sans geler la fenêtre active
void activeWait(double time) {
	double count = 0;
	double inc = 5; //ms

	while (count < time) {
		auto initialTime = chrono::high_resolution_clock::now(); //Commence le timer

		Sleep(inc);
		QCoreApplication::processEvents();

		auto endTime = chrono::high_resolution_clock::now(); //Arrête le timer
		chrono::duration<double> elapsed = endTime - initialTime;

		count += (elapsed.count() * 1000); // .count() returns in seconds...
	}
}

//Boucle de lecture principale du programme
void loopReadPhoneme(CommunicationFPGA &port, CustomSoundSignature &newSignature) {
	while (true) {

		int matchPhoneme = identifyPhoneme(newSignature, readPhonemeFromPort(port));

		if (matchPhoneme < 0)
			cout << "";
		else {
			cout << "Matched: " << PHONEMES[matchPhoneme] << endl;
			//Appeler la fonction appropriée

			//Pourrais tenter d'identifier le phoneme encore quelques fois ici pour valider (va falloir réduire le temps de lecture de phonème)
		}
		activeWait(10);
	}
}