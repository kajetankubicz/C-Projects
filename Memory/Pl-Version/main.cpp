#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;
#define SZEROKOSC_KARTY 5
#define WYSOKOSC_KARTY 5

struct KARTA{
    char* wartosc = NULL; // Dynamicznie alokowany ciag znakow na karcie
    bool czy_otwarta=false, // Czy ma byc wyswietlona wartosc karty przy rysowaniu
         czy_zebrana=false; // Czy karta zostala juz zabrana z planszy
};

KARTA* karty=NULL; // Globalny wskaznik na jednowymiarowa tablice kart
int szer, wys, punkty_graczy[2], ktory_gracz; // Wymiary planszy
bool koniec;

bool unikalny_ciag_znakow(char** obrazki, int index){ // wskazuje na miejsce w pamięci gdzie leży wskaźnik który pokazuje na chara
    for (int i = 0; i < index; i++) { // Dla wszystkich ciagow o mniejszych indeksach
        if (strcmp(obrazki[i], obrazki[index]) == 0) // Sprawdz czy ciagi znakow sa takie same
            return true; // Ciag znakow sie powtarza
    }
    return false; // Ciag znakow jest unikalny
}

void tasowanie(char** obrazki, int rozmiar){ // Przetasuj ciagi znakow w tablicy
    for (int i = 0; i < rozmiar - 1; i++) { // Dla wszystkich ciagow w tablicy
        int j = (rand() % (rozmiar - i)) + i; // Wylosuj indeks ciagu mniejszy lub równy "i"
        char* t = obrazki[j]; // Zamien ciagi znakow ze soba
        obrazki[j] = obrazki[i]; // Zamien ciagi znakow ze soba
        obrazki[i] = t; // Zamien ciagi znakow ze soba
    }
}

int  pote(int a, int b){ // Funkcja obliczajaca wartosc potegi a^b dla nieujemnych wartosci b
    if (b == 0) return 1; // Dla wykladnika rownego 0, zwroc 1
    for (int i = 0; i < b - 1; i++) // W przeciwnym przypadku pomnoz a przez siebie b razy
        a = a * a;
    return a;
}

