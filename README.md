# Aplikacje-mikrokontrolerow

# mniAM
mniAM to gra, gdzie gracze to mikrokontrolery, które rywalizują pomiędzy sobą o tranzystory obecne na planszy. Mogą zjadać również siebie wzajemnie.
Klient łączy się z serwerami i podejmuję próbę nawiązania połączenia z programami graczy. Między klientem a serwerem otwierane jest połączenie TCP. Ruch danych klient->serwer jest przekazywany na port szeregowy, na którym podłączona jest płytka.

Moim zadaniem była realizacja strony serwerowej na płytce STM32 (odbieranie pakietów AMCOM, algorytm ruchu gracza oraz odsyłanie odpowiedzi do klienta).

# linked lists and ring buffer
Implementacja struktur danych: bufor kołowy, lista jednokierunkowa i lista dwukierunkowa.
