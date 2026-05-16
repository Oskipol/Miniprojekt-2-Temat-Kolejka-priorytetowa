#include "Tablica.hpp"

TablicaPosortowana::TablicaPosortowana() : rozmiar(0), pojemnosc(1) {
    dane = new Element[pojemnosc];
}

TablicaPosortowana::~TablicaPosortowana() {
    delete[] dane;
}

// Podwaja pojemnosc i kopiuje dane do nowej tablicy
void TablicaPosortowana::rozszerz() {
    pojemnosc *= 2;
    Element* nowe = new Element[pojemnosc];
    for (int i = 0; i < rozmiar; i++) nowe[i] = dane[i];
    delete[] dane;
    dane = nowe;
}

int TablicaPosortowana::size() {
    return rozmiar;
}

// Wstawia element w odpowiednie miejsce zachowujac porzadek rosnacy
// dla równych priorytetow FIFO
void TablicaPosortowana::wstaw(int wartosc, int priorytet) {
    if (rozmiar == pojemnosc) rozszerz();

    // Szukamy miejsca od konca pierwsza pozycja gdzie poprzedni ma mniejszy lub rowny priorytet
    int idx = rozmiar;
    while (idx > 0 && dane[idx - 1].priorytet > priorytet) idx--;

    // Przesun elementy w prawo zeby zrobic miejsce na pozycji idx
    for (int i = rozmiar; i > idx; i--) dane[i] = dane[i - 1];
    dane[idx] = {wartosc, priorytet};
    rozmiar++;
}

// Usuwa i zwraca element o max priorytecie
int TablicaPosortowana::usunMax() {
    if (rozmiar == 0) return -1;
    rozmiar--;
    return dane[rozmiar].wartosc;
}

// Zwraca max bez usuwania
int TablicaPosortowana::znajdzMax() {
    if (rozmiar == 0) return -1;
    return dane[rozmiar - 1].wartosc;
}

// Zmienia priorytet pierwszego wystapienia elementu o danej wartosci, idk czy o to chodzilo
void TablicaPosortowana::modyfikuj(int wartosc, int nowyPriorytet) {
    for (int i = 0; i < rozmiar; i++) {
        if (dane[i].wartosc == wartosc) {
            // Usun element przesuwajac pozostale w lewo
            for (int j = i; j < rozmiar - 1; j++) dane[j] = dane[j + 1];
            rozmiar--;
            // Wstaw ponownie z nowym priorytetem na wlasciwe miejsce
            wstaw(wartosc, nowyPriorytet);
            return;
        }
    }
}

// Wyswietla do 10 elementow w kolejnosci od max
void TablicaPosortowana::wyswietl() {
    if (rozmiar == 0) { cout << "(pusta)\n"; return; }
    int lim = rozmiar < 10 ? rozmiar : 10;
    for (int i = rozmiar - 1; i >= rozmiar - lim; i--)
        cout << "[war=" << dane[i].wartosc << " pri=" << dane[i].priorytet << "] ";
    if (rozmiar > 10) cout << "...";
    cout << " [rozmiar=" << rozmiar << "]\n";
}

// Czysci stare dane i wypelnia strukture nowymi
void TablicaPosortowana::wypelnij(const int* wartosci, const int* priorytety, int n) {
    delete[] dane;
    rozmiar = 0; pojemnosc = 1;
    dane = new Element[pojemnosc];
    for (int i = 0; i < n; i++) wstaw(wartosci[i], priorytety[i]);
}