void rysowanie(){ // Narysuj plansze na ekranie
    system("CLS");
    for (int i = 0; i < wys; i++) { // Dla wszystkich rzedow kart
        cout << "-";  // Narysuj gorny brzeg rzedu kart
        for (int j = 0; j < szer; j++) { // Dla wszystkich kolumn kart
            for (int k = 0; k < SZEROKOSC_KARTY; k++) { // Dla kazdej karty w rzedzie
                cout << "-";
            }
            cout << "-";
        }
        cout << "\n";

        for (int k = 0; k < WYSOKOSC_KARTY; k++) {// Narysuj srodkowa czesc rzedu kart // Dla kazdego poziomu karty w danym rzedzie
            cout << "|"; //lewa linijka dla ka¿dej kolumny = praw dla poprzedniej
            for (int j = 0; j < szer; j++) { // Dla wszystkich kolumn kart
                for (int l = 0; l < SZEROKOSC_KARTY; l++) { // Dla kazdej karty w rzedzie
                    if (karty[i * szer + j].czy_zebrana) { // Jesli karta jest zebrana narysuj pusta przestrzen
                            //jezeli karta (i *n + j to indeks karty od 0 do m*n w tablicy jednowymiarowej kart)
                        cout << " ";
                        continue;
                    };

                    if (k == 0 || k == WYSOKOSC_KARTY - 1 || l == 0 || l == SZEROKOSC_KARTY - 1) { // Jesli rysowany jest brzeg karty(granica)
                        if (karty[i * szer + j].czy_otwarta) { // Jesli jakrta jest odkryta
                            cout << "/";
                        }
                        else // W przeciwnym przypadku
                            cout << "#";
                    }
                    else { // Jesli rysowane jest wnetrze(srodek) karty
                        if (!karty[i * szer + j].czy_otwarta) { // Jesli karta jest zakryta
                            int a = i * szer + j + 1; // Znajdz numer karty na podstawie pozycji w rzedzie i kolumnie (numerowane kolejno rzedami od lewej do prawej od 0+1)
                            int x = (a / pote(10, (WYSOKOSC_KARTY - 1) - k - 1)) % 10; // Pobierz odpowiednia cyfre liczby w systemie dziesietnym czyli jezeli k == wysokosc_karty - 1 to wypisz jednosci, k == wysokosc_karty - 2 to wypisz dziesiatki itp
                            if ((x != 0 || ((WYSOKOSC_KARTY - 1) - k == 1 && a >= 10) || (WYSOKOSC_KARTY - 2) - k == 1 && a>=100) && l == SZEROKOSC_KARTY / 2) {// Jesli cyfra powinna byc narysowana na srodku karty w razie szerszej karty
                                cout << x; // Wypisz odpowiednia cyfre
                            }
                            else { // W przeciwnym przypadku
                                cout << " "; // Narysuj pusta przestrzen
                            }
                        }
                        else { // Jesli karta jest odkryta
                            if (WYSOKOSC_KARTY - k - 2 < 3 && l == SZEROKOSC_KARTY / 2) // W zaleznosci od pozycji na srodku karty
                                cout << karty[i * szer + j].wartosc[WYSOKOSC_KARTY - k - 2]; // Narysuj odpowiedni znak z ciagu
                            else
                                cout << " "; // Lub narysuj pusta przestrzen
                        }
                    };
                }
                cout << "|"; // zamkniecie prawej linijki
            }
            cout << "\n";
        }
    }
    // Narysuj brzeg dolnych kart
    for (int j = 0; j < szer; j++) {
        for (int k = 0; k < SZEROKOSC_KARTY + 1; k++) {
            cout << "-";
        }
    }
    cout << endl << endl;
    cout << "ZEBRANE KARTY: \nGRACZ 1: " << punkty_graczy[0] << "\nGRACZ 2: " << punkty_graczy[1] << "\n\n"; // Wyswietl punktacje obydwu graczy
    cout << "Aktualny gracz " << ktory_gracz + 1 << "\n\n"; // Wyswietl informacje o numerze aktywnego gracza
}

void wygenerowywanie_obrazkow(){ // Wygeneruj losowe ciagi znakow i przypisz je do kart, tak aby dokladnie 2 karty mialy ten sam ciag znakow
    char** obrazki = new char* [wys * szer]; // Zaalokuj pamiec na ciagi znakow dla wszystkich kart
    for (int j = 0; j < wys * szer / 2;) { // Dla polowy kart wygeneruj unikalny ciag znakow
        obrazki[j] = new char[4]; // Zaalokuj pamiec na ciag znakow (mozna bylo uzyc tablicy statycznej w strukturze KARTA)
        for (int i = 0; i < 3; i++) { // Wylosuj 3 znaki
            int numer = (rand() % 33) + 222; // Wybierz losowa liczbe z zakresu od 1 do 1000
            obrazki[j][i] = numer; // Rzutuj liczbe na znak
        }
        obrazki[j][3] = '\0'; // Zakoncz ciag znakow NULLem
        if (!unikalny_ciag_znakow(obrazki, j)) j++; // Jesli ciag znakow juz wczesniej wystapil, sprobuj wygenerowac jeszcze raz
    }
    memcpy(obrazki + wys * szer / 2, obrazki, sizeof(char*) * wys * szer / 2); // Skopiuj ciagi znakow dla drugiej polowki talii kart
    int rozmiar = wys * szer;
    tasowanie(obrazki, rozmiar); // Losowo zamien kolejnosc ciagow znakow
    for (int i = 0; i < wys * szer; i++) { // Dla wszystkich kart
        if (karty[i].wartosc != NULL) // Jesli karta ma przypisany stary ciag znakow
            delete karty[i].wartosc; // Usun ten ciag z pamieci
        karty[i].wartosc = obrazki[i]; // Przypisz nowy ciag znakow do karty
    }
    delete[] obrazki; // Zwolnij pamiec na tablice ciagow znakow (ciagi znakow nadal sa w pamieci, zostaly usuniete tylko wskazniki)
}

