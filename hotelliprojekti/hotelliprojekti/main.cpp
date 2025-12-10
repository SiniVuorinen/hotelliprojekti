//Sini Vuorinen
/*Hotellivarausj‰rjestelm‰, C++ peruskurssin lopputyˆ*/

#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<ctime>
#include<cstdlib>
#include<limits>
using namespace std;

//VAKIOT
const int MAX_HUONEET = 300;
const int MIN_HUONEET = 40;

const int MAX_VARAUKSET = 5;	//kerralla varattavien huoneiden maksimim‰‰r‰
const int MAX_AIKA = 7;			//maksimi vuorokausim‰‰r‰

const double HINTA_1_HENGEN = 100.0;
const double HINTA_2_HENGEN = 150.0;

struct Huone {
	int numero;			// huonenumero
	int kapasiteetti;	// 1 tai 2
	bool varattu;		// true/false
	string varaaja;		// "" jos ei varausta
	int varausID;		// 0 jos ei varausta
};

void paaValikonToiminnot(Huone huoneet[], int huoneMaara);
void teeVarausToiminnot(Huone huoneet[], int huoneMaara);
void peruVarausToiminnot(Huone huoneet[], int huoneMaara);
void muokkaaVaraustaToiminnot(Huone huoneet[], int huoneMaara);
void vapaatHuoneetToiminnot(Huone huoneet[], int huoneMaara);
void teeVaraus(Huone huoneet[], int huoneMaara);
void peruVarausIDlla(Huone huoneet[], int huoneMaara);
void peruVarausNimella(Huone huoneet[], int huoneMaara);
void muokkaaVaraus(Huone huoneet[], int huoneMaara);
void tulostaHuoneet(Huone huoneet[], int huoneMaara);
void tulostaVapaatHuoneet(Huone huoneet[], int huoneMaara);
void tulostaVaratutHuoneet(Huone huoneet[], int huoneMaara);
int haeVapaatHuoneet(Huone huoneet[], int huoneMaara);
int haeVapaaHuone(Huone huoneet[], int huoneMaara, int tyyppi);

/*funktiot syˆtteen tarkistuksille */
void tarkistaOnkoNumero(int& muuttuja) {
	if (cin.fail() || cin.peek() != '\n') {
		//virhe = true;
		cout << "\nVirheellinen syˆte. Valitse hyv‰ksytt‰v‰ numero.\n" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("pause");
		muuttuja = -1;
	}
	muuttuja;
}
bool tarkistaValidiNimi(const string& nimi) {
	//tarkistetaan jokainen merkki
	for (char c : nimi) {
		if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') {
			cout << "\nNimessa on sallimattomia merkkej‰.\n";
			cout << "\nSallitut merkit: kirjaimet, v‰lilyˆnti, - ja heittomerkit\n\n";
			system("pause");
			return false;
		}
	}
	true;
}
int virheellinenNumero() {
	cout << "\nVirheellinen syˆte. Valitse valikossa oleva numero.\n" << endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("pause");
	return -1;
}

