#pragma once
#include "KolejkaBaza.hpp"
#include <iostream>
using namespace std;

class TablicaPosortowana : public KolejkaBaza {
    struct Element {
        int wartosc;
        int priorytet;
    };

    Element* dane;
    int rozmiar;
    int pojemnosc;

    void rozszerz();

public:
    TablicaPosortowana();
    ~TablicaPosortowana();

    int  size() override;
    void wstaw(int wartosc, int priorytet) override;
    int  usunMax() override;
    int  znajdzMax() override;
    void modyfikuj(int wartosc, int nowyPriorytet) override;
    void wyswietl() override;
    void wypelnij(const int* wartosci, const int* priorytety, int n) override;
};