void wymiary(){
    while (true) {
        system("CLS"); // Wyczysc okno terminala
        char X[500]; // Bufory wejciowe
        cout << "Witaj w grze o nazwie 'MEMORY'!!! \nPierwsze co musisz zrobic to wybrac wymiary swojej tablicy(tzn ile ma byc kart w wierszu i kolumnie) \nTo sa zasady ktorych musisz sie trzymac: " << endl << endl;
        cout << "Wymiary nie moga byc \n- Nieparzyste(tzn chociaz jedna liczba musi byc parzysta)  \n- Rowne 0 \n- O wymiarze 2x1, 1x2 lub 2x2 \n- Mniejsze od 0 \nLiczba kolumn nie moze byc wieksz niz 35" << endl << endl;
        cout << "Wymiary: \n Kolumn = "; cin >> X; // Pobierz szerokosc planszy 
        szer = atoi(X); // Przekonwertuj ciag znakow na liczbe calkowita
        cout << " Wierszy = "; cin >> X; // Pobierz wysokosc planszy (blednie nazwana zmienna)
        wys = atoi(X); // Przekonwertuj ciag znakow na liczbe calkowita
        if ((szer <= 2 && wys <= 2) || (szer>=36) || (szer>=36 && wys>=29)) { // Sprawdz czy wymiary planszy nie sa zbyt male i zbyt duze
            cout << "\nPodaj poprawne wymiary.\n";
            cout << "\nWcisnij enter by sprobowac ponownie.\n";
            cin.get(); cin.get(); // Poczekaj na znak nowej linii
            continue;
        }
        if (szer % 2 != 0 && wys % 2 != 0) { // Sprawdz czy liczba kart jest parzysta
            cout << "\nJeden wymiar musi byc parzysty.\n";
            cout << "\nWcisnij enter by sprobowac ponownie.\n";
            cin.get(); cin.get(); // Poczekaj na znak nowej linii
            continue;
        }
        break;
    }
    ktory_gracz = 0; // Zacznij rozgrywke od gracza pierwszego
    delete[] karty; // Usun karty z poprzeniej gry
    karty = new KARTA[wys * szer]; // Zarezerwuj pamiec na nowe karty
    punkty_graczy[0] = punkty_graczy[1] = 0; // Wyzeruj liczbe punktow obydwu graczy
    wygenerowywanie_obrazkow(); // Wygeneruj unikalne ciagi znakow dla kart
    rysowanie(); // Narysuj plansze
}

