//============================================================================ 
// Name: MN Interpolacja.cpp
// Author: Paweł Lodzik & Tomasz Cwik 
// Version     : 1.0.0.0
// Copyright   : 
// Description : Program wyznaczający wartości wielomianu interpolacyjnego Newtona w punktach leżących w przedziale <a, b> dla zadanej przez prowadzącego funkcji interpolowanej f(x).
//============================================================================
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

//Klasa pomocnicza Punkt
class Punkt
{
private:
	double y;
	double x;
public:
	Punkt(double x, double y) :x(x), y(y) {};
	double getX() { return this->x; };
	double getY() { return this->y; };
};

//Deklaracja funkcji pomocniczych
void wprowadzDane(int &n, double &np, double &a, double &b);
double obliczWartoscInterpolowanejFunkcji(double x);
void tablicujWezly(int wybor, vector<Punkt>& tablicaPunktow, vector<double>& wezly, int n, double h, double a, double b);
double wyznaczWspolczynnikiWielomianuNewtona(vector<Punkt>& tablicaPunktow, int n);
double wyznaczWartoscWielomianuNewtonaWDowolnymPunkcie(vector<Punkt>& tablicaWartosci, int n, double x);
void utworzRaport(int wybor, double a, double b, vector<Punkt> tablicaPunktow, vector<Punkt> wartosciFunkcji, vector<double> wartosciWielomianuNewtona, int n, double np);



int main()
{
	int n;
	double a, b, np;
	wprowadzDane(n, np, a, b);
	double h = (b - a) / n; //Różnica przedziału końcowego od przedziału początkowego dzielona przez liczbę wezłów - wzór z polecenia

	vector<double> wezly;
	vector<Punkt> tablicaPunktow;

	int wybor = 0;
	cout << "Podaj jak dobrać węzły" << endl << "1 - Rownoodległe" << endl << "2 - Dobrane optymalnie" << endl;
	cin >> wybor;

	tablicujWezly(wybor, tablicaPunktow, wezly, n, h, a, b);


	double hp = (b - a) / np; //Szerokość podprzedziału między punktami dla testów funkcji interpolowanej i interpolujacej

	vector<Punkt> wartosciFunkcji;
	vector<double>wartosciWielomianuNewtona;

	for (int j = 0; j <= np; j++) // petla która tworzy tablice punktów xpj
	{
		double xi = a + j*hp;
		wartosciWielomianuNewtona.push_back(wyznaczWartoscWielomianuNewtonaWDowolnymPunkcie(tablicaPunktow, n, xi)); //a+j*hp = xpj, kolejne punkty do obliczenia
		wartosciFunkcji.push_back(Punkt(xi, obliczWartoscInterpolowanejFunkcji(xi))); // wartości funkcji f(x) interpolowanej
	}
	utworzRaport(wybor, a, b, tablicaPunktow, wartosciFunkcji, wartosciWielomianuNewtona, n, np);
	system("pause");
    return 0;
}

//Implementacja funkcji pomocniczych
void wprowadzDane(int &n, double &np, double &a, double &b)
{
	cout << "Podaj a: " << endl;
	cin >> a;
	cout << "Podaj b: " << endl;
	cin >> b;
	cout << "Podaj n:" << endl;
	cin >> n;
	cout << "Podaj np:" << endl;
	cin >> np;
}

double obliczWartoscInterpolowanejFunkcji(double x)
{
	return abs(x) / (1 + pow(x, 2)); //Wzór funkcji interpolowanej z polecenia
}

void tablicujWezly(int wybor, vector<Punkt>& tablicaPunktow, vector<double>& wezly, int n, double h, double a, double b)
{
	if (wybor == 1)//rownoodlegle
	{
		for (int i = 0; i <= n; i++)
		{
			wezly.push_back(a + i*h); //Wzór na xi z polecnia xi = a + i * h
		}
	}
	else
	{
		// optymalnie
		for (double i = 0; i <= n; i++)
		{
			wezly.push_back((a + b) / 2 + ((b - a)*cos((2 * i + 1) / (2 * n + 2)*M_PI)) / 2); //Wzór na dobór optymalny węzłów Wzór 13 strona 3
		}
	}

	double x = 0, y = 0;
	for (int i = 0; i <= n; i++)
	{
		x = wezly[i];
		y = obliczWartoscInterpolowanejFunkcji(x); //Obliczanie wartości funkcji interpolowanej dla kolejnych węzłów
		tablicaPunktow.push_back(Punkt(x, y));
	}
}