//huoneiden arpomiset ja k‰sittelyt
void arvoHuoneet(Huone huoneet[], int& huoneMaara) {
	// arpoo huoneita maksimi ja minimin‰‰rien mukaan
	int maara = rand() % (MAX_HUONEET - MIN_HUONEET + 1) + MIN_HUONEET;

	if (maara % 2 != 0) {
		maara++;  //muutetaan parilliseksi
	}
	huoneMaara = maara;
	int puolet = maara / 2;

	// Yhden hengen huoneet
	for (int i = 0; i < puolet; i++) {
		huoneet[i].numero = i + 1;
		huoneet[i].kapasiteetti = 1;
		huoneet[i].varattu = false;
		huoneet[i].varaaja = "";
		huoneet[i].varausID = 0;
	}
	// Kahden hengen huoneet
	for (int i = puolet; i < maara; i++) {
		huoneet[i].numero = i + 1;
		huoneet[i].kapasiteetti = 2;
		huoneet[i].varattu = false;
		huoneet[i].varaaja = "";
		huoneet[i].varausID = 0;
	}
}
void lueHuoneet(Huone huoneet[], int& huoneMaara) {
	ifstream file("huoneet.txt");
	huoneMaara = 0;

	while (true) {
		int numero, kapasiteetti, varattuInt, varausID;

		// yritet‰‰n lukea 4 ensimm‰ist‰ kentt‰‰
		if (!(file >> numero >> kapasiteetti >> varattuInt >> varausID)) {
			break;
		}
		string varaaja;
		getline(file, varaaja); // luetaan loppurivi

		// poista mahdollinen tyhj‰ v‰li
		if (!varaaja.empty() && varaaja[0] == ' ') {
			varaaja.erase(0, 1);
		}
		huoneet[huoneMaara].numero = numero;
		huoneet[huoneMaara].kapasiteetti = kapasiteetti;
		huoneet[huoneMaara].varattu = (varattuInt != 0);
		huoneet[huoneMaara].varausID = varausID;

		if (huoneet[huoneMaara].varattu && varaaja != "-") {
			huoneet[huoneMaara].varaaja = varaaja;
		}
		else {
			huoneet[huoneMaara].varaaja = "";
		}

		huoneMaara++;
		if (huoneMaara >= MAX_HUONEET) break;
	}
}
void tallennaHuoneet(Huone huoneet[], int huoneMaara) {
	ofstream file("huoneet.txt");
	for (int i = 0; i < huoneMaara; i++) {
		file << huoneet[i].numero << " "
			<< huoneet[i].kapasiteetti << " "
			<< huoneet[i].varattu << " "
			<< (huoneet[i].varattu ? huoneet[i].varaaja : "-") << " "
			<< huoneet[i].varausID << "\n";

		if (huoneet[i].varattu) {
			file << huoneet[i].varaaja;
		}
		else {
			file << "-";
		}
		file << "\n";
	}
}

//apufunktiot
int haeVapaatHuoneet(Huone huoneet[], int huoneMaara) {
	int laskuri = 0;
	for (int i = 0; i < huoneMaara; i++) {
		if (!huoneet[i].varattu) {
			laskuri++;
		}
	}
	return laskuri;
}

int haeVapaaHuone(Huone huoneet[], int huoneMaara, int tyyppi) {
	for (int i = 0; i < huoneMaara; i++) {
		if (!huoneet[i].varattu && huoneet[i].kapasiteetti == tyyppi) {
			return i;
		}
	}
	return -1;
}


//MAIN
int main() {
	setlocale(LC_ALL, "fi_FI");
	srand(time(NULL));  // satunnaislukujen siemen

	Huone huoneet[MAX_HUONEET];
	int huoneMaara = 0;

	//tarkistetaan tiedoston tila
	ifstream file("huoneet.txt");
	bool tyhja = file.peek() == ifstream::traits_type::eof();
	file.close();

	//jos tiedosto on tyhj‰, kutsutaan funktiot huoneiden arvonnalle ja tallennukselle
	if (tyhja) {

		cout << "\nHotellissa ei ole viel‰ huoneita. Arvotaan huoneet...\n";
		system("pause");

		arvoHuoneet(huoneet, huoneMaara);
		tallennaHuoneet(huoneet, huoneMaara);

		cout << "\nHuoneet arvottu ja tallennettu.\n\n";
		cout << "=====================================\n";
		cout << "          Tervetuloa.\n";
		cout << "=====================================\n\n";
		system("pause");
	}
	else {
		lueHuoneet(huoneet, huoneMaara);
	}
	paaValikonToiminnot(huoneet, huoneMaara);
	return 0;
}


