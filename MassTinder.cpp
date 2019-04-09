// Massgames.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "fluxy.h"
#include <stdlib.h>



using namespace std;

fluxy run;
int on = 1;
int ono = -1;
string pos = "0";
int position = 0;
string answer = "0";
string user1 = "nothing";
string password;
string description = "I am an empty shell of a human being";
string pickle;
int x = 0;

int main()
{

	cout << "Hello and Welcome to name pending... \n" << endl;
	while (on == 1) {
		cout << "Please Pick one of the following\n\n1- Login\n2- New Account\n3- List current users\n4- QUIT" << endl;
		getline(cin, answer);
		system("cls");
		if (answer == "1" || answer == "2" || answer == "3" || answer == "4" || answer == "5") {
			int y = run.stringToInt(answer);
			switch (y) {
			case 1:
				cout << "Username : " << endl;
				getline(cin, user1);
				cin.clear();
				cout << "Password : " << endl;
				getline(cin, password);
				cin.clear();
				if (run.login(user1, password) == -1) {
					cout << "\nThat name was not found in our data banks" << endl;
				}
				else {
					cout << "\nWelcome to name pending...\n" << user1 << endl;
					ono = 1;
				}
				break;
			case 2:
				cin.clear();
				system("cls");
				cout << "What's your name : ";
				getline(cin, user1);
				cout << user1 << endl;
				cin.clear();
				cout << "\nWhat would you like your password to be?" << endl;
				cout << "new Password :";
				getline(cin, password);
				cout << password << endl;
				cin.clear();
				cout << "\nWould you like to describe yourself(Y/N)";
				getline(cin, pickle);
				cin.clear();
				if (pickle == "Y" || pickle == "y") {
					cout << "\nPlease provide description :" << flush;
					getline(cin, description);
					cin.clear();
				}
				run.addAccount(user1, password, description);
				break;
			case 3:
				run.listUsers();
				break;
			case 4:
				on = -1;
				ono = -1;
				break;
			case 5:
				run.save();
				break;
			default:
				break;
			}
		}
		else cout << "That answer is not accepted\n" << endl;

		while (ono == 1) {
			//system("cls");
			string a = run.pickUser();
			cout << "\nWhat do think of ...\n" << a << "\n1- Like\n2- Dislike\n3- Super Like\n4- Logout\n" << endl;
			getline(cin, answer);
			cin.clear();
			if (answer == "1" || answer == "2" || answer == "3" || answer == "4" || answer == "5") {
				int z = run.stringToInt(answer);
				switch (z) {

				case 1:
					run.like();
					break;
				case 2:
					run.dislike();
					break;
				case 3:
					run.superLike();
					break;
				case 4:
					ono = -1;
					break;
				case 5:
					run.checkStatus();
					break;
			}

			}
			else cout << "That answer is not accepted\n" << endl;
			if (run.match() == true) {
				cout << "You have a match you lucky duck" << endl;
			}
			cout << "Press any key for next JUGEMENT" << endl;
			getline(cin, pickle);
			cin.clear();
		}
	}
	run.~fluxy();
	return 0;
}