#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <windows.h>
#include <algorithm>



using namespace std;

struct osoba
{
    int id = 0;
    int idUzytkownika = 0;
    string imie, nazwisko, adres, email, nrTel;
};

struct uzytkownik
{
    int id = 0;
    string login, haslo;
};


string wczytajLinie()
{
    string wejscie;
    cin.sync();
    getline(cin, wejscie);
    return wejscie;
}

int znajdzOstatnieIdWPliku(const string& nazwaPliku)
{
    ifstream plik(nazwaPliku);
    int ostatnieId = 0;
    if (plik.is_open())
    {
        string linia;
        while (getline(plik, linia))
        {
            stringstream ss(linia);
            string pole;
            getline(ss, pole, '|');
            ostatnieId = stoi(pole);
        }
        plik.close();
    }
    return ostatnieId;
}

void utworzPlikJesliNieIstnieje(const string& nazwaPliku)
{
    ifstream plik(nazwaPliku);
    if (!plik.good())
    {
        ofstream nowyPlik(nazwaPliku);
        nowyPlik.close();
    }
}

int rejestracja(int iloscUzytkownikow, vector<uzytkownik>& uzytkownicy)
{
    string nazwaUzytkownika;
    string hasloUzytkownika;

    bool znaleziono = true;

    while (znaleziono)
    {
        znaleziono = false;

        cout << "Wpisz nazwe uzytkownika:";
        cin >> nazwaUzytkownika;

        ifstream plik("uzytkownicy.txt");
        if (!plik.is_open())
        {
            cout << "Blad odczytu pliku." << endl;
            return iloscUzytkownikow;
        }

        string linia;
        while (getline(plik, linia))
        {
            stringstream ss(linia);
            string pole;
            getline(ss, pole, '|');
            getline(ss, pole, '|');
            if (pole == nazwaUzytkownika)
            {
                cout << "Uzytkownik o podanej nazwie istnieje. Wpisz inna nazwe uzytkownika." << endl;
                znaleziono = true;
                break;
            }
        }
        plik.close();
    }

    cout << "Podaj haslo:";
    cin >> hasloUzytkownika;


    string nazwaPliku = "uzytkownicy.txt";
    int ostatnieId = znajdzOstatnieIdWPliku(nazwaPliku);

    uzytkownik nowyUzytkownik;
    nowyUzytkownik.id = ostatnieId + 1;
    nowyUzytkownik.login = nazwaUzytkownika;
    nowyUzytkownik.haslo = hasloUzytkownika;


    uzytkownicy.push_back(nowyUzytkownik);

    ofstream plik2("uzytkownicy.txt", ios::app);
    if (plik2.is_open())
    {
        plik2 << nowyUzytkownik.id << "|" << nowyUzytkownik.login << "|" << nowyUzytkownik.haslo << endl;
        plik2.close();
    }
    else
    {
        cout << "Blad zapisu do pliku." << endl;
    }

    cout << "Uzytkownik zarejestrowany" << endl;
    Sleep(1000);

    return iloscUzytkownikow + 1;
}

void wczytajDaneZPlikuZalogowanegoUzytkownika(vector<osoba>& osoby, int idZalogowanegoUzytkownika)
{

    ifstream plik("ksiazka.txt");
    if (plik.is_open())
    {
        osoby.clear();
        string linia;

        while (getline(plik, linia))
        {
            osoba adresatZPliku;
            stringstream ss(linia);

            string pole;

            getline(ss, pole, '|');
            if (pole.empty())
            {
                cout << "Błąd w danych" << endl;
                continue;
            }
            adresatZPliku.id = stoi(pole);
            getline(ss, pole, '|');
            adresatZPliku.idUzytkownika = stoi(pole);

            if (adresatZPliku.idUzytkownika == idZalogowanegoUzytkownika)
            {
                getline(ss, adresatZPliku.imie, '|');
                getline(ss, adresatZPliku.nazwisko, '|');
                getline(ss, adresatZPliku.nrTel, '|');
                getline(ss, adresatZPliku.email, '|');
                getline(ss, adresatZPliku.adres);

                osoby.push_back(adresatZPliku);
            }
        }

        plik.close();
    }
    else
    {
        cout << "Błąd otwarcia pliku." << endl;
    }
}