double wyznaczWspolczynnikiWielomianuNewtona(vector<Punkt>& tablicaPunktow, int n)
{
	double iloczyn = 1.0;
	double wspolczynnikNewtona = 0;


	//Obliczamy wielomian czynnikowy stopnia i-tego określonym następująco - Wzór 7 strona 2
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (j != i)
			{
				iloczyn *= tablicaPunktow[i].getX() - tablicaPunktow[j].getX();
			}
		}
		wspolczynnikNewtona += (tablicaPunktow[i].getY() / iloczyn);
		iloczyn = 1.0;
	}
	return wspolczynnikNewtona;
}

double wyznaczWartoscWielomianuNewtonaWDowolnymPunkcie(vector<Punkt>& tablicaPunktow, int n, double x)
{
	double wartoscNetwona = 0;
	double omega = 1.0;
	//Wykorzystujemy Wzór Newtona - Wzór 5 strona 2
	for (int i = 0; i <= n; i++)
	{
		//Omega
		for (int j = 0; j < i; j++)
		{
			//Wzór na omege - Wzór 6 strona 2
			omega *= (x - tablicaPunktow[j].getX());
		}
		wartoscNetwona += wyznaczWspolczynnikiWielomianuNewtona(tablicaPunktow, i)*omega;
		omega = 1.0;
	}
	return wartoscNetwona;
}



void utworzRaport(int wybor, double a, double b, vector<Punkt> tablicaPunktow, vector<Punkt> wartosciFunkcji, vector<double> wartosciWielomianuNewtona, int n, double np)
{

	string nazwaPliku = "raport_";

	if (wybor == 1) {
		nazwaPliku += "rownoodlegle";
	}
	else {
		nazwaPliku += "optymalnie";
	}

	nazwaPliku += "_";
	nazwaPliku += std::to_string(a);
	nazwaPliku += "_";
	nazwaPliku += to_string(b);
	nazwaPliku += "_";
	nazwaPliku += to_string(n);
	nazwaPliku += "_";
	nazwaPliku += to_string(np);
	nazwaPliku += ".txt";
	nazwaPliku.begin();
	fstream plik(nazwaPliku.c_str(), fstream::out);
	//Zapisanie wezlow interpolacyjnych x0, x1,..., xn
	plik << "xi\t";
	string str;
	for (int i = 0; i <= n; i++) {
		str = to_string(tablicaPunktow[i].getX());
		std::replace(str.begin(), str.end(), '.', ',');
		plik << str << "\t";
	}
	//Zapisanie wezlow interpolacyjnych f(x0), f(x1),..., f(xn)
	plik << "\nf(xi)\t";
	for (int i = 0; i <= n; i++) {
		str = to_string(tablicaPunktow[i].getY());
		std::replace(str.begin(), str.end(), '.', ',');
		plik << str << "\t";
	}

	plik << endl << endl;

	//Zapisanie tabeli dla np wynikow
	plik << "XPj\tf(XPj)\tL(XPj)\t|f(XPj)-L(XPj)|" << endl;
	for (int i = 0; i <= np; i++) {
		string str1 = to_string(wartosciFunkcji[i].getX());
		string str2 = to_string(wartosciFunkcji[i].getY());
		string str3 = to_string(wartosciWielomianuNewtona[i]);
		string str4 = to_string(fabs(wartosciFunkcji[i].getY() - wartosciWielomianuNewtona[i]));
		std::replace(str1.begin(), str1.end(), '.', ',');
		std::replace(str2.begin(), str2.end(), '.', ',');
		std::replace(str3.begin(), str3.end(), '.', ',');
		std::replace(str4.begin(), str4.end(), '.', ',');
		plik << str1 << "\t" << str2 << "\t" << str3 << "\t" << str4 << endl;
	}
	plik.flush();
	plik.close();
	cout << endl << "PLIK O NAZWIE " << nazwaPliku.c_str() << " ZOSTAL ZAPISANY" << endl;
}