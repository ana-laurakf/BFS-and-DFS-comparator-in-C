
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct que armazena os dados de cada jogo
typedef struct {
    char nome[150];
    float horas;
} JogoInfo;

// struct do nodo da árvore - conforme os slides
typedef struct NodoArvore {
    JogoInfo info;          // dados do jogo
    struct NodoArvore *esq;
    struct NodoArvore *dir;
    int fb;                 // adicionamoa a info de fator de balanceamento, para facilitar as rotações AVL
} pNodoA;


//headers das funções

// inicializa a árvore e retorna null
pNodoA* inicializa();

// destroi a árvore (libera toda a memória)
pNodoA* destroi(pNodoA* a);

// busca um jogo pelo nome, retorna a info do jogo se achar e null se não achar
JogoInfo* busca(pNodoA* a, char* nome, int *comp);


// funções da ABP

// Iinsere um jogo numa ABP
pNodoA* insereABP(pNodoA* a, JogoInfo info);


// funções da AVL

//insere um jogo numa AVL
pNodoA* insereAVL(pNodoA* a, JogoInfo info, int *cresceu, int *nm_rotacao);

// funções de rotação
pNodoA* rotacao_direita(pNodoA* p);
pNodoA* rotacao_esquerda(pNodoA* p);
pNodoA* rotacao_dupla_direita(pNodoA* p);
pNodoA* rotacao_dupla_esquerda(pNodoA* p);

// casos de desbalanceamento
pNodoA* Caso1(pNodoA* a, int *cresceu);
pNodoA* Caso2(pNodoA* a, int *cresceu);
int Altura(pNodoA* a);