int logowanie(vector<uzytkownik>& uzytkownicy, int iloscUzytkownikow, vector<osoba>& osoby)
{
    string login, haslo;

    cout << "Wpisz login:" << endl;
    cin >> login;

    cout << "Podaj haslo:";
    cin >> haslo;

    ifstream plik("uzytkownicy.txt");
    if (plik.is_open())
    {
        string linia;
        while (getline(plik, linia))
        {
            stringstream ss(linia);
            string pole;
            uzytkownik osoba;
            getline(ss, pole, '|');
            osoba.id = stoi(pole);
            getline(ss, osoba.login, '|');
            getline(ss, osoba.haslo, '|');
            uzytkownicy.push_back(osoba);
        }
        plik.close();

        for (const auto& osoba : uzytkownicy)
        {
            if (osoba.login == login && osoba.haslo == haslo)
            {
                cout << "Poprawnie zalogowano.";
                wczytajDaneZPlikuZalogowanegoUzytkownika(osoby, osoba.id);
                Sleep(1000);
                return osoba.id;
            }
        }

        for (int i = 0; i < 2; i++)
        {
            cout << "Bledny login lub haslo. Pozostaly " << 2 - i << " proby" << endl;

            cout << "Wpisz login:" << endl;
            cin >> login;

            cout << "Podaj haslo:";
            cin >> haslo;

            for (const auto& osoba : uzytkownicy)
            {
                if (osoba.login == login && osoba.haslo == haslo)
                {
                    cout << "Poprawnie zalogowano.";
                    Sleep(1000);
                    wczytajDaneZPlikuZalogowanegoUzytkownika(osoby, osoba.id);
                    return osoba.id;
                }
            }
        }

        cout << "Przekroczono limit prob logowania. Program zostanie zamkniety." << endl;
        exit(0);
    }
    else
    {
        cout << "Blad otwarcia pliku z uzytkownikami." << endl;
        exit(0);
    }
}

void zmienHaslo(vector<uzytkownik>& uzytkownicy, int idZalogowanegoUzytkownika)
{
    string noweHaslo;
    string potwierdzenieHasla;

    do
    {
        cout << "Podaj nowe haslo: ";
        cin >> noweHaslo;

        cout << "Potwierdz nowe haslo: ";
        cin >> potwierdzenieHasla;

        if (noweHaslo != potwierdzenieHasla)
        {
            cout << "Podane hasla roznia sie. Sprobuj ponownie." << endl;
        }
    }

    while (noweHaslo != potwierdzenieHasla);

    auto it = find_if(uzytkownicy.begin(), uzytkownicy.end(), [idZalogowanegoUzytkownika](const uzytkownik& u)
    {
        return u.id == idZalogowanegoUzytkownika;
    });

    if (it != uzytkownicy.end())
    {
        it->haslo = noweHaslo;
        cout << "Haslo zostalo zmienione." << endl;
        system("pause");

        ofstream plik("uzytkownicy.txt");
        if (plik.is_open())
        {
            for (const auto& u : uzytkownicy)
            {
                plik << u.id << "|" << u.login << "|" << u.haslo << endl;
            }
            plik.close();
        }
        else
        {
            cout << "Błąd otwarcia pliku." << endl;
        }
    }

}


void wyloguj(vector<osoba>& osoby, int& idZalogowanegoUzytkownika)
{
    idZalogowanegoUzytkownika = 0;
    osoby.clear();
    cout << "Wylogowano pomyslnie." << endl;
    system("pause");
}

void zapiszAktualizacjeAdresataDoPliku(int IDosobyEdytowanej, const vector<osoba>& osoby)
{
    ifstream plikWejsciowy("ksiazka.txt");
    ofstream plikWyjsciowy("ksiazka_tymczasowa.txt");

    string linia;

    while (getline(plikWejsciowy, linia))
    {
        osoba adresatZPliku;
        stringstream ss(linia);

        string pole;

        getline(ss, pole, '|');
        adresatZPliku.id = stoi(pole);
        getline(ss, pole, '|');
        adresatZPliku.idUzytkownika = stoi(pole);

        if (adresatZPliku.id == IDosobyEdytowanej)
        {

            for (const auto& os : osoby)
            {
                if (os.id == IDosobyEdytowanej)
                {
                    plikWyjsciowy << os.id << "|" << os.idUzytkownika << "|" << os.imie << "|" << os.nazwisko << "|" << os.nrTel << "|" << os.email << "|" << os.adres << endl;
                }
            }
        }
        else
        {
            plikWyjsciowy << linia << endl;
        }

    }

    plikWejsciowy.close();
    plikWyjsciowy.close();

    remove("ksiazka.txt");
    rename("ksiazka_tymczasowa.txt", "ksiazka.txt");
}

void usunAdresatazPliku(int IDosobyUsuwanej, const vector<osoba>& osoby)
{
    ifstream plikWejsciowy("ksiazka.txt");
    ofstream plikWyjsciowy("ksiazka_tymczasowa.txt");

    string linia;

    while (getline(plikWejsciowy, linia))
    {
        osoba adresatZPliku;
        stringstream ss(linia);

        string pole;

        getline(ss, pole, '|');
        adresatZPliku.id = stoi(pole);
        getline(ss, pole, '|');
        adresatZPliku.idUzytkownika = stoi(pole);

        if (adresatZPliku.id != IDosobyUsuwanej)
        {

            plikWyjsciowy << linia << endl;
        }

    }

    plikWejsciowy.close();
    plikWyjsciowy.close();


    remove("ksiazka.txt");
    rename("ksiazka_tymczasowa.txt", "ksiazka.txt");
}

