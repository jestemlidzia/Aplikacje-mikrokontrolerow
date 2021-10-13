#include <stdio.h>
#include <string.h>
#include <assert.h>

// (1) Uzupełnij funkcję searchStream oraz strukturę StreamState tak, aby umożliwić przetwarzanie danych strumieniowo, poszukując pewnej sekwencji znaków.
//     Kolejne porcje strumienia danych przekazywane są poprzez kolejne wywołania funkcji searchStream poprzez argument 'data'.
//     Rozmiar każdej kolejnej porcji danych (ilość znaków strumienia) zadawana jest poprzez argument size.
//
//     Poprzez tzw. sekwencję znaków będziemy rozumieć ciąg znaków rozpoczynjący się od znaku '#" i przerwany dopiero białym znakiem lub kolejnym znakiem '#'
//     Przykładowo:
//         #abcd              - to sekwencja o rozmiarze 4
//         #12345678xy        - to sekwencja o rozmiarze 10
//     Sekwencje mogą znajdować się losowo w strumieniu danych np:
//         w tym ciagu danych jest#sekwencja oraz #drug4-sekwencj4 bla bla bla
//
//     Funkcja searchStream powinna zwracać najdłuższą, napotkaną dotychczas ilość znaków w sekwencji.
//     Dla przykładu powyżej wynik funkcji to 15 (drug4-sekwencj4), o ile wcześniej nie napotkano dłuższej sekwencji.
//     Każdy znak '#' kończy poprzednią sekwencję, przykładowo:
//         abc#sekwencja#abcde blabla}    - w takim przypadku funkcja search searchStream powinna rozpoznać 2 sekwencje #sekwencja i #abcde
//
//     Struktura StreamState powinna przechowywać informacje o stanie poszukiwania sekwencji w strumieniu
//     Zwróć uwagę na to, że strumień może zostać przerwany w połowie sekwencji tj. pierwsza porcja danych strumienia może zawierać początek
//     a druga koniec sekwencji i taka sekwencja powinna zostać uwzględniona.


typedef struct {
    // TODO
    int state;
    int currentSize;
    int maxSize;

} StreamState;

size_t searchStream(StreamState* state, const char* data, size_t size) {
    // TODO

    for (int i = 0; i < size; i++) {
        if (state->state == 0) {//szuka poczatku nowego
            if (data[i] == '#') {
                state->state = 1;
            }
        }
        else if (state->state == 1) {//nie skonczone, w trakcie liczenia
            //koniec sekwencji, zapisuje nowy najwiekszy rozmiar jesli jest, state = 0, bo bedzie szukal nowej sekwencji
            if (data[i] == ' ') {
                if (state->currentSize > state->maxSize) { 
                    state->maxSize = state->currentSize;
                }
                state->state = 0;
                state->currentSize = 0;
            }
            // koniec sekwencji, zapisuje nowy najwiekszy rozmiar jesli jest, state = 1, bo juz znalazl nowa sekwencje konczac rownoczesnie poprzednia
            else if (data[i] == '#') {
                if (state->currentSize > state->maxSize) { //koiniec sekwencji, zapisuje nowy najwiekszy rozmiar
                    state->maxSize = state->currentSize;
                }
                state->currentSize = 0;
            }
            else {
                state->currentSize = state->currentSize + 1;
            }
        }
    }

    return state->maxSize;
}

// (2) Uzupełnij funkcję clearStream aby zerowała stan poszukiwania sekwencji w strumieniu
void clearStream(StreamState* state) {
    // TODO
    state->state = 0;
    state->maxSize = 0;
    state->currentSize = 0;
}



int main()
{
    printf("Testujemy funkcję searchStream:\n");

    StreamState ss;

    // wyzeruj stan strumienia
    clearStream(&ss);
    // na początku ma być 0:
    assert(0 == searchStream(&ss, "", 0));
    // tu nie ma sekwencji:
    assert(0 == searchStream(&ss, "abcdefgh", 8));
    // tu jest sekwencja (8):
    assert(5 == searchStream(&ss, "abcd#12345 678efgh", 18));
    // tu też sekwencja, ale krótsza (4):
    assert(5 == searchStream(&ss, "ab cd #1234  ef ghij", 20));
    // tu są dwie (4) (6):
    assert(6 == searchStream(&ss, "ab #1234#efghij  bla", 20));
    // tu jest sekwencja ale rozłożona na dwie porcje strumienia (9):
    assert(6 == searchStream(&ss, "poczatek:#sekw", 14));
    assert(9 == searchStream(&ss, "encja :koniec", 13));
    // wyzeruj stan strumienia
    clearStream(&ss);
    // znów zaczynamy od zera
    assert(3 == searchStream(&ss, "abc#def ghi", 11));

    printf("Sukces!\n");
    return 0;
}