void wybor(){
    char ktora[500]; // Bufor wejsciowy
    int numer; // Numer wybranej karty
    rysowanie(); // Narysuj plansze

    cout << "Podejrzyj pierwsza karte o numerze: "; cin >> ktora; // Pobierz numer pierwszej karty
    numer = atoi(ktora); // Przekonwertuj numer karty z ciagu znakow na liczbe calkowita
    if (numer <= 0 || numer > wys * szer) { // Jesli nie istnieje karta o danym numerze
        cout << "Wprowadz poprawny numer karty.\n\n";
        return; // Wroc do menu glownego
    }
    KARTA* card1 = &karty[numer - 1]; // Pobierz adres karty o podanym numerze
    if ((*card1).czy_otwarta || (*card1).czy_zebrana) { // Jesli karty nie ma juz na planszy albo jest aktualnie odkryta
        cout << "Ta karta zostala juz wybrana.\n\n";
        (*card1).czy_otwarta = false; // Zakryj karte, jesli byla odkryta (np. w poprzednim ruchu)
        return; // Wroc do menu glownego
    }
    (*card1).czy_otwarta = true; // Odkryj wybrana karte
    rysowanie(); // Narysuj plansze

    cout << "Podejrzyj druga karte o numerze: "; cin >> ktora; // Pobierz numer drugiej karty
    numer = atoi(ktora); // Przekonwertuj numer karty z ciagu znakow na liczbe calkowita
    if (numer <= 0 || numer > wys * szer) { // Jesli nie istnieje karta o danym numerze
        cout << "Wprowadz poprawny numer karty.\n\n";
        (*card1).czy_otwarta = false; // Zakryj pierwsza karte
        return; // Wroc do menu glownego
    }
    KARTA* card2 = &karty[numer - 1]; // Pobierz adres karty o podanym numerze
    if ( (*card2).czy_otwarta || (*card2).czy_zebrana ) { // Jesli karty nie ma juz na planszy albo jest aktualnie odkryta
        cout << "Ta karta zostala juz wybrana.\n\n";
        (*card1).czy_otwarta = false; // Zakryj obie karty
        return;
    }
    (*card2).czy_otwarta = true; // Odkryj druga karte
    rysowanie(); // Narysuj plansze

    int wartosci = strcmp((*card1).wartosc, (*card2).wartosc); // Sprawdz, czy karty maja taki sam ciag znakow
    if (wartosci == 0) { // Jesli ciag znakow jest identyczny
        cout << "Karty zgadzaja sie! Otrzymujesz je.\n";
        (*card1).czy_zebrana = true; // Zbierz obie karty
        (*card2).czy_zebrana = true;
        punkty_graczy[ktory_gracz] += 2; // Przyznaj aktywnemu graczowi 2 punkty
    }
    else { // Jesli ciag znakow jest rozny
        cout << "Karty nie zgadzaja sie.\n";
    }

    cout << "\nWcisnij dowolny przycisk w celu zmiany gracza.\n";
    ktory_gracz = (ktory_gracz + 1) % 2; // Zmien numer aktywnego gracza
    (*card1).czy_otwarta = false; // Zakryj obie karty
    (*card2).czy_otwarta = false;
    cin.get(); cin.get(); // Poczekaj na znak nowej linii
    rysowanie(); // Narysuj plansze
}

int  main(){
    koniec = false;
    srand(time(NULL)); // Inicjalizacja generatora liczb pseudolosowych aktualnym czasem systemu - potrzebne do funkcji rand()
    wymiary(); // Pobierz od uzytkownika wymiary planszy
    while (!koniec) {
        char znak; // Bufor na dane wejsciowe
        if (punkty_graczy[0] + punkty_graczy[1] != wys * szer) { // Jesli nie wszystkie karty zostaly zabrane
            cout << "S - Zacznij kolejke; R - Restartuj; Q - Wyjdz\n";
            cin >> znak; // Pobierz wybor opcji od uzytkownika
            switch (znak)
            {
            case 'q': { // Zakoncz gre i wyjdz z glownej petli programu
                koniec = true; break;
            }
            case 's': { // Wykonaj ruch w grze
                wybor(); break;
            }
            case 'r': { // Zacznij gre od nowa (wybierz wymiary nowej planszy)
                wymiary(); break;
            }
            default: { // Narysuj plansze
                rysowanie(); break;
            }
            }
        }
        else { // Wszystkie karty zostaly zabrane (wyswietlenie wynikow)
            if (punkty_graczy[0] == punkty_graczy[1]) {
                cout << "REMIS!";
            }
            else {
                cout << "WYGRANA GRACZA ";
                if (punkty_graczy[0] > punkty_graczy[1])
                    cout << "nr. 1 !!! Gratki!!!";
                else
                    cout << "nr. 2 !!! Gratki!!!";
            }
            cout << "\n\nR - Restartuj; Q - Wyjdz\n";
            cin >> znak; // Pobierz opcje od gracza
            switch (znak)
            {
            case 'q': { // Zakoncz gre i wyjdz z glownej petli programu
                koniec = true; break;
            }
            case 'r': { // Zacznij gre od nowa (wybierz wymiary nowej planszy)
                wymiary(); break;
            }
            }
            rysowanie(); // Narysuj plansze
        }
    }
    delete[] karty;
}