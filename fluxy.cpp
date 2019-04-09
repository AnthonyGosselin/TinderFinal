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


void fluxy::addAccount(string newAccount, string password, string description) {
	donnee.name = toLower(newAccount);
	donnee.password = password;
	donnee.description = description;
	accounts += donnee;
	nbUsers++;
}

void fluxy::like() {
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
	//rate();
	//string b = pickUser();
	//cout << "what do you think of" << b << endl;
	cout << currentUser.name << " you have liked " << currentPray.name << endl;
}
void fluxy::dislike() {
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
		//rate();
		//string b = pickUser();
		//cout << "what do you think of" << b << endl;
		cout << currentUser.name << " you have disliked " << currentPray.name << endl;
	}
}
void fluxy::superLike() {
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
	//rate();
	//string b = pickUser();
	//cout << "what do you think of" << b << endl;
	cout << currentUser.name << " you have superliked " << currentPray.name << endl;
}


// FIX LOGIN
int fluxy::login(string name, string password) {
	int i = 0;
	info a = accounts[i];
	while (a.name != toLower(name) && i<nbUsers) { 
		//cout << a.name << "?=" << toLower(name) << endl;
		i++;
		a = accounts[i];
	}
	//cout << a.name << "=" << toLower(name) << endl;
	if (a.password != password) {
		system("CLS");
		cout << "That is the wrong combination of username and password" << endl;
		return -1;
	}
	currentUser = a;
	cu = i;
	cout << "You are logged in as " << currentUser.name << endl;
	//string judge = pickUser();
	//cout << "what do you think of" << judge << endl;
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

void fluxy::listAccounts() {
	ifstream fichier(CoorsLight);
	if (fichier) {
		string ligne;
		string a;
		getline(fichier, a);
		nbUsers = stringToInt(a);
		while (getline(fichier, ligne)) {
			donnee.name = ligne;
			getline(fichier, ligne);
			donnee.password = ligne;
			getline(fichier, ligne);
			fillList(ligne, donnee.like);
			getline(fichier, ligne);
			fillList(ligne, donnee.dislike);
			getline(fichier, ligne);
			fillList(ligne, donnee.superLike);
			getline(fichier, ligne);
			fillList(ligne, donnee.iLike);
			getline(fichier, ligne);
			fillList(ligne, donnee.iDislike);
			getline(fichier, ligne);
			fillList(ligne, donnee.iSuperLike);
			getline(fichier, ligne);
			donnee.description = ligne;
			getline(fichier, ligne);
			donnee.rate = ligne;

			accounts += donnee;
		}
	}
	else {
		cout << "Erreur impossible d'ouvrir le fichier" << endl;
	}
}
// Change for loops for while loops that stop at NULL so doesn't go through the entire array every time (if you have time)
string fluxy::pickUser() {
	string b = "There is no one left in your queue :/";
	int i = 0;
	int reset = 0;
	info a = accounts[0];
	while (reset == 0 && i < nbUsers) {
		reset = 1;
		for (int j = 0; j < currentUser.iLike->size(); j++) {
			if (a.name == currentUser.iLike[j] && i < nbUsers || a.name == currentUser.name && i < nbUsers) {
				i++;
				a = accounts[i];
				reset = 0;
			}
		}
		for (int k = 0; k < currentUser.iDislike->size(); k++) {
			if (a.name == currentUser.iDislike[k] && i < nbUsers || a.name == currentUser.name && i < nbUsers) {
				i++;
				a = accounts[i];
				reset = 0;
			}
		}
		for (int l = 0; l < currentUser.iSuperLike->size(); l++) {
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
		return currentPray.name;
	}
	else return b;
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
		for (int i = 0; i<nbUsers; i++) {
			a = accounts[i];
			fichier << a.name << endl;
			fichier << a.password << endl;
			int j = 0;
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
			fichier << a.rate << endl;
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
		cout << i+1 << "- " << a.name << endl;
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

bool fluxy::changePassword(string password, string password_check) {
	if (password != password_check) {
		return false;
	}
	else {
		currentUser.password = password;
	}
	tempSave(currentUser, cu);
	return true;
}

void fluxy::changeDescription(string description) {
	currentUser.description = description;
	tempSave(currentUser, cu);

}

string fluxy::checkRate(){
	return currentUser.rate;
}

string fluxy::checkDescription_U() {
	return currentUser.description;
}

string fluxy::checkDescription_P() {
	return currentPray.description;
}

void fluxy::rate() {
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
}

void fluxy::logout(){
	save();
}

void fluxy::delete_account() {
	accounts -= cu;
	nbUsers--;
}

bool fluxy::match() {
	int i = 0;
	while (currentUser.like[i] != "\0") {
		i++;
		if (currentPray.name == currentUser.like[i]) {
			return true;
		}
	}
	while (currentUser.superLike[i] != "\0") {
		i++;
		if (currentPray.name == currentUser.superLike[i]) {
			return true;
		}
	}
	return false;
}