//Valikot
void paaValikonToiminnot(Huone huoneet[], int huoneMaara) {
	int valinta = -1;

	while (valinta != 0) {
		system("cls");
		cout << endl;
		cout << "=====================================\n";
		cout << "     Hotellin varausj‰rjestelm‰\n";
		cout << "=====================================\n";
		cout << "1. Tee uusi varaus\n";
		cout << "2. Peru varaus\n";
		cout << "3. Muokkaa varausta\n";
		cout << "4. N‰yt‰ huoneet\n";
		cout << "0. Lopeta\n";
		cout << "=====================================\n";
		cout << "Valitse toiminto: ";

		cin >> valinta;
		tarkistaOnkoNumero(valinta);

		if (valinta == -1) {
			continue;
		}
		if (valinta == 1) {
			teeVarausToiminnot(huoneet, huoneMaara);
		}
		else if (valinta == 2) {
			peruVarausToiminnot(huoneet, huoneMaara);
		}
		else if (valinta == 3) {
			muokkaaVaraustaToiminnot(huoneet, huoneMaara);
		}
		else if (valinta == 4) {
			vapaatHuoneetToiminnot(huoneet, huoneMaara);
		}
		else if (valinta == 0) {
			cout << "\nOhjelma p‰‰ttyy\n";
			cout << "\nKiitos ja tervetuloa uudelleen!\n";
		}
		else {
			virheellinenNumero();
		}
	}
}

void teeVarausToiminnot(Huone huoneet[], int huoneMaara) {
	int valinta = -1;

	while (valinta != 0) {
		system("cls");
		cout << "=====================================\n";
		cout << "         Tee uusi varaus\n";
		cout << "=====================================\n";
		cout << "1. Aloita tietojen syˆtt‰minen\n";
		cout << "0. Palaa p‰‰valikkoon\n";
		cout << "=====================================\n";
		cout << "Valitse toiminto: ";

		cin >> valinta;
		tarkistaOnkoNumero(valinta);
		if (valinta == -1) {
			continue;
		}

		if (valinta == 1) {
			teeVaraus(huoneet, huoneMaara);
		}
		else if (valinta == 0) {
			break;
		}
		else {
			virheellinenNumero();
		}
	}
}

void peruVarausToiminnot(Huone huoneet[], int huoneMaara) {
	int valinta = -1;

	while (valinta != 0) {
		system("cls");
		cout << "=====================================\n";
		cout << "           Peru varauksia\n";
		cout << "=====================================\n";
		cout << "1. Peru varaajan nimell‰\n";
		cout << "2. Peru varausnumerolla\n";
		cout << "0. Palaa p‰‰valikkoon\n";
		cout << "=====================================\n";
		cout << "Valitse toiminto: ";

		cin >> valinta;
		tarkistaOnkoNumero(valinta);
		if (valinta == -1) {
			continue;
		}

		if (valinta == 1) {
			peruVarausNimella(huoneet, huoneMaara);
			system("pause");
		}
		else if (valinta == 2) {
			peruVarausIDlla(huoneet, huoneMaara);
			system("pause");
		}
		else if (valinta == 0) {
			return;
		}
		else {
			virheellinenNumero();
		}
	}
}

void muokkaaVaraustaToiminnot(Huone huoneet[], int huoneMaara) {
	int valinta = -1;

	while (valinta != 0) {
		system("cls");
		cout << "=====================================\n";
		cout << "	  Muokkaa varauksia\n";
		cout << "=====================================\n";
		cout << "1. Muokkaa varausta\n";
		cout << "0. Palaa p‰‰valikkoon\n";
		cout << "=====================================\n";
		cout << "Valitse toiminto: ";

		cin >> valinta;
		tarkistaOnkoNumero(valinta);
		if (valinta == -1) {
			continue;
		}
		if (valinta == 1) {
			muokkaaVaraus(huoneet, huoneMaara);
			system("pause");
			continue;
		}
		else if (valinta == 0) {
			break;
		}
		else {
			virheellinenNumero();
		}
	}
}

