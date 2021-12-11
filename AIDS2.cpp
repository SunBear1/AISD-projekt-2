#include <iostream>

#define MAX 1000

using namespace std;

struct konfiguracja
{
	int def_rotora[MAX] = {};
	int litera_triggerujaca[MAX] = {};
	int def_reflektora[MAX] = {};
};

struct kopia {
	int instrukcja[MAX] = {};
	int indeksy[MAX] = {};
	int odbicie[MAX] = {};
};

void obrot(int indeksrotora, int iloscliter, kopia* rotor_kopia)
{
	int tmp1 = rotor_kopia[indeksrotora].instrukcja[0];
	int tmp2 = rotor_kopia[indeksrotora].indeksy[0];
	int tmp3 = rotor_kopia[indeksrotora].odbicie[0];
	for (int i = 0; i < iloscliter - 1; i++)
	{
		rotor_kopia[indeksrotora].instrukcja[i] = rotor_kopia[indeksrotora].instrukcja[i + 1];
		rotor_kopia[indeksrotora].indeksy[i] = rotor_kopia[indeksrotora].indeksy[i + 1];
		rotor_kopia[indeksrotora].odbicie[i] = rotor_kopia[indeksrotora].odbicie[i + 1];
	}
	rotor_kopia[indeksrotora].instrukcja[iloscliter - 1] = tmp1;
	rotor_kopia[indeksrotora].indeksy[iloscliter - 1] = tmp2;
	rotor_kopia[indeksrotora].odbicie[iloscliter - 1] = tmp3;
}

void zaszyfruj(int uzywanerotory, int indeksrotora[], int indeksreflektora, int szyfr, konfiguracja* rotor_orginal, konfiguracja* reflektor, int iloscliter, bool pierwsza, kopia* rotor_kopia, bool* triggerrotor2, bool* podwojny)
{
	obrot(0, iloscliter, rotor_kopia);
	if (uzywanerotory > 1 && *podwojny == false)
	{
		for (int j = 1; j <= rotor_orginal[indeksrotora[0]].litera_triggerujaca[0]; j++)
		{
			if ((rotor_orginal[indeksrotora[0]].litera_triggerujaca[j] == rotor_kopia[0].indeksy[0]) && (pierwsza == false))
			{
				*triggerrotor2 = true;
				obrot(1, iloscliter, rotor_kopia);
				break;
			}
		}
	}
	if (*podwojny == true)
	{
		for (int i = 1; i < 3; i++)
		{
			obrot(i, iloscliter, rotor_kopia);
		}
		*podwojny = false;
	}
	if (uzywanerotory > 2 && *triggerrotor2 == true)
	{
		for (int j = 1; j <= rotor_orginal[indeksrotora[1]].litera_triggerujaca[0]; j++)
		{
			int poprawka = rotor_orginal[indeksrotora[1]].litera_triggerujaca[j] - 1;
			if (poprawka == 0)
				poprawka = iloscliter;
			if (poprawka == rotor_kopia[1].indeksy[0])
			{
				*podwojny = true;
				break;
			}
		}
	}
	for (int j = 0; j < uzywanerotory; j++)
	{
		szyfr = szyfr + rotor_kopia[j].instrukcja[szyfr - 1];
		szyfr = szyfr % iloscliter;
		if (szyfr == 0)
			szyfr = iloscliter;
		if (szyfr < 0)
			szyfr = szyfr + iloscliter;
	}
	szyfr = reflektor[indeksreflektora].def_reflektora[szyfr - 1];
	for (int j = uzywanerotory - 1; j >= 0; j--)
	{
		szyfr = szyfr + rotor_kopia[j].odbicie[szyfr - 1];
		szyfr = szyfr % iloscliter;
		if (szyfr == 0)
			szyfr = iloscliter;
		if (szyfr < 0)
			szyfr = szyfr + iloscliter;
	}
	cout << szyfr << " ";
}

