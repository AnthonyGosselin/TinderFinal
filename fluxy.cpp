#include "fluxy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdio.h>
#include <cstring>
#include <math.h>

using namespace std;

fluxy::fluxy()
{
	srand(time(NULL));
	listAccounts();
}

fluxy::~fluxy()
{
	save();
}

int fluxy::addAccount(string newAccount, string password, string description) {
	info a;
	for (int i = 0; i < nbUsers; i++) {
		if (newAccount == accounts[i].name) {
			return -1;
		}
	}
	a.name = toLower(newAccount);
	a.password = password;
	if (description == "") {
		a.description = "Je suis une coquille vide dépourvu d'âme.";
	}
	else a.description = description;
	a.path = "no path";
	accounts += a;
	nbUsers++;
	cu = nbUsers - 1;
	currentUser = accounts[cu];
	if (pickUser() == -1) {
		return 1;
	}
	return 0;
}

int fluxy::like() {
	int i = 0;
	while (currentPray.like[i] != "\0") {
		i++;
	}
	currentPray.like[i] = currentUser.name;
	i = 0;
	while (currentUser.iLike[i] != "\0") {
		i++;
	}
	currentUser.iLike[i] = currentPray.name;
	tempSave(currentUser, cu, currentPray, cp);
	int j = match();

	if (pickUser() == -1) {
		if (j == 0) {
			return 2;
		}
		return 1;
	}
	return j;
}
int fluxy::dislike() {
	{
		int i = 0;
		while (currentPray.dislike[i] != "\0") {
			i++;
		}
		currentPray.dislike[i] = currentUser.name;
		i = 0;
		while (currentUser.iDislike[i] != "\0") {
			i++;
		}
		currentUser.iDislike[i] = currentPray.name;
		tempSave(currentUser, cu, currentPray, cp);
		if (pickUser() == -1) {
			return 1;
		}
	}
}
int fluxy::superLike() {
	int i = 0;
	while (currentPray.superLike[i] != "\0") {
		i++;
	}
	currentPray.superLike[i] = currentUser.name;
	i = 0;
	while (currentUser.iSuperLike[i] != "\0") {
		i++;
	}
	currentUser.iSuperLike[i] = currentPray.name;
	tempSave(currentUser, cu, currentPray, cp);
	int j = match();
	if (pickUser() == -1) {
		if (j == 0) {
			return 2;
		}
		return 1;
	}
	return j;
}


// FIX LOGIN
int fluxy::login(string name, string password) {
	int i = 0;
	info a = accounts[i];
	while (a.name != toLower(name) && i < nbUsers) {
		i++;
		a = accounts[i];
	}
	if (a.password != password) {
		return -1;
	}
	currentUser = a;
	cu = i;
	if (pickUser() == -1) {
		return 1;
	}
	return 0;
}
//string fluxy::sponsor() {}

void fluxy::fillList(string stuff, string *c) {
	char cstr[1000];
	char *bob;
	strcpy(cstr, stuff.c_str());
	int i = 0;
	bob = strtok(cstr, "\t");
	while (bob != NULL) {
		c[i] = bob;
		bob = strtok(NULL, "\t");
		i++;
	}
}

int fluxy::listAccounts() {
	ifstream fichier(CoorsLight);
	info a;
	if (fichier) {
		string ligne;
		string n;
		getline(fichier, n);
		nbUsers = stringToInt(n);
		while (getline(fichier, ligne)) {
			a.name = ligne;
			getline(fichier, ligne);
			a.password = ligne;
			getline(fichier, ligne);
			fillList(ligne, a.Filtre_1);
			getline(fichier, ligne);
			fillList(ligne, a.Filtre_2);
			getline(fichier, ligne);
			fillList(ligne, a.Filtre_3);
			getline(fichier, ligne);
			fillList(ligne, a.Filtre_4);
			getline(fichier, ligne);
			fillList(ligne, a.like);
			getline(fichier, ligne);
			fillList(ligne, a.dislike);
			getline(fichier, ligne);
			fillList(ligne, a.superLike);
			getline(fichier, ligne);
			fillList(ligne, a.iLike);
			getline(fichier, ligne);
			fillList(ligne, a.iDislike);
			getline(fichier, ligne);
			fillList(ligne, a.iSuperLike);
			getline(fichier, ligne);
			a.description = ligne;
			getline(fichier, ligne);
			a.path = ligne;

			accounts += a;
			a.name = "\0";
			a.password = "\0";
			for (int i = 0; i <= 100; i++) {
				a.like[i] = "\0";
				a.dislike[i] = "\0";
				a.superLike[i] = "\0";
				a.iLike[i] = "\0";
				a.iDislike[i] = "\0";
				a.iSuperLike[i] = "\0";
			}
			for (int j = 0; j < 4; j++) {
				a.Filtre_1[j] = "\0";
				a.Filtre_2[j] = "\0";
				a.Filtre_3[j] = "\0";
				a.Filtre_4[j] = "\0";
			}
			a.description = "\0";
			a.path = "\0";


		}
		return 0;
	}
	else {
		return -1;
	}
}
// Change for loops for while loops that stop at NULL so doesn't go through the entire array every time (if you have time)
string fluxy::getName_P() {
	return currentPray.name;
}

