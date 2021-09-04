#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;
class Slowo{  // Klasa przechowujaca slowo i jego tlumaczenie na angielski
public: //Dostep publiczny umozliwia uzyskiwanie dostepu do dowolnej zmiennej i funkcji z dowolnego miejsca w kodzie(z zachowana logika)
    Slowo(const string &wartosc, const string &tlumaczenie) : slowo(wartosc), tlumaczenie(tlumaczenie) {} // Definicja konstruktora przyjmujacy slowo i tlumaczenie slowa
    friend ostream &operator<<(ostream &os, const Slowo &slowo){ // Operator wyjsciowy zdef. przez uzytkownika pobiera swój strumien wyjsciowy(przez referencje) i
        os << "slowo: " << slowo.slowo << " Tlumaczenie: " << slowo.tlumaczenie;// zwraca go w wyniku do wypisywania obiektu na terminalu(w ustalonym formacie os)
        return os;
        }
        string slowo, tlumaczenie; // Slowo odpowiednio w jezyku polskim i angielskim
    };
vector<string> zaladujPlik(string plik);
vector<Slowo> zaladujOba(string slowka, string tlumaczenie);
void nauka(vector<Slowo> slowa);
void test(vector<Slowo> slowa);
void menu(vector<Slowo> slowa);
int main(){
    vector<Slowo> slowa=zaladujOba("slowka.txt", "tlumaczenie.txt"); // Wczytaj pary slowo-tlumaczenie z dwoch plikow
    srand(time(NULL)); // Zainicjalizuj generator liczb pseudolosowych obecnym czasem
    menu(slowa); // Wyswietl menu glowne
    return 0;
}
vector<string> zaladujPlik(string plik){ // Wczytaj do wektora frazy odczytane jako kolejne linie pliku o podanej nazwie
    ifstream p(plik); // Utworz strumien wejsciowy i otworz plik o podanej nazwie
    string tmp; // Utworz tymczasowa zmienna na wczytana linie z pliku
    vector<string> tekst; // Utworz pusty wektor ciagow znakow (slow / fraz)
    if (p.is_open()){  // Jesli plik zostal prawidlowo otwarty
        while (getline(p, tmp)){  // Wczytuj z pliku kolejne linie do zmiennej "tmp", dopoki istnieja linie w pliku
            tekst.push_back(tmp); // Dodaj wczytana linie do wektora
            }
            p.close(); // Zamknij plik
        }
    return tekst; // Zwroc utworzony wektor slowek
}
vector<Slowo> zaladujOba(string slowka, string tlumaczenie){ // Wczytaj do wektora obiekty klasy "Slowo" odczytane z dwoch plikow(w jednym znajduja sie polskie frazy,w drugim angielskie)
    vector<Slowo> objs; // Utworz pusty wektor obiektow klasy "Slowo"
    vector<string> slowa=zaladujPlik(slowka); // Wczytaj do wektora "slowa" linie z pliku o nazwie podanej w zmiennej "slowka"
    vector<string> translation=zaladujPlik(tlumaczenie); // Wczytaj do wektora "translation" linie z pliku o nazwie podanej w zmiennej "tlumaczenie"
    int unsigned max=slowa.size()>translation.size() ? translation.size() : slowa.size(); // Znajdz krotsza dlugosc wektora (mniejsza liczbe elementow) i zapisz do zmiennej max
    for(int unsigned i=0; i<max; i++){ // Dla wszystkich elementow krotszego wektora
        objs.push_back(Slowo(slowa.at(i),translation.at(i))); // Utworz pary slowo i jego tlumaczenie i dodaj do wektora "objs"
    }
    return objs; // Zwroc wektor obiektow klasy "Slowo" (baza slowek i tlumaczen)
}
void nauka(vector<Slowo> slowa){ // Rozpocznij tryb nauki slowek z podanej bazy
    system("CLS"); // Wyczysc okno terminala
    vector<Slowo> tmp; // Utworz pusty wektor par slowo-tlumaczenie
    for(int unsigned i=0; i<slowa.size(); i++){
        tmp.push_back(slowa.at(i)); // Skopiuj do wektora wszystkie pary z bazy
    }
    cin.ignore(); // Wyczysc wejscie standardowe (usun oczekujace znaki, np '\n' pozostaly po poprzednim uzyciu "cin")
    while(tmp.size() > 0){  // Dopoki wektor "tmp" jest niepusty (sa jeszcze slowa do nauczenia)
        int id=rand()%tmp.size(); // Wylosuj indeks wektora "tmp" i zapisz do "id"
        cout<<"Wpisz tlumaczenie slowa:"<<endl<<endl;
        cout<<"\t'"<<tmp.at(id).slowo<<"': "<<endl<<endl; // Wyswietl slowo po polsku
        cout<<"Tlumaczenie: ";
        string str; // Utworz zmienna tymczasowa na podany przez uzytkownika tekst
        getline(cin,str); // Pobierz z terminala ciag znakow do znaku nowej linii (entera)
        system("cls"); // Wyczysc okno terminala
        if(str==tmp.at(id).tlumaczenie){  // Jesli wprowadzony przez uzytkownika tekst jest tlumaczeniem
            cout << "\n\tPoprawna odpowiedz. Wdus enter zeby przejsc dalej" << endl << endl;
            cin.get(); // Pobierz pojedynczy znak z terminala
            system("CLS"); // Wyczysc okno terminala
            tmp.erase(tmp.begin()+id); // Usun z wektora przetlumaczone slowo
        }
        else{ //Jesli tlumaczenie jest zle
            cout<<"\n\tZle, poprawna odpowiedz to: "<<tmp.at(id).tlumaczenie<<endl<<endl; // Wyswietl poprawne tlumaczenie
            cout<<"Miej na uwadze ze pytanie o to slowko pojawi sie jeszcze w tym tescie \n"<<endl;
            cout<<"Wdus enter zeby przejsc dalej"<<endl;
            cin.get(); // Pobierz pojedynczy znak z terminala
            system("CLS"); // Wyczysc okno terminala
        }
    }
    cout<<endl;
    cout<<"\t\t\t\t\tKoniec trybu nauki"<<endl<<endl<<endl;
    cout<<"Wcisnij 'm' oraz kliknij enter zeby sprobowac ponownie w jednym z dwoch trybow"<<endl<<endl;
    cout<<"Wcisnij cokolwiek innego niz 'm' oraz kliknij enter aby zeby zakonczyc gre"<<endl<<endl;
    char tryb; // Bufor na znak z klawiatury
    cout<<"Decyzja: ";
    cin>>tryb; // Pobierz znak z klawiatury
    switch(tryb){
    case 'm': // Jesli wprowadzony znak to 'm'
        menu(slowa); // Wyswietl menu z oryginalna baza slowek
        break;
    default: // Jesli wprowadzony znak to 'm'
        system("CLS"); // Wyczysc okno terminala
        cout<<"Dziekuje za skorzystanie z aplikacji. Zapraszam ponownie w przyszlosci by nauczyc sie jeszcze wiekszej ilosci slowek."<<endl;
    }
    // Wyjscie z funkcji powoduje wyjscie z rekurencji i zakonczenie programu
}
void test(vector<Slowo> slowa){ // Rozpocznij test sprawdzajacy znajomosc podanej bazy slowek
    system("CLS"); // Wyczysc okno terminala
    vector<Slowo> tmp; // Utworz pusty wektor par slowo-tlumaczenie
    for(int unsigned i=0; i<slowa.size(); i++){
        tmp.push_back(slowa.at(i)); // Skopiuj do wektora wszystkie pary z bazy
        }
    cin.ignore(); // Wyczysc wejscie standardowe (usun oczekujace znaki, np '\n' pozostaly po poprzednim uzyciu "cin")
    int count=0; // Liczba poprawnie przetlumaczonych slow = 0
    while(tmp.size()>0){  // Dopoki wektor "tmp" jest niepusty (sa jeszcze slowa do przetlumaczenia)
        int id=rand()%tmp.size(); // Wylosuj indeks wektora "tmp" i zapisz do "id"
        cout<<"Wpisz tlumaczenie slowa:"<<endl<<endl;
        cout<<"\t'"<<tmp.at(id).slowo<<"': "<<endl<<endl; // Wyswietl slowo po polsku
        cout<<"Tlumaczenie: ";
        string str; // Utworz zmienna tymczasowa na podany przez uzytkownika tekst
        getline(cin,str); // Pobierz z terminala ciag znakow do znaku nowej linii (entera)
        system("cls"); // Wyczysc okno terminala
        if(str==tmp.at(id).tlumaczenie){  // Jesli wprowadzony przez uzytkownika tekst jest tlumaczeniem
            count++; // Dodaj jeden punkt poprawnych odpowiedzi
            }
        tmp.erase(tmp.begin()+id); // Usun z wektora przetlumaczone slowo
        }
    cout<<"Wyniki testu "<<count<<" / "<<slowa.size()<<endl<<endl; // Wypisz jaka czesc slowek zostala przetlumaczenia
    cout<<"\t\t\t\t\tKoniec trybu testu"<<endl<<endl;
    cout<<"Wcisnij 'm' oraz kliknij enter zeby sprobowac ponownie w jednym z dwoch trybow"<<endl<<endl;
    cout<<"Wcisnij cokolwiek innego niz 'm' oraz kliknij enter aby zeby zakonczyc gre"<<endl<<endl;
    char tryb; // Bufor na znak z klawiatury
    cout<<"Decyzja: ";
    cin>>tryb; // Pobierz znak z klawiatury
    switch(tryb){
    case 'm': // Jesli wprowadzony znak to 'm'
        menu(slowa); // Wyswietl menu z oryginalna baza slowek
        break;
    default:
        system("CLS"); // Wyczysc okno terminala
        cout<<"Dziekuje za skorzystanie z aplikacji. Zapraszam ponownie w przyszlosci by nauczyc sie jeszcze wiekszej ilosci slowek."<<endl;
    }
}
void menu(vector<Slowo> slowa){ // Wyswietl menu glowne programu (zmienna "slowa" potrzebna jest do funkcji "nauka" oraz "test")
    system("CLS"); // Wyczysc okno terminala
    cout<<"   !!! Witaj w aplikacji dzieki ktorej nauczysz sie slowek po angielsku !!! \n"<<endl;
    cout<<"\t\t !!!!!! Licza sie wielkosci liter !!!!!! \n"<<endl;
    cout<<"\t\t !!!!!! Nie uzywaj znakow interpunkcyjnych !!!!!! \n"<<endl;
    cout<<"Wstukaj numer: "<<endl;
    cout<<" '1' by przejsc w tryb nauki"<<endl;
    cout<<" '2' by przejsc w tryb testu"<<endl;
    cout<<"Co wybierasz: ";
    char tryb; // Bufor na znak z klawiatury
    cin>>tryb; // Pobierz znak z klawiatury
    switch(tryb){
    case '1': // Jesli wprowadzony znak to '1'
        nauka(slowa); // Przejdz do trybu nauki
        break;
    case '2': // Jesli wprowadzony znak to '2'
        test(slowa); // Przejdz do trybu test
        break;
    default: // Jesli wprowadzony znak to 'm'
        cout<<endl;
        cout << "Bledne dane! Wdus enter zeby przejsc dalej" << endl;
        cin.get();
        cin.get();
        menu(slowa); // Wyswietl menu z oryginalna baza slowek
    }
}