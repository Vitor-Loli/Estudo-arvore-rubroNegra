#ifndef MAPA_H
#define MAPA_H

typedef struct smapa Mapa;

Mapa* cria (void);
Mapa* insere (Mapa* m, int chave, int novodado);
int busca (Mapa *m, int chave);
Mapa* retira (Mapa *m, int chave);
void destroi (Mapa *m);

#endif //MAPA_H
