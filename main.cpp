#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "KolejkaBaza.hpp"
#include "Kopiec.hpp"
#include "Tablica.hpp"
using namespace std;


mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int losujWartosc() {
    // losowa 4-bajtowa liczba calkowita
    return (int)rng();
}

int losujPriorytet(int maxPriorytet) {
    uniform_int_distribution<int> dist(0, maxPriorytet);
    return dist(rng);
}

int losujIndeks(int n) {
    uniform_int_distribution<int> dist(0, n - 1);
    return dist(rng);
}

long long nanos() {
    return chrono::duration_cast<chrono::nanoseconds>(
        chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

void benchmark() {
    const int ROZMIARY[] = {5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000};
    const int ILE  = 8;
    const int OPS  = 1000; // liczba operacji na pomiar
    const int POWT = 5;    // powtorzenia dla usrednienia

    // Zakres priorytetow — kilkukrotnie wiekszy niz max rozmiar struktury
    const int MAX_PRIORYTET = 100000 * 5;

    const char* nazwy[] = {"Kopiec", "Tablica"};

    cout << "\nN\t";
    for (int s = 0; s < 2; s++)
        cout << nazwy[s] << "_insert\t"
             << nazwy[s] << "_findMax\t"
             << nazwy[s] << "_modifyKey\t"
             << nazwy[s] << "_extractMax\t";
    cout << "\n";

    for (int s = 0; s < ILE; s++) {
        int N = ROZMIARY[s];
        KolejkaBaza* str[2] = { new Kopiec(), new TablicaPosortowana() };
        long long czasy[2][4] = {}; // 2 struktury x 4 operacje

        for (int r = 0; r < POWT; r++) {

            // Generuje wspolne dane
            int* wartosci   = new int[N];
            int* priorytety = new int[N];
            for (int i = 0; i < N; i++) {
                wartosci[i]   = losujWartosc();
                priorytety[i] = losujPriorytet(MAX_PRIORYTET);
            }

            // Wypelnij obie struktury tymi samymi danymi
            for (int i = 0; i < 2; i++) str[i]->wypelnij(wartosci, priorytety, N);

            // Dla modifyKey potrzebujemy wartosci ktore sa w strukturze
            int* doModify = new int[OPS];
            for (int i = 0; i < OPS; i++)
                doModify[i] = wartosci[losujIndeks(N)];

            // Dla wyszukiwania (findMax) — generujemy liste OPS zapytan
            // (findMax zawsze zwraca ten sam element wiec mierzymy sam koszt operacji)


            long long t0, t1;

            for (int i = 0; i < 2; i++) {

                // insert — OPS nowych elementow
                str[i]->wypelnij(wartosci, priorytety, N);
                t0 = nanos();
                for (int j = 0; j < OPS; j++)
                    str[i]->wstaw(losujWartosc(), losujPriorytet(MAX_PRIORYTET));
                t1 = nanos();
                czasy[i][0] += t1 - t0;

                // findMax — OPS zapytan na tej samej strukturze
                str[i]->wypelnij(wartosci, priorytety, N);
                t0 = nanos();
                for (int j = 0; j < OPS; j++)
                    str[i]->znajdzMax();
                t1 = nanos();
                czasy[i][2] += t1 - t0;

                // modifyKey — zmiana priorytetu OPS losowych elementow
                str[i]->wypelnij(wartosci, priorytety, N);
                t0 = nanos();
                for (int j = 0; j < OPS; j++)
                    str[i]->modyfikuj(doModify[j], losujPriorytet(MAX_PRIORYTET));
                t1 = nanos();
                czasy[i][3] += t1 - t0;
                // extractMax
                str[i]->wypelnij(wartosci, priorytety, N);
                t0 = nanos();
                for (int j = 0; j < OPS; j++)
                    str[i]->usunMax();
                t1 = nanos();
                czasy[i][1] += t1 - t0;
            }

            delete[] doModify;
            delete[] priorytety;
            delete[] wartosci;
        }

        cout << N << "\t";
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 4; j++)
                cout << czasy[i][j] / POWT / OPS << "\t";
        cout << "\n";

        for (int i = 0; i < 2; i++) delete str[i];
    }
}

void podmenu(KolejkaBaza* s, const char* nazwa) {
    const char* opcje[] = {
        "1. Utworz losowo",
        "2. Insert (dodaj element)",
        "3. Extract-Max (usun max)",
        "4. Find-Max (podejrzyj max)",
        "5. Modify-Key (zmien priorytet)",
        "6. Rozmiar",
        "7. Wyswietl",
        "8. Powrot"
    };

    const int MAX_PRIORYTET = 500000;

    int wybor;
    do {
        cout << "\n=== " << nazwa << " ===\n";
        for (const char* o : opcje) cout << o << "\n";
        cout << "Wybor: ";
        cin >> wybor;

        if (wybor == 1) {
            int n; cout << "Ile elementow? "; cin >> n;
            int* w = new int[n];
            int* p = new int[n];
            for (int i = 0; i < n; i++) {
                w[i] = losujWartosc();
                p[i] = losujPriorytet(MAX_PRIORYTET);
            }
            s->wypelnij(w, p, n);
            delete[] w; delete[] p;
            cout << "Utworzono " << n << " elementow.\n";
        }

        else if (wybor == 2) {
            int w, p;
            cout << "Wartosc: "; cin >> w;
            cout << "Priorytet: "; cin >> p;
            s->wstaw(w, p);
            cout << "Dodano.\n";
        }

        else if (wybor == 3) {
            int wynik = s->usunMax();
            if (wynik == -1) cout << "Kolejka pusta.\n";
            else cout << "Usunieto element o wartosci: " << wynik << "\n";
        }

        else if (wybor == 4) {
            int wynik = s->znajdzMax();
            if (wynik == -1) cout << "Kolejka pusta.\n";
            else cout << "Element o max priorytecie: " << wynik << "\n";
        }

        else if (wybor == 5) {
            int w, p;
            cout << "Wartosc elementu do zmiany: "; cin >> w;
            cout << "Nowy priorytet: "; cin >> p;
            s->modyfikuj(w, p);
            cout << "Zmieniono.\n";
        }

        else if (wybor == 6) {
            cout << "Rozmiar: " << s->size() << "\n";
        }

        else if (wybor == 7) {
            s->wyswietl();
        }

    } while (wybor != 0);
}

int main() {
    const char* opcje[] = {
        "1. Kopiec (Max-Heap)",
        "2. Tablica posortowana",
        "3. Benchmark",
        "0. Wyjdz"
    };

    int wybor;
    do {
        cout << "\nKOLEJKA PRIORYTETOWA\n";
        for (const char* o : opcje) cout << o << "\n";
        cout << "Wybor: ";
        cin >> wybor;

        if (wybor == 1) { Kopiec k; podmenu(&k, "Kopiec"); }
        else if (wybor == 2) { TablicaPosortowana t; podmenu(&t, "Tablica posortowana");}
        else if (wybor == 3) { benchmark(); }

    } while (wybor != 0);

    return 0;
}