int wpisanieOsoby(vector<osoba>& osoby, int iloscOsob, int idZalogowanegoUzytkownika)
{
    system("cls");

    osoba nowaOsoba;
    cout << "Wpisz imie: ";
    nowaOsoba.imie = wczytajLinie();

    cout << "Wpisz nazwisko: ";
    nowaOsoba.nazwisko = wczytajLinie();

    cout << "Wpisz nr telefonu: ";
    nowaOsoba.nrTel = wczytajLinie();

    cout << "Wpisz adres: ";
    nowaOsoba.adres = wczytajLinie();

    cout << "Wpisz e-mail: ";
    nowaOsoba.email = wczytajLinie();


    string nazwaPliku = "ksiazka.txt";
    int ostatnieId = znajdzOstatnieIdWPliku(nazwaPliku);

    nowaOsoba.id = ostatnieId + 1;
    nowaOsoba.idUzytkownika = idZalogowanegoUzytkownika;

    ofstream plik("ksiazka.txt", ios::app);
    if (plik.is_open())
    {

        plik << nowaOsoba.id << "|" << nowaOsoba.idUzytkownika << "|" << nowaOsoba.imie << "|" << nowaOsoba.nazwisko << "|" << nowaOsoba.nrTel << "|" << nowaOsoba.email << "|" << nowaOsoba.adres << endl;
        plik.close();
    }
    else
    {
        cout << "Błąd otwarcia pliku." << endl;
    }

    osoby.push_back(nowaOsoba);

    cout << "Dodano adresata" << endl;
    system("pause");
    return idZalogowanegoUzytkownika;

}
void wyszukajImie( vector <osoba>& osoby)
{
    system("cls");

    string szukaneImie;
    bool znaleziono = false;

    cout << "Podaj imie: ";
    getline(cin, szukaneImie);

    for (auto& osoba : osoby)
    {
        if (osoba.imie == szukaneImie)
        {
            cout << osoba.id << "|" << osoba.imie << "|" << osoba.nazwisko << "|" << osoba.nrTel << "|" << osoba.email << "|" << osoba.adres<< endl;
            znaleziono = true;
        }
    }

    if (!znaleziono)
        cout << "Nie znaleziono osoby o podanym imieniu." << endl;

    system("pause");
}

void wyszukajNazwisko(vector <osoba>& osoby)
{
    system("cls");

    string szukaneNazwisko;
    bool znaleziono = false;

    cout << "Podaj nazwisko: ";
    getline(cin, szukaneNazwisko);

    for ( auto& osoba : osoby)
    {
        if (osoba.nazwisko == szukaneNazwisko)
        {
            cout << osoba.id << "|" << osoba.imie << "|" << osoba.nazwisko << "|" << osoba.nrTel << "|" << osoba.email << "|" << osoba.adres << endl;
            znaleziono = true;
        }
    }

    if (!znaleziono)
        cout << "Nie znaleziono osoby o podanym nazwisku." << endl;

    system("pause");
}

void wyswietlWszystkieOsoby( vector<osoba>& osoby)
{
    system("cls");

    for (auto& osoba : osoby)
    {
        cout << osoba.id << "|" << osoba.imie << "|" << osoba.nazwisko << "|" << osoba.nrTel << "|" << osoba.email << "|" << osoba.adres << endl;
    }

    system("pause");
}

void usunAdresata( vector <osoba>& osoby, int idZalogowanegoUzytkownika)
{
    bool znaleziono = false;
    int IDosobyUsuwanej;
    char wybor;

    cout << "Podaj ID osoby, ktora chcesz usunac: " << endl;
    cin >> IDosobyUsuwanej;

    cout << "Czy na pewno chcesz usunac dana osobe? (t/n)" << endl;
    cin >> wybor;

    if (wybor == 't')
    {
        for (auto it = osoby.begin(); it != osoby.end(); ++it)
        {
            if (it -> id == IDosobyUsuwanej)
            {
                osoby.erase(it);
                znaleziono = true;
                usunAdresatazPliku(IDosobyUsuwanej,osoby);
                break;
            }
        }

        if (!znaleziono)
        {
            cout << "Nie znaleziono osoby o podanym ID." << endl;
            return;
        }


        cout << "Usunieto adresata o ID: " << IDosobyUsuwanej << " z pliku." << endl;
    }
    system("pause");

}

