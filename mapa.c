#include <stdio.h>
#include <stdlib.h>

#include "mapa.h"

struct smapa {
  int chave;
  int dado;
  char vermelho;
  struct smapa* esq;
  struct smapa *dir;
};

typedef enum result {OK, RED, LEFTRED, RIGHTRED} Result;

static char *strstatus[] = {"ok", "red", "leftred", "rightred"};

static Mapa *cria_no (int c, int novodado) {
  Mapa *nn = (Mapa *)malloc(sizeof(Mapa));
  if (nn!=NULL) {
    nn->esq = nn->dir = NULL;
    nn->chave = c;
    nn->vermelho = 1;
    nn->dado = novodado;
  }
  return nn;
}

Mapa* cria (void) {
  return NULL;
}

void destroi (Mapa *m) {
  if (m != NULL) {
    destroi (m->esq);
    destroi (m->dir);
    free(m);
  }
}

static void mostra_mapa_int (Mapa* m) {
  printf("(");
  if (m != NULL) {
    printf("%d-%d", m->chave, m->vermelho);
    mostra_mapa_int(m->esq);
    mostra_mapa_int(m->dir);
  }
  printf(")");
}

void mostra (Mapa* m) {
  mostra_mapa_int (m);
  printf ("\n");
}

static Mapa* rotaciona_dir(Mapa *m) {
  Mapa* esq = m->esq;
  m->esq = esq->dir;
  esq->dir = m;
  return esq;
}

static Mapa* rotaciona_esq(Mapa *m) {
  Mapa* dir = m->dir;
  m->dir = dir->esq;
  dir->esq = m;
  return dir;
}

static Mapa* trocaCores (Mapa *m) {
  char corpai = m->vermelho;
  m->vermelho = !corpai;
  if (m->esq != NULL) m->esq->vermelho = corpai;
  if (m->dir != NULL) m->dir->vermelho = corpai;
  return m;
}

static Mapa* corrigeDir (Mapa *m, Result* status) {
  switch (*status) {
    case OK:
      break;
    case RED:
      if (m->vermelho) *status = RIGHTRED;
      else *status = OK;
      break;
    case RIGHTRED:
      // Violação à direita: verificar tio esquerdo
      if (!m->esq || !m->esq->vermelho) {
        // Tio preto ou vazio
        printf("RRb\n");
        m = rotaciona_esq(m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->esq && m->esq->vermelho) {
        // Tio vermelho
        printf("RRr\n");
        m = trocaCores(m);
        *status = RED;
      }
      break;
    case LEFTRED:
      // Quando a violação vem da esquerda em corrigeDir
      if (!m->esq || !m->esq->vermelho) {
        printf("RLb\n");
        m->dir = rotaciona_dir(m->dir);
        m = rotaciona_esq(m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->esq && m->esq->vermelho) {
        printf("RLr\n");
        m = trocaCores(m);
        *status = RED;
      }
      break;
  }
  return m;
}

static Mapa* corrigeEsq (Mapa *m, Result* status) {
  switch (*status) {
    case OK:
      break;
    case RED:
      if (m->vermelho) *status = LEFTRED;
      else *status = OK;
      break;
    case LEFTRED:
      // Violação à esquerda: verificar tio direito
      if (!m->dir || !m->dir->vermelho) {
        // Tio preto ou vazio
        printf("LLb - LEFTRED: tio preto\n");
        m = rotaciona_dir(m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->dir && m->dir->vermelho) {
        // Tio vermelho
        printf("LLr - LEFTRED: tio vermelho\n");
        m = trocaCores(m);
        *status = RED;
      }
      break;
    case RIGHTRED:
      // Quando a violação vem da direita em corrigeEsq
      if (!m->dir || !m->dir->vermelho) {
        // Tio preto ou vazio
        printf("LRb\n");
        m->esq = rotaciona_esq(m->esq);
        m = rotaciona_dir(m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->dir && m->dir->vermelho) {
        // Tio vermelho
        printf("LRr\n");
        m = trocaCores(m);
        *status = RED;
      }
      break;
  }
  return m;
}

static Mapa* insereRec (Mapa* m, int chave, int novodado, Result* status) {
  if (m == NULL) {
    m = cria_no (chave, novodado);
    *status = RED;
  }
  else if (chave < m->chave) {
    m->esq = insereRec (m->esq, chave, novodado, status);
    m = corrigeEsq (m, status);
  }
  else if (chave > m->chave) {
    m->dir = insereRec (m->dir, chave, novodado, status);
    m = corrigeDir (m, status);
  }
  return m;
}

Mapa* insere (Mapa* m, int chave, int novodado) {
  Result status;
  m = insereRec (m, chave, novodado, &status);
  if (status == RED) m->vermelho = 0;
  else if (status != OK) {
    printf ("erro ao voltar para a raiz: status invalido %s !\n", strstatus[status]);
    mostra(m);
    exit(1);
  }
  return m;
}

int busca (Mapa *m, int chave) {
  if (m == NULL) {
    return -1;
  }

  if (chave == m->chave) {
    return m->dado;
  }
  else if (chave < m->chave) {
    return busca(m->esq, chave);
  }
  else {
    return busca(m->dir, chave);
  }
}

static Mapa* retiraRec (Mapa *m, int chave) {
  if (m == NULL) {
    return NULL;
  }

  if (chave < m->chave) {
    m->esq = retiraRec(m->esq, chave);
  }
  else if (chave > m->chave) {
    m->dir = retiraRec(m->dir, chave);
  }
  else {
    if (m->esq == NULL && m->dir == NULL) {
      free(m);
      return NULL;
    }
    else if (m->esq == NULL) {
      Mapa *temp = m->dir;
      free(m);
      return temp;
    }
    else if (m->dir == NULL) {
      Mapa *temp = m->esq;
      free(m);
      return temp;
    }
    else {
      Mapa *temp = m->dir;
      while (temp->esq != NULL) {
        temp = temp->esq;
      }

      m->chave = temp->chave;
      m->dado = temp->dado;

      m->dir = retiraRec(m->dir, temp->chave);
    }
  }

  return m;
}

Mapa* retira (Mapa *m, int chave) {
  return retiraRec(m, chave);
}