void konfiguracja_rotorow(konfiguracja* rotor_orginal, int iloscrotorow, int iloscliter)
{
	for (int i = 0; i < iloscrotorow; i++)
	{
		for (int j = 0; j < iloscliter; j++)
		{
			cin >> rotor_orginal[i].def_rotora[j];
		}
		cin >> rotor_orginal[i].litera_triggerujaca[0];
		for (int k = 1; k <= rotor_orginal[i].litera_triggerujaca[0]; k++)
		{
			cin >> rotor_orginal[i].litera_triggerujaca[k];
		}
	}
}

void konfiguracja_reflektorow(konfiguracja* reflektor, int iloscreflektorow, int iloscliter)
{
	for (int i = 0; i < iloscreflektorow; i++)
	{
		for (int j = 0; j < iloscliter; j++)
		{
			cin >> reflektor[i].def_reflektora[j];
		}
	}
}

void poczatkoweobroty(konfiguracja* rotor_orginal, int uzywanerotory, int iloscliter, int indeksrotora[], int pozycjarotora[], kopia* rot)
{
	for (int i = 0; i < uzywanerotory; i++)
	{
		for (int k = 0; k < iloscliter; k++)
		{
			rot[i].instrukcja[k] = rotor_orginal[indeksrotora[i]].def_rotora[k] - (k + 1);

			int y = k + rot[i].instrukcja[k];
			y = y % iloscliter;
			if (y < 0)
				y = y + iloscliter;
			rot[i].odbicie[y] = -1 * rot[i].instrukcja[k];

			rot[i].indeksy[k] = k + 1;
		}
		for (int j = 0; j < (pozycjarotora[i] - 1) % iloscliter; j++)
		{
			obrot(i, iloscliter, rot);
		}
	}
}

void dane_wiadomosci(konfiguracja* rotor_orginal, konfiguracja* reflektor, int iloscliter)
{
	int uzywanerotory, indeksreflektora, wiadomosc, indeks_wiadomosci = 0;
	bool pierwsza = true, tr2 = false, ds = false;
	bool* podwojny = &ds, * triggerrotor2 = &tr2;
	cin >> uzywanerotory;
	kopia* rotor_kopia = new kopia[uzywanerotory];
	int* indeksrotora = new int[uzywanerotory];
	int* pozycjarotora = new int[uzywanerotory];
	for (int i = 0; i < uzywanerotory; i++)
	{
		cin >> indeksrotora[i] >> pozycjarotora[i];
	}
	poczatkoweobroty(rotor_orginal, uzywanerotory, iloscliter, indeksrotora, pozycjarotora, rotor_kopia);
	cin >> indeksreflektora;
	while (true)
	{
		if (indeks_wiadomosci != 0)
			pierwsza = false;

		cin >> wiadomosc;
		if (wiadomosc == 0)
			break;
		else
		{
			zaszyfruj(uzywanerotory, indeksrotora, indeksreflektora, wiadomosc, rotor_orginal, reflektor, iloscliter, pierwsza, rotor_kopia, triggerrotor2, podwojny);
			indeks_wiadomosci++;
		}
	}
	cout << endl;
	delete[] rotor_kopia;
	delete[] pozycjarotora;
	delete[] indeksrotora;
}

int main()
{
	int iloscliter, iloscrotorow, iloscreflektorow, iloscwiadomosci;
	cin >> iloscliter >> iloscrotorow;
	konfiguracja* rotor_orginal = new konfiguracja[iloscrotorow];
	konfiguracja_rotorow(rotor_orginal, iloscrotorow, iloscliter);
	cin >> iloscreflektorow;
	konfiguracja* reflektor = new konfiguracja[iloscreflektorow];
	konfiguracja_reflektorow(reflektor, iloscreflektorow, iloscliter);
	cin >> iloscwiadomosci;
	for (int i = 0; i < iloscwiadomosci; i++)
	{
		dane_wiadomosci(rotor_orginal, reflektor, iloscliter);
	}
	delete[] rotor_orginal;
	delete[] reflektor;
	return 0;
}