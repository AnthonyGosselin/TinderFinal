#ifndef VECTEUR_H
#define VECTEUR_H

#include <iostream>

using namespace std;

template <typename T>
class vecteur
{
public:
	vecteur()
	{
		vec = new T[CAPACITE];
		taille = 0;
		cap = CAPACITE;
	}

	~vecteur()
	{
		taille = 0;
		cap = CAPACITE;
	}

	int getTaille() {
		return taille;
	}
	int getCapacite() {
		return cap;
	}

	void viderVec() {
		vec = new T[CAPACITE];
		taille = 0;
		cap = CAPACITE;
	}
	bool operator+=(T newItem) {
		if (taille < cap)
		{
			vec[taille] = newItem;

		}
		else
		{
			cap = cap * 2;
			T* vecTemp = new T[cap];
			for (int i = 0; i < taille; i++) {
				vecTemp[i] = vec[i];
			}
			vec = new T[cap];
			for (int i = 0; i < taille; i++) {
				vec[i] = vecTemp[i];
			}
			delete[] vecTemp;
			vec[taille] = newItem;
		}
		taille++;
		return true;
	}
	T operator[](int i) {
		if (i <= taille) {
			T retour = vec[i];
			return retour;
		}
		else {
			cout << "index out of bounds" << endl;
		}
	}

	bool operator-=(int c) {
		for (int i = c + 1; i <= taille; i++) {
			vec[i - 1] = vec[i];
		}
		taille--;
		return true;
	}

protected:

private:
	T * vec;
	int taille;
	int const CAPACITE = 10;
	int cap;
};

#endif // VECTEUR_H