void vapaatHuoneetToiminnot(Huone huoneet[], int huoneMaara) {
	int valinta = -1;

	while (valinta != 0) {
		system("cls");
		cout << "=====================================\n";
		cout << "          N‰yt‰ huoneet\n";
		cout << "=====================================\n";
		cout << "1. N‰yt‰ hotellin kaikki huoneet \n";
		cout << "2. N‰yt‰ vain vapaat huoneet\n";
		cout << "3. N‰yt‰ vain varatut huoneet\n";
		cout << "0. Palaa p‰‰valikkoon\n";
		cout << "=====================================\n";
		cout << "Valitse toiminto: ";

		cin >> valinta;
		tarkistaOnkoNumero(valinta);
		if (valinta == -1) {
			continue;
		}

		if (valinta == 1) {
			tulostaHuoneet(huoneet, huoneMaara);
			system("pause");
		}
		else if (valinta == 2)
		{
			tulostaVapaatHuoneet(huoneet, huoneMaara);
			system("pause");
		}
		else if (valinta == 3)
		{
			tulostaVaratutHuoneet(huoneet, huoneMaara);
			system("pause");
		}
		else if (valinta == 0) {
			break;
		}
		else {
			virheellinenNumero();
		}
	}
}

//P‰‰funktiot
//Varauksen tekeminen
void teeVaraus(Huone huoneet[], int huoneMaara) {
	int vapaatHuoneet = haeVapaatHuoneet(huoneet, huoneMaara);
	int sallitutVaraukset;
	string varaaja = "-1";
	int varaukset = -1;
	int huone = -1;
	int vuorokaudet = -1;
	bool tallenna;

	//vapaiden huoneiden tarkistus
	if (vapaatHuoneet == 0) {
		cout << "\nPahoittelut. Kaikki huoneet ovat varattuina.\n";
		system("pause");
		return;
	}
	if (vapaatHuoneet < MAX_VARAUKSET) {
		sallitutVaraukset = vapaatHuoneet;
	}
	else {
		sallitutVaraukset = MAX_VARAUKSET;
	}

	// varaajan nimi
	while (true) {
		system("cls");
		cout << "\n0 peruuttaa \n\n";
		cout << "\nAnna varaajan nimi: \n";

		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjent‰‰ rivin
		getline(cin, varaaja);

		if (varaaja == "0") {
			cout << "\nPalataan p‰‰valikkoon\n";
			system("pause");
			return;
		}

		if (varaaja.empty()) {
			cout << "\nNimi ei voi olla tyhj‰.\n";
			system("pause");
			continue;
		}
		if (!tarkistaValidiNimi(varaaja)) {
			continue;
		}
		break;
	}

	// varattavien huoneiden m‰‰r‰
	while (true) {
		system("cls");
		cout << "\n0 peruuttaa \n\n";
		cout << "\nVoit varata kerralla enint‰‰n " << sallitutVaraukset << " huonetta\n";
		cout << "Montako huonetta haluat varata? \n";
		cin >> varaukset;
		tarkistaOnkoNumero(varaukset);

		if (varaukset == 0) {
			cout << "\nPalataan p‰‰valikkoon\n";
			system("pause");
			return;
		}

		if (varaukset < 1 || varaukset > sallitutVaraukset) {
			cout << "\nAnna luku valilta 1 - " << sallitutVaraukset << ".\n";
			system("pause");
			continue;
		}
		break;
	}

	// huonetyypin valinte (1hh / 2hh)
	while (true) {
		system("cls");
		cout << "\n0 peruuttaa \n\n";
		cout << "\nHaluatko varata 1 vai 2 hengen huoneen?\n";
		cin >> huone;
		tarkistaOnkoNumero(huone);

		if (huone == 0) {
			cout << "\nPalataan p‰‰valikkoon\n";
			system("pause");
			return;
		}
		if (huone < 1 || huone > 2) {
			cout << "\nAnna luku 1 tai 2.\n";
			system("pause");
			continue;
		}
		break;
	}

	// ˆiden m‰‰r‰
	while (true) {
		system("cls");
		cout << "\n0 peruuttaa \n\n";
		cout << "\nKuinka monelle p‰iv‰lle varaat huoneen? (1 - " << MAX_AIKA << ")\n";
		cin >> vuorokaudet;
		tarkistaOnkoNumero(vuorokaudet);

		if (vuorokaudet == 0) {
			cout << "\nPalataan p‰‰valikkoon\n";
			system("pause");
			return;
		}
		if (vuorokaudet < 1 || vuorokaudet > MAX_AIKA) {
			cout << "\nAnna luku v‰lill‰ 1 - " << MAX_AIKA << ".\n";
			system("pause");
			continue;
		}
		break;
	}

	// TARKISTUS JOS KAIKKI TIEDOT SAATIIN
	if (!varaaja.empty() && varaukset > 0 && huone > 0 && vuorokaudet > 0) {

		double hintaPerYo = (huone == 1 ? 100.0 : 150.0);
		double summa = hintaPerYo * varaukset * vuorokaudet;

		// Alennus: 0%, 10% tai 20%
		int arpa = rand() % 3;
		// Hinnan laskeminen
		int alennus = (arpa == 0 ? 0 : (arpa == 1 ? 10 : 20));
		double aleEuro = summa * (alennus / 100.0);
		double loppuhinta = summa - aleEuro;

		cout << "========================================\n";
		cout << "      Tarkista varaustiedot \n";
		cout << "========================================\n";
		cout << "Varaajan nimi:      " << varaaja << "\n";
		cout << "Varattavat huoneet: " << varaukset << "\n";
		cout << "Huoneen tyyppi:     " << huone << " hh\n";
		cout << "Vuorokaudet:        " << vuorokaudet << "\n";
		cout << "Hinta yhteens‰:     " << loppuhinta << " Ä (alennus " << alennus << "%)\n";
		cout << "========================================\n";
		cout << "Hyv‰ksyt‰‰nkˆ varaus? (1 = kyll‰, 0 = ei): ";
		cin >> tallenna;

		if (tallenna == true) {
			// varauksen toteutus
			for (int i = 0; i < varaukset; i++) {

				int vapaaIndeksi = haeVapaaHuone(huoneet, huoneMaara, huone);
				if (vapaaIndeksi == -1) {
					cout << "\nHuoneita ei en‰‰ ole vapaana!\n";
					system("pause");
					break;
				}
				huoneet[vapaaIndeksi].varattu = true;
				huoneet[vapaaIndeksi].varaaja = varaaja;
				huoneet[vapaaIndeksi].varausID = rand() % 90000 + 10000;
			}
			tallennaHuoneet(huoneet, huoneMaara);

			system("cls");
			cout << "\n=====================================\n";
			cout << "        Varaus hyv‰ksytty.\n";
			cout << "   Tiedot tallennettu j‰rjestelm‰‰n.\n";
			cout << "=====================================\n";
			system("pause");
		}
		else {
			system("cls");
			cout << "\n======================================\n";
			cout << "          Varaus peruttu.\n";
			cout << "   Tietoja ei tallennettu j‰rjestelm‰‰n.\n";
			cout << "======================================\n";
			system("pause");
		}
	}
	else {
		cout << "\nTietojen syˆtt‰misess‰ on virhe.\n";
		system("pause");
	}
}

