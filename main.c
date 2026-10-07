#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "arvore.h"

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, ""); // para imprimir os acentos

    //variáveis associadas ao tempo
    clock_t start_load, end_load, start_search, end_search;
    double time_load, time_search;

    // inclusão de variáveis de arquivo
    FILE *f_dataset;
    FILE *f_lista;
    FILE *f_saida;
    char linha[1000];
    char *token;

    //variáveis da árvore
    pNodoA *arvore = inicializa();
    char* tipoArvore;
    JogoInfo novoJogo;
    JogoInfo* jogoEncontrado;

    //variáveis de resultado
    float totalHoras = 0.0;
    int cresceu_avl = 0; // flag para a inserção AVL
    int nm_rotacao;// variável de controle de rotações
    int nm_nodos; //variável de controle de nodos;
    int alturaArvore;

    nm_rotacao = 0;//inicializa o número de rotações
    nm_nodos = 0; //inicializa no número de nodods

    // confirma se foram passados os parâmetros corretos
    if (argc != 4) {
        printf("Numero incorreto de parametros.\n");
        printf("Uso: %s <abp|avl> <arq_dataset> <arq_lista>\n", argv[0]);
        return 1;
    }

    tipoArvore = argv[1];
    char* arqDataset = argv[2];
    char* arqLista = argv[3];

    if (strcmp(tipoArvore, "abp") != 0 && strcmp(tipoArvore, "avl") != 0) {
        printf("Tipo de arvore invalido. Use 'abp' ou 'avl'.\n");
        return 1;
    }

    // carregamento do dataset e construção da árvore

    printf("Iniciando Fase de Carregamento (%s)...\n", tipoArvore);
    f_dataset = fopen(arqDataset, "r");
    if (f_dataset == NULL) {
        printf("Erro ao abrir o arquivo de dataset %s\n", arqDataset);
        return 1;
    }

    start_load = clock(); // inicia o timer

    // lê a primeira linha (cabeçalho) e ignora
    fgets(linha, 1000, f_dataset);

    while (fgets(linha, 1000, f_dataset)) {
        // pega o nome do jogo (antes da vírgula)
        token = strtok(linha, ",");
        if (token) {
            strcpy(novoJogo.nome, token);
        }

        // pega as horas jogadas (depois da vírgula)
        token = strtok(NULL, "\n"); // pega o restante da linha após a vírgula
        if (token) {
            novoJogo.horas = atof(token); // faz a conversão de string pra float
        }

        // insere o jogo na árvore
        if (strcmp(tipoArvore, "abp") == 0) {
            arvore = insereABP(arvore, novoJogo);
            nm_nodos++;
        } else {
            arvore = insereAVL(arvore, novoJogo, &cresceu_avl, &nm_rotacao);
            nm_nodos++;
        }
    }

    end_load = clock(); // enserra o timer
    fclose(f_dataset);

    time_load = ((double)(end_load - start_load)) / CLOCKS_PER_SEC;
    printf("Fase de Carregamento concluida.\n");
    printf("Tempo de Carregamento: %.6f segundos\n", time_load);

    // busca na lista do jogador

    printf("\nIniciando Fase de Busca...\n");
    f_lista = fopen(arqLista, "r");
    if (f_lista == NULL) {
        printf("Erro ao abrir o arquivo de lista %s\n", arqLista);
        destroi(arvore); // libera memória antes de sair
        return 1;
    }
    int comp;

    comp = 0;
    start_search = clock(); // inicia contagem do tempo
    while (fgets(linha, 1000, f_lista)) {
        linha[strcspn(linha, "\r\n")] = 0;

        jogoEncontrado = busca(arvore, linha, &comp);

        if (jogoEncontrado != NULL) {
            totalHoras += jogoEncontrado->horas;
        } else {
            printf("AVISO: Jogo '%s' da lista nao encontrado no dataset.\n", linha);
        }
    }

    end_search = clock(); // encerra a contagem de tempo
    fclose(f_lista);

    time_search = ((double)(end_search - start_search)) / CLOCKS_PER_SEC;
    printf("Fase de Busca concluida.\n");
    printf("Tempo de Busca: %.6f segundos\n", time_search);

    alturaArvore = Altura(arvore);

    // printa os resultados
    printf("\n--- Resultados Finais ---\n");
    printf("Tipo de Arvore: %s\n", tipoArvore);
    printf("Tempo Total de Carregamento: %.6f s\n", time_load);
    printf("Tempo Total de Busca: %.6f s\n", time_search);
    printf("Altura da árvore: %d\n", alturaArvore);
    printf("Total de Horas a Jogar: %.2f horas\n", totalHoras);
    printf("Total de comparacao: %d comparacoes\n", comp);
    printf("Total de rotações: %d rotações\n", nm_rotacao);
    printf("Total de nodos: %d nodos\n", nm_nodos);

    /*Gerando o arquivo de saída*/

    f_saida = fopen("C:\\Users\\luisg\\OneDrive\\Documentos\\TrabalhoFinal\\arquivo_saida.txt", "w");//cria o arquivo de saida no endereço passado
    if(f_saida == NULL){
        printf("Erro ao gerar o arquivo de saída\n"); // mensagem de erro caso tenha gerado erro ao gerar o arquivo
    } else {
        if(fprintf(f_saida, "Tipo de Arvore: %s\n", tipoArvore) < 0){ // coloca o tipo de arvore(avl ou abp) no arquivo de saída
            printf("Erro ao inserir o tipo de árvore\n");
        }
        if(fprintf(f_saida, "Total de Horas a Jogar: %.2f horas\n", totalHoras) < 0){ // coloca as horas totais no arquivo de saída
            printf("Erro ao inserir as horas totais\n");
        }
        if(fprintf(f_saida, "Total de comparacao: %d comparacoes\n", comp) < 0){ // coloca o número de comparações no arquivo de saída
            printf("Erro ao inserir o número de comparações\n");
        }
        if(fprintf(f_saida, "Total de rotações: %d rotações\n", nm_rotacao) < 0){ // coloca o número de rotações no arquivo de saída
            printf("Erro ao inserir o número de rotações\n");
        }
        if(fprintf(f_saida,"Altura da árvore: %d\n", alturaArvore) < 0){ // coloca a altura da ávore(avl ou abp) no arquivo de saída
            printf("Erro ao inserir a altura da árvore\n");
        }
        if(fprintf(f_saida, "Total de nodos: %d nodos\n", nm_nodos) < 0){ // coloca o número de nodos no arquivo de saída
            printf("Erro ao inserir o número de nodos\n");
        }
    }

    // limpa a memória
    printf("\nLimpando memoria...\n");
    arvore = destroi(arvore);
    printf("Concluido.\n");

    return 0;
}
// C:\Users\luisg\OneDrive\Documentos\TrabalhoFinal\bin\Debug\TrabalhoFinal.exe avl C:\Users\luisg\OneDrive\Documentos\TrabalhoFinal\dataset.csv C:\Users\luisg\OneDrive\Documentos\TrabalhoFinal\lista_jogador1.txt//
