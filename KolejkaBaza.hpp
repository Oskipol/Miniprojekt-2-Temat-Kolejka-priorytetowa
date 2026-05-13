#pragma once 

// Klasa bazowa kolejki priorytetowej MAX
// Element to para, wartość i priorytet
class KolejkaBaza {
public:
    virtual void insert(int wartosc, int priorytet)=0; // dodaj element
    virtual int extractMax()=0; // usuń i zwróć element o max priorytecie
    virtual int findMax()=0; // podejrzyj max bez usuwania
    virtual void modifyKey(int wartosc, int nowyPriorytet)=0; // zmien priorytet elementu
    virtual int size()=0; // rozmiar
    virtual void wyswietl()=0; // wyświetl zawartość
    virtual void wypelnij(const int* wartosci, const int* priorytety, int n)=0;
    virtual ~KolejkaBaza(){}
};
 