//perumisfunktiot
void peruVarausIDlla(Huone huoneet[], int huoneMaara) {
	system("cls");
	int id;
	cout << "Anna varausnumero: ";
	cin >> id;

	for (int i = 0; i < huoneMaara; i++) {
		if (huoneet[i].varausID == id) {
			if (!huoneet[i].varattu) {
				cout << "Huone ei ole varattu.\n";
				return;
			}
			else {
				// Perutaan varaus
				huoneet[i].varattu = false;
				huoneet[i].varaaja = "";
				huoneet[i].varausID = 0;
				tallennaHuoneet(huoneet, huoneMaara);

				cout << "\nVarauksen peruminen onnistui.\n";
				return;
			}
		}
	}
	cout << "Huonetta ei lˆytynyt.\n";
}

void peruVarausNimella(Huone huoneet[], int huoneMaara) {
	system("cls");
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	string nimi;
	cout << "Anna varaajan nimi: ";
	getline(cin, nimi);

	for (int i = 0; i < huoneMaara; i++) {
		if (huoneet[i].varaaja == nimi) {
			if (!huoneet[i].varattu) {
				cout << "Huone ei ole varattu.\n";
				return;
			}
			else {
				// Perutaan varaus
				huoneet[i].varattu = false;
				huoneet[i].varaaja = "";
				huoneet[i].varausID = 0;
				tallennaHuoneet(huoneet, huoneMaara);

				cout << "\nVarauksen peruminen onnistui.\n";
				return;
			}
		}
	}
	cout << "Huonetta ei lˆytynyt.\n";
}

