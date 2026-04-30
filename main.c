#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include "cmapa.h"
#include "arvore.h"

#define TAM 12

int main(void) {

    int chavedada;

    Mapa *mapa1 = cria();
    Mapa *mapa2 = cria();

    while (1) {
        printf ("inserir em mapa1 (digite -111 para terminar): ");
        scanf ("%d", &chavedada);
        if (chavedada==-111) break;
        mapa1 = insere(mapa1, chavedada, 2*chavedada);
        printf("mapa:\n");
        mostra(mapa1);

    }

    destroi (mapa1);
    destroi (mapa2);
    return 0;
}