string fluxy::getName_U() {
	return currentUser.name;
}

int fluxy::pickUser() {
	string b = "There is no one left in your queue :/";
	int i = 0;
	int reset = 0;
	if (cu == 0) i++;
	info a = accounts[i];
	while (reset == 0 && i < nbUsers) {
		reset = 1;
		for (int j = 0; j < 100; j++) {
			if (a.name == currentUser.iLike[j] && i < nbUsers || a.name == currentUser.name && i < nbUsers) {
				i++;
				a = accounts[i];
				reset = 0;
			}
		}
		for (int k = 0; k < 100; k++) {
			if (a.name == currentUser.iDislike[k] && i < nbUsers || a.name == currentUser.name && i < nbUsers) {
				i++;
				a = accounts[i];
				reset = 0;
			}
		}
		for (int l = 0; l < 100; l++) {
			if (a.name == currentUser.iSuperLike[l] && i < nbUsers || a.name == currentUser.name && i < nbUsers) {
				i++;
				a = accounts[i];
				reset = 0;
			}
		}
	}
	if (i < nbUsers) {
		currentPray = a;
		cp = i;
		cout << "Your pray is " << currentPray.name << endl;
		return 0;
	}
	else return -1;
}

int fluxy::getNbUsers() {
	return nbUsers;
}

int fluxy::stringToInt(string numberToConvert) {
	stringstream geek(numberToConvert);
	int x = 0;
	geek >> x;
	return x;
}