void edytujAdresata (vector <osoba>& osoby, int idZalogowanegoUzytkownika)
{
    system("cls");

    int IDosobyEdytowanej;
    cout << "Podaj ID osoby, ktora chcesz edytowac: ";
    cin >> IDosobyEdytowanej;

    bool znaleziono = false;

    for (auto& osoba : osoby)
    {
        if (osoba.id == IDosobyEdytowanej)
        {
            cout << "Aktualne dane osoby o ID " << IDosobyEdytowanej << ":" << endl;
            cout << osoba.id << "|" << osoba.imie << "|" << osoba.nazwisko << "|" << osoba.nrTel << "|" << osoba.email << "|" << osoba.adres << endl;

            int wybor;
            cout << "Wybierz co chcesz zmienic:" << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Nr telefonu" << endl;
            cout << "4. Adres" << endl;
            cout << "5. E-mail" << endl;
            cout << "Wybierz opcje: ";
            cin >> wybor;

            switch (wybor)
            {
            case 1:
                cout << "Wpisz nowe imie: ";
                cin >> osoba.imie;
                break;
            case 2:
                cout << "Wpisz nowe nazwisko: ";
                cin >> osoba.nazwisko;
                break;
            case 3:
                cout << "Wpisz nowy nr telefonu: ";
                cin >> osoba.nrTel;
                break;
            case 4:
                cout << "Wpisz nowy adres: ";
                cin >> osoba.adres;
                break;
            case 5:
                cout << "Wpisz nowy e-mail: ";
                cin >> osoba.email;
                break;
            default:
                cout << "Niepoprawny wybor." << endl;
                break;
            }

            znaleziono = true;
            break;
        }
    }

    if (!znaleziono)
    {
        cout << "Nie znaleziono osoby o podanym ID." << endl;
        return;
    }

    zapiszAktualizacjeAdresataDoPliku (IDosobyEdytowanej, osoby );

    {

        cout << "Zaktualizowano dane adresata o ID: " << IDosobyEdytowanej << endl;

        system("pause");
    }
}



int main()
{
    int wybor;
    int iloscOsob = 0;
    int iloscUzytkownikow = 0;
    int idZalogowanegoUzytkownika = 0;

    utworzPlikJesliNieIstnieje("uzytkownicy.txt");
    utworzPlikJesliNieIstnieje("ksiazka.txt");

    vector <uzytkownik> uzytkownicy;
    vector <osoba> osoby;

    while (true)
    {
        if (idZalogowanegoUzytkownika == 0)
        {
            system ("cls");

            cout << " >>>>>> MENU <<<<<<" <<endl;
            cout << "1.Rejestracja" << endl;
            cout << "2.Logowanie" << endl;
            cout << "9. Zakoncz program" <<endl;

            cin >> wybor;

            if ( wybor == 1 )
            {
                iloscUzytkownikow = rejestracja( iloscUzytkownikow,  uzytkownicy);
            }

            else if ( wybor == 2 )
            {

                idZalogowanegoUzytkownika = logowanie ( uzytkownicy, iloscUzytkownikow, osoby);

            }
            else if ( wybor == 9 )
            {
                exit (0);
            }
        }


        else
        {
            system("cls");
            cout << " >>>>>> MENU <<<<<< " << endl;
            cout << "1.Dodaj osobe" << endl;
            cout << "2.Wyszukaj po imieniu" << endl;
            cout << "3.Wyszukaj po nazwisku" << endl;
            cout << "4.Wyswietl wszystkie osoby" << endl;
            cout << "5.Usun adresata" << endl;
            cout << "6.Edytuj adresata" << endl;
            cout << "7.Wyloguj" << endl;
            cout << "8.Zmien haslo" << endl;
            cout << "9.Zakoncz program" << endl;

            cout << "Dokonaj wyboru: ";
            cin >> wybor;
            cin.ignore();

            switch (wybor)
            {
            case 1:
                wpisanieOsoby(osoby, iloscOsob, idZalogowanegoUzytkownika);
                break;

            case 2:
                wyszukajImie(osoby);
                break;

            case 3:
                wyszukajNazwisko(osoby);
                break;

            case 4:
                wyswietlWszystkieOsoby(osoby);
                break;

            case 5:
                usunAdresata(osoby, idZalogowanegoUzytkownika);
                break;

            case 6:
                edytujAdresata(osoby, idZalogowanegoUzytkownika);
                break;

            case 7:
                wyloguj(osoby, idZalogowanegoUzytkownika);
                break;

            case 8:
                zmienHaslo(uzytkownicy, idZalogowanegoUzytkownika);
                break;

            case 9:
                cout << "Koniec programu!" << endl;
                exit(0);
                break;
            }
        }

    }

    return 0;
}



