#include "arvore.h"

int rotacoes = 0;//inicializa rotaçoes;

pNodoA* inicializa() {
    return NULL;
}

pNodoA* destroi(pNodoA* a) {
    if (a != NULL) {
        destroi(a->esq);  // libera sub-árvore esquerda
        destroi(a->dir);  // libera sub-árvore direita
        free(a);          // libera o nodo raiz
    }
    return NULL;
}
int compara = 0;//inicializa comparações com zero
JogoInfo* busca(pNodoA* a, char* nome, int *comp) {
    compara++;
    if (a == NULL) {
        return NULL; // se não encotrar, retorna null
    }
    int comparacao = strcmp(nome, a->info.nome);
    if (comparacao == 0) { // encontrou
        *comp = compara;
        return &(a->info);
    } else if (comparacao < 0) { // o nome buscado é menor, logo vai pra sub-árvore esquerda, com a recursão
        return busca(a->esq, nome, comp);
    } else { // o nome buscado é maior, logo vai pra sub-árvore direita, com a recursão
        return busca(a->dir, nome, comp);
    }
}


// funções referentes à ABP

pNodoA* insereABP(pNodoA* a, JogoInfo info) {
    if (a == NULL) {
        a = (pNodoA*) malloc(sizeof(pNodoA));
        a->info = info; // copia a struct para info do nodo
        a->esq = NULL;
        a->dir = NULL;
        a->fb = 0;
    } else {
        if (strcmp(info.nome, a->info.nome) < 0) {
            a->esq = insereABP(a->esq, info);
        } else {
            a->dir = insereABP(a->dir, info);
        }
    }
    return a;
}


// funções referentes à AVL
// implementação utilizando os slides da aula 18

// rotação simples direita
pNodoA* rotacao_direita(pNodoA* p) {
    pNodoA *u;
    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->fb = 0; // nodo p (antiga raiz) fica balanceado
    u->fb = 0; // nodo u (nova raiz) fica balanceado
    return u;
}

// rotação simples esquerda
pNodoA* rotacao_esquerda(pNodoA* p) {
    pNodoA *z;
    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->fb = 0; // nodo p (antiga raiz) fica balanceado
    z->fb = 0; // nodo z (nova raiz) fica balanceado
    return z;
}

// rotação dupla direita
pNodoA* rotacao_dupla_direita(pNodoA* p) {
    pNodoA *u, *v;
    u = p->esq;
    v = u->dir;

    // rotação esquerda em u
    u->dir = v->esq;
    v->esq = u;

    // rotação direita em p
    p->esq = v->dir;
    v->dir = p;

    // atualizaçao dos fatores de balanceamento
    if (v->fb == 1) { // v estava pendendo para esquerda
        p->fb = -1; // p "pende" pra direita
        u->fb = 0;
    } else if (v->fb == -1) { // v estava pendendo para direita
        p->fb = 0;
        u->fb = 1; // u "pende" pra esquerda
    } else { // v era o nodo inserido
        p->fb = 0;
        u->fb = 0;
    }

    v->fb = 0; // nova raiz (v) fica balanceada
    return v;
}

// rotação dupla esquerda
pNodoA* rotacao_dupla_esquerda(pNodoA* p) {
    pNodoA *z, *y;
    z = p->dir;
    y = z->esq;

    // rotação direita em z
    z->esq = y->dir;
    y->dir = z;

    // rotação esquerda em p
    p->dir = y->esq;
    y->esq = p;

    // atuaçização dos fatores de balanceamento
    if (y->fb == -1) { // y estava pendendo para direita
        p->fb = 1; // p "pende" pra esquerda
        z->fb = 0;
    } else if (y->fb == 1) { // y estava pendendo para esquerda
        p->fb = 0;
        z->fb = -1; // z "pende" pra direita
    } else { // y era o nodo inserido
        p->fb = 0;
        z->fb = 0;
    }

    y->fb = 0; // nova raiz (y) fica balanceada
    return y;
}

// caso 1: a arvore está desbalanceada à esquerda
pNodoA* Caso1(pNodoA* a, int *cresceu) {
    pNodoA *z;
    z = a->esq;
    rotacoes++;

    if (z->fb == 1) { // rotação simples direita
        a = rotacao_direita(a);
    } else { // rotação dupla direita
        a = rotacao_dupla_direita(a);
    }

    // os fatores de balanceamento já são ajustados nas funções de rotação
    *cresceu = 0; // a altura da sub-árvore foi corrigida, não cresceu.
    return a;
}

// caso 2: a arvore está desbalanceada à direita
pNodoA* Caso2(pNodoA* a, int *cresceu) {
    pNodoA *z;
    z = a->dir;
    rotacoes++;

    if (z->fb == -1) { // rotação simples esquerda
        //rotacoes++; //soma 1 à variável de controle de rotações
        a = rotacao_esquerda(a);
    } else { // rotação dupla esquerda
        //rotacoes+=2; //soma 2 à variável de controle de rotações
        a = rotacao_dupla_esquerda(a);
    }

    *cresceu = 0; // a altura da sub-árvore foi corrigida, não cresceu.
    return a;
}
// inserção AVL
pNodoA* insereAVL(pNodoA* a, JogoInfo info, int *cresceu, int *nm_rotacao) {
    if (a == NULL) {
        a = (pNodoA*) malloc(sizeof(pNodoA));
        a->info = info;
        a->esq = NULL;
        a->dir = NULL;
        a->fb = 0;
        *cresceu = 1; // o nodo foi criado, a árvore cresceu
        return a;
    }

    // compara o nome (chave)
    if (strcmp(info.nome, a->info.nome) < 0) {
        // insere à esquerda
        a->esq = insereAVL(a->esq, info, cresceu, nm_rotacao);

        if (*cresceu) { // se a sub-árvore esquerda cresceu
            switch (a->fb) {
                case -1: // estava pendendo pra direita, agora fica balanceada
                    a->fb = 0;
                    *cresceu = 0;
                    break;
                case 0:  // estava balanceada, agora pende para esquerda
                    a->fb = 1;
                    *cresceu = 1;
                    break;
                case 1:  // estava pendendo pra esquerda, agora desbalanceada
                    a = Caso1(a, cresceu); // *cresceu será setado para 0 dentro do caso 1
                    break;
            }
        }
    } else {
        // insere à direita
        a->dir = insereAVL(a->dir, info, cresceu, nm_rotacao);

        if (*cresceu) { // se a sub-árvore direita cresceu
            switch (a->fb) {
                case 1:  // estava pendendo para esquerda, agora fica balanceada
                    a->fb = 0;
                    *cresceu = 0;
                    break;
                case 0:  // estava balanceada, agora pende para direita
                    a->fb = -1;
                    *cresceu = 1;
                    break;
                case -1: // estava pendendo para direita, agora desbalanceada
                    a = Caso2(a, cresceu); // *cresceu será setado para 0 dentro do caso 2
                    break;
            }
        }
    }

    *nm_rotacao = rotacoes;
    return a;
}
int Altura (pNodoA *a){
    int Alt_Esq, Alt_Dir;

    if (a == NULL)
        return 0;
    else{

        Alt_Esq = Altura (a->esq);
        Alt_Dir = Altura (a->dir);

        if (Alt_Esq > Alt_Dir)
            return (1 + Alt_Esq);
        else
            return (1 + Alt_Dir);
    }
}