void fluxy::save() {
	ofstream fichier(CoorsLight);
	info a;
	if (fichier) {
		fichier << nbUsers << endl;
		for (int i = 0; i < nbUsers; i++) {
			a = accounts[i];
			fichier << a.name << endl;
			fichier << a.password << endl;
			int j = 0;
			while (j < 4) {
				fichier << a.Filtre_1[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (j < 4) {
				fichier << a.Filtre_2[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (j < 4) {
				fichier << a.Filtre_3[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (j < 4) {
				fichier << a.Filtre_4[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.like[j] != "\0") {
				fichier << a.like[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.dislike[j] != "\0") {
				fichier << a.dislike[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.superLike[j] != "\0") {
				fichier << a.superLike[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.iLike[j] != "\0") {
				fichier << a.iLike[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.iDislike[j] != "\0") {
				fichier << a.iDislike[j] << "\t";
				j++;
			}
			fichier << endl;
			j = 0;
			while (a.iSuperLike[j] != "\0") {
				fichier << a.iSuperLike[j] << "\t";
				j++;
			}
			fichier << endl;
			fichier << a.description << endl;
			fichier << a.path << endl;
		}
	}

}

string fluxy::toLower(string name) {
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	return name;
}

void fluxy::listUsers() {
	info a;
	for (int i = 0; i < nbUsers; i++) {
		a = accounts[i];
		cout << i + 1 << "- " << a.name << endl;
	}
}

void fluxy::checkStatus() {
	int j = 0;
	while (currentUser.like[j] != "\0") {
		cout << currentUser.like[j] << "\t";
		j++;
	}
	cout << endl;
	j = 0;
	while (currentUser.dislike[j] != "\0") {
		cout << currentUser.dislike[j] << "\t";
		j++;
	}
	cout << endl;
	j = 0;
	while (currentUser.superLike[j] != "\0") {
		cout << currentUser.superLike[j] << "\t";
		j++;
	}
	cout << endl;
	j = 0;
	while (currentUser.iLike[j] != "\0") {
		cout << currentUser.iLike[j] << "\t";
		j++;
	}
	cout << endl;
	j = 0;
	while (currentUser.iDislike[j] != "\0") {
		cout << currentUser.iDislike[j] << "\t";
		j++;
	}
	cout << endl;
	j = 0;
	while (accounts[cu].iSuperLike[j] != "\0") {
		cout << accounts[cu].iSuperLike[j] << "\t";
		j++;
	}
	cout << endl;
}

void fluxy::tempSave(info a, int aa, info b, int bb) {
	accounts2.viderVec();
	for (int i = 0; i < nbUsers; i++) {
		if (i == aa) {
			accounts2 += a;
		}
		else if (i == bb) {
			accounts2 += b;
		}
		else {
			accounts2 += accounts[i];
		}
	}
	accounts.viderVec();
	for (int j = 0; j < nbUsers; j++) {
		accounts += accounts2[j];
	}
}

void fluxy::tempSave(info a, int aa) {
	accounts2.viderVec();
	for (int i = 0; i < nbUsers; i++) {
		if (i == aa) {
			accounts2 += a;
		}

		else {
			accounts2 += accounts[i];
		}
	}
	accounts.viderVec();
	for (int j = 0; j < nbUsers; j++) {
		accounts += accounts2[j];
	}
}

int fluxy::changePassword(string password, string password_check) {
	if (password != password_check) {
		return -1;
	}
	else {
		currentUser.password = password;
	}
	tempSave(currentUser, cu);
	return 0;
}

void fluxy::changeDescription(string description) {
	currentUser.description = description;
	tempSave(currentUser, cu);

}

string fluxy::getPath_U() {
	return currentUser.path;
}

string fluxy::getPath_P() {
	return currentPray.path;
}

string fluxy::checkDescription_U() {
	return currentUser.description;
}

string fluxy::checkDescription_P() {
	return currentPray.description;
}

/*void fluxy::rate() {
	string p_rated;
	float p_rating;
	int p_totalL = 0;
	int p_total = 0;
	int i = 0;
	while (currentPray.like[i] != "\0") {
		p_total++;
	}
	i = 0;
	while (currentPray.like[i] != "\0") {
		p_totalL++;
		p_total++;
	}
	while (currentPray.superLike[i] != "\0") {
		p_totalL++;
		p_total++;
	}
	if (p_total == 0) {
		currentPray.rate = "none";
	}
	else {
		p_rating = p_totalL / p_total;
		p_rating = p_rating * 100;
		p_rating = round(p_rating);
		p_rated = to_string(p_rating);
		currentPray.rate = p_rated;
	}
	string u_rated;
	float u_rating;
	int u_totalL = 0;
	int u_total = 0;
	i = 0;
	while (currentUser.like[i] != "\0") {
		u_total++;
	}
	i = 0;
	while (currentUser.like[i] != "\0") {
		u_totalL++;
		u_total++;
	}
	while (currentUser.superLike[i] != "\0") {
		u_totalL++;
		u_total++;
	}
	if (u_total == 0) {
		currentUser.rate = "none";
	}
	else {
		u_rating = u_totalL / u_total;
		u_rating = u_rating * 100;
		u_rating = round(u_rating);
		u_rated = to_string(u_rating);
		currentUser.rate = u_rated;
	}
}*/

void fluxy::logout() {
	save();
}

void fluxy::delete_account() {
	accounts -= cu;
	nbUsers--;
}

int fluxy::match() {
	for (int i = 0; i < currentUser.like->size(); i++) {
		if (currentPray.name == currentUser.like[i]) {
			return 0;
		}
	}
	for (int i = 0; i < currentUser.superLike->size(); i++) {
		if (currentPray.name == currentUser.superLike[i]) {
			return 0;
		}
	}
	return -1;
}

void fluxy::setPath(string newPath) {
	currentUser.path = newPath;
	tempSave(currentUser, cu);
}

void fluxy::setFiltre(float *filtre_1, float *filtre_2, float *filtre_3, float *filtre_4) {
	for (int i = 0; i < 4; i++) {
		currentUser.Filtre_1[i] = to_string(filtre_1[i]);
		currentUser.Filtre_2[i] = to_string(filtre_2[i]);
		currentUser.Filtre_3[i] = to_string(filtre_3[i]);
		currentUser.Filtre_4[i] = to_string(filtre_4[i]);
	}
	tempSave(currentUser, cu);
}

void fluxy::getFiltre(float *filtre_1, float *filtre_2, float *filtre_3, float *filtre_4) {
	string::size_type sz;
	for (int i = 0; i < 4; i++) {
		filtre_1[i] = stof(currentUser.Filtre_1[i], &sz);
		filtre_2[i] = stof(currentUser.Filtre_2[i], &sz);
		filtre_3[i] = stof(currentUser.Filtre_3[i], &sz);
		filtre_4[i] = stof(currentUser.Filtre_4[i], &sz);
	}
}

string fluxy::getPass() {
	return currentUser.password;
}