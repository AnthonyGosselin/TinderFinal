#ifndef FLUXY_H
#define FLUXY_H

#include <iostream>
#include <fstream>
#include <string>
#include "vecteur.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

class fluxy
{

private:
	struct info {
		string name;
		string password = "none";
		string like[100];
		string dislike[100];
		string superLike[100];
		string iLike[100];
		string iDislike[100];
		string iSuperLike[100];
		string description = "I am so bored I can't even take the time to enter a description of myself";
		string rate = "none";
	};
	info donnee;
	string const CoorsLight = "info.txt";
	string answer;
	string newUser;
	info currentUser;
	info currentPray;
	int nbUsers = 0;
	vecteur<info> accounts;
	vecteur<info> accounts2;
	int cu;
	int cp;


public:
	fluxy(); // creates object and placesvoid pulls all info from .txt file
	virtual ~fluxy();
	void addAccount(string newAccount, string password, string description); // creates new account and prompts for all info
	void like(); // adds a like to the current user
	void dislike(); //adds a dislike to the current user
	void superLike(); //adds a superlike to the current user
	string pickUser(); //outputs user based on like and dislike ratio
	int login(string name, string password); // ask for name and password and check if they correspond
	//string sponsor(); //at random intervals spawns commercial sponsor in your face
	void listUsers(); // list all current users in the network
	int stringToInt(string numberToConvert); // converts string to int
	void checkStatus(); // outputs the likes and dislikes
	void save(); // save all data in a txt fill
	bool changePassword(string password, string password_check);
	void changeDescription(string description);
	string checkRate();
	string checkDescription_U();
	string checkDescription_P();
	void logout();
	void delete_account();
	bool match();

protected:
	int getNbUsers(); // returns total amount of users
	//void save(); // save all data in a txt file
	void listAccounts(); // pulls all accounts and relitave info from txt file
	void fillList(string stuff, string *c); // puts elements of string  seperated by /t into an array
	string toLower(string name); // sends string to lowercase
	void tempSave(info a, int aa, info b, int bb);
	void tempSave(info a, int aa);
	void rate(); // example this person is highly rated amongts your peers



};

#endif // FLUXY_H