//Varauksen muokkaaminen
void muokkaaVaraus(Huone huoneet[], int huoneMaara) {

	system("cls");
	cout << "=====================================\n";
	cout << "          Varauksen muokkaus\n";
	cout << "=====================================\n";

	int hakutapa = -1;
	cout << "Haluatko muokata:\n";
	cout << "1. Varausnumerolla\n";
	cout << "2. Varaajan nimell‰\n";
	cout << "0. Peruuta\n";
	cout << "Valinta: ";
	cin >> hakutapa;
	tarkistaOnkoNumero(hakutapa);
	if (hakutapa == 0) {
		return;
	}

	int id = -1;
	string nimi = "";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');  // tyhjennet‰‰n puskuri

	// haku idll‰
	if (hakutapa == 1) {
		cout << "Anna varausnumero: ";
		cin >> id;
		tarkistaOnkoNumero(id);
		if (id <= 0) {
			return;
		}
	}
	// haku nimell‰
	else if (hakutapa == 2) {
		cout << "Anna varaajan nimi: ";
		getline(cin, nimi);
		if (nimi == "0") {
			return;
		}
	}
	else {
		virheellinenNumero();
		return;
	}

	// etsit‰‰n varaus
	int indeksi = -1;
	for (int i = 0; i < huoneMaara; i++) {
		if (!huoneet[i].varattu) continue;

		bool match = false;

		if (hakutapa == 1 && huoneet[i].varausID == id) {
			match = true;
		}
		if (hakutapa == 2 && huoneet[i].varaaja == nimi) {
			match = true;
		}
		if (match) {
			indeksi = i;
			break;
		}
	}
	if (indeksi == -1) {
		cout << "\nVarausta ei lˆydetty.\n";
		system("pause");
		return;
	}

	// n‰ytet‰‰n tiedot
	system("cls");
	cout << "=====================================\n";
	cout << "     Muutettavat varauksen tiedot\n";
	cout << "=====================================\n";

	cout << "Huone nro: " << huoneet[indeksi].numero << "\n";
	cout << "Huonetyyppi: " << huoneet[indeksi].kapasiteetti << " hh\n";
	cout << "Varaaja: " << huoneet[indeksi].varaaja << "\n";
	cout << "Varaus ID: " << huoneet[indeksi].varausID << "\n";
	cout << "-------------------------------------\n";

	cout << "Mit‰ haluat muokata?\n";
	cout << "1. Varaajan nimi\n";
	cout << "2. P‰ivien m‰‰r‰ (hinnanlasku uusiksi)\n";
	cout << "0. Peruuta\n";
	cout << "Valinta: ";

	int valinta;
	cin >> valinta;
	tarkistaOnkoNumero(valinta);
	if (valinta == 0) {
		return;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// MUOKKAUS: NIMI
	if (valinta == 1) {
		string uusiNimi;
		cout << "\nAnna uusi varaajan nimi: ";
		getline(cin, uusiNimi);

		if (!uusiNimi.empty()) {
			huoneet[indeksi].varaaja = uusiNimi;
			cout << "Nimi paivitetty.\n";
		}
	}

	//muokattavat p‰iv‰t
	else if (valinta == 2) {
		int paivat;
		cout << "\nAnna uusi p‰ivien m‰‰r‰: ";
		cin >> paivat;

		tarkistaOnkoNumero(paivat);
		if (paivat < 1) {
			cout << "Virheellinen m‰‰r‰.\n";
			system("pause");
			return;
		}
		double hinta = (huoneet[indeksi].kapasiteetti == 1 ? 100.0 : 150.0) * paivat;
		int arpa = rand() % 3;
		int alennus = (arpa == 0 ? 0 : (arpa == 1 ? 10 : 20));
		double loppuhinta = hinta - (hinta * (alennus / 100.0));

		cout << "\nUusi hinta: " << loppuhinta << " Ä (alennus " << alennus << "%)\n";
	}
	else {
		virheellinenNumero();
		return;
	}
	// tallennus
	tallennaHuoneet(huoneet, huoneMaara);
	cout << "\n=====================================\n";
	cout << "       Muutokset tallennettu.\n";
	cout << "=====================================\n";
	system("pause");
}


//Huoneiden tulostukset
//kaikki huoneet
void tulostaHuoneet(Huone huoneet[], int huoneMaara) {
	system("cls");
	cout << "=====================================\n";
	cout << "         Kaikki Huoneet\n";
	cout << "=====================================\n";

	for (int i = 0; i < huoneMaara; i++) {
		cout << "Huoneen numero: " << huoneet[i].numero << "\n";

		if (huoneet[i].kapasiteetti == 1) {
			cout << "1 hengen huone\n";
		}
		else {
			cout << "2 hengen huone\n";
		}

		if (huoneet[i].varattu) {
			cout << "Varattu\n";
		}
		else {
			cout << "Vapaa\n";
		}

		cout << "Varaajan tiedot: ";
		if (huoneet[i].varattu) {
			cout << huoneet[i].varaaja << "\n";
		}
		else {
			cout << "-\n";
		}

		cout << "Varaus ID: ";
		if (huoneet[i].varattu) {
			cout << huoneet[i].varausID << "\n";
		}
		else {
			cout << "-\n";
		}

		cout << "-------------------------------------\n";
	}
	cout << "=====================================\n";
	cout << "Paina mit‰ tahansa jatkaaksesi.\n";
}

//vain vapaat huoneet
void tulostaVapaatHuoneet(Huone huoneet[], int huoneMaara) {
	system("cls");
	cout << "=====================================\n";
	cout << "        Vapaat Huoneet\n";
	cout << "=====================================\n";

	bool loytyi = false;

	for (int i = 0; i < huoneMaara; i++)
	{
		if (!huoneet[i].varattu) {

			loytyi = true;
			cout << "Huoneen numero: " << huoneet[i].numero << "\n";
			if (huoneet[i].kapasiteetti == 1) {
				cout << "1 hengen huone\n";
			}
			else {
				cout << "2 hengen huone\n";
			}
			cout << "Tila: Vapaa\n";
			cout << "-------------------------------------\n";
		}
	}
	if (!loytyi) {
		cout << "Ei vapaita huoneita.\n";
	}
	cout << "=====================================\n";
	cout << "Paina mit‰ tahansa jatkaaksesi.\n";
}

//vain varatut huoneet
void tulostaVaratutHuoneet(Huone huoneet[], int huoneMaara) {
	system("cls");
	cout << "=====================================\n";
	cout << "         Varatut Huoneet\n";
	cout << "=====================================\n";

	bool loytyi = false;

	for (int i = 0; i < huoneMaara; i++)
	{
		if (huoneet[i].varattu) {

			loytyi = true;

			cout << "Huoneen numero: " << huoneet[i].numero << "\n";

			if (huoneet[i].kapasiteetti == 1) {
				cout << "1 hengen huone\n";
			}
			else {
				cout << "2 hengen huone\n";
			}

			cout << "Tila: Varattu\n";
			cout << "Varaaja: " << huoneet[i].varaaja << "\n";
			cout << "Varaus ID: " << huoneet[i].varausID << "\n";
			cout << "-------------------------------------\n";
		}
	}

	if (!loytyi) {
		cout << "Ei varattuja huoneita.\n";
	}

	cout << "=====================================\n";
	cout << "Paina mit‰ tahansa jatkaaksesi.\n";
}