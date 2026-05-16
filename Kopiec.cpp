#include "Kopiec.hpp"

Kopiec::Kopiec() : rozmiar(0), pojemnosc(1) {
    dane = new Element[pojemnosc];
}

Kopiec::~Kopiec() {
    delete[] dane;
}

// Podwaja pojemnosc i kopiuje dane do nowej tablicy
void Kopiec::rozszerz() {
    pojemnosc *= 2;
    Element* nowe = new Element[pojemnosc];
    for (int i = 0; i < rozmiar; i++) nowe[i] = dane[i];
    delete[] dane;
    dane = nowe;
}

// Zamienia miejscami elementy o indeksach a i b
void Kopiec::zamien(int a, int b) {
    Element tmp = dane[a];
    dane[a] = dane[b];
    dane[b] = tmp;
}

// Przesuwa element w gore dopoki ma wiekszy priorytet niz rodzic
void Kopiec::wGore(int i) {
    while (i > 0) {
        int rodzic = (i - 1) / 2;
        if (dane[rodzic].priorytet < dane[i].priorytet) {
            zamien(rodzic, i);
            i = rodzic;
        } else break;
    }
}

// Przesuwa element w dol dopoki ma mniejszy priorytet niz dzieci
void Kopiec::wDol(int i) {
    while (true) {
        int max = i;
        int l = 2 * i + 1; // lewe dziecko
        int p = 2 * i + 2; // prawe dziecko
        if (l < rozmiar && dane[l].priorytet > dane[max].priorytet) max = l;
        if (p < rozmiar && dane[p].priorytet > dane[max].priorytet) max = p;
        if (max == i) break; // element jest na swoim miejscu
        zamien(i, max);
        i = max;
    }
}

int Kopiec::size() {
    return rozmiar;
}

// Dodaje element i przywraca wlasnosc kopca
void Kopiec::wstaw(int wartosc, int priorytet) {
    if (rozmiar == pojemnosc) rozszerz();
    dane[rozmiar] = {wartosc, priorytet};
    wGore(rozmiar);
    rozmiar++;
}

// Usuwa i zwraca element o max priorytecie
int Kopiec::usunMax() {
    if (rozmiar == 0) return -1;
    int wynik = dane[0].wartosc;
    dane[0] = dane[rozmiar - 1]; // wstaw ostatni element na szczyt
    rozmiar--;
    if (rozmiar > 0) wDol(0);    // przywroc wlasnosc kopca
    return wynik;
}

// Zwraca max bez usuwania
int Kopiec::znajdzMax() {
    if (rozmiar == 0) return -1;
    return dane[0].wartosc;
}

// Zmienia priorytet pierwszego wystapienia elementu o danej wartosci
void Kopiec::modyfikuj(int wartosc, int nowyPriorytet) {
    for (int i = 0; i < rozmiar; i++) {
        if (dane[i].wartosc == wartosc) {
            int stary = dane[i].priorytet;
            dane[i].priorytet = nowyPriorytet;
            if (nowyPriorytet > stary) wGore(i); //idz w gore
            else wDol(i);  //idz w dol
            return;
        }
    }
}

// Wyswietla do 10 elementow w kolejnosci od max
void Kopiec::wyswietl() {
    if (rozmiar == 0) { cout << "(pusta)\n"; return; }
    int lim = rozmiar < 10 ? rozmiar : 10;
    for (int i = 0; i < lim; i++)
        cout << "[war=" << dane[i].wartosc << " pri=" << dane[i].priorytet << "] ";
    if (rozmiar > 10) cout << "...";
    cout << " [rozmiar=" << rozmiar << "]\n";
}

// Czysci stare dane i wypelnia kopiec nowymi
void Kopiec::wypelnij(const int* wartosci, const int* priorytety, int n) {
    delete[] dane;
    rozmiar = 0; pojemnosc = 1;
    dane = new Element[pojemnosc];
    for (int i = 0; i < n; i++) wstaw(wartosci[i], priorytety[i]);
}