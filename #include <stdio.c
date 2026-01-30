#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



typedef struct {
    int id;
    char nome[50];
    int classifGeral;
    int classifNegro; 
    int classifPCD;   
} Candidato;


typedef struct Node {
    Candidato dados;
    char modalidade[30];
    int posicao_convocacao;
    struct Node *prox;
} Node;


typedef struct {
    Node *inicio;
    Node *fim;
} Fila;



void inicializarFila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}

void enfileirar(Fila *f, Candidato c, char *modalidade, int posicao) {
    Node *novo = (Node*) malloc(sizeof(Node));
    novo->dados = c;
    strcpy(novo->modalidade, modalidade);
    novo->posicao_convocacao = posicao;
    novo->prox = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
}

void imprimirRelatorioFinal(Fila *f) {
    printf("\n=================================================================\n");
    printf("                  RESULTADO FINAL DA CONVOCAÇÃO                  \n");
    printf("=================================================================\n");
    printf("%-5s %-30s %-20s %-5s\n", "Pos", "Nome", "Modalidade", "ID");
    printf("-----------------------------------------------------------------\n");

    Node *atual = f->inicio;
    while (atual != NULL) {
        printf("%-5d %-30s %-20s %-5d\n", 
               atual->posicao_convocacao, 
               atual->dados.nome, 
               atual->modalidade, 
               atual->dados.id);
        atual = atual->prox;
    }
    printf("-----------------------------------------------------------------\n");
}


int obterTipoVaga(int n) {

    if (n == 21 || (n > 21 && (n - 21) % 20 == 0)) {
        return 2; 
    }

    if ((n % 5) == 3) {
        return 1;
    }
    return 0; // Ampla
}

void imprimirListaOrigem(Candidato *lista, int qtd, char *titulo, int filtro) {
    printf("\n========== %s ==========\n", titulo);
    printf("%-5s %-30s %-15s %-15s %-15s\n", "ID", "Nome", "Classif.Geral", "Classif.Negro", "Classif.PCD");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < qtd; i++) {
        // Filtro 1: Apenas Negros, Filtro 2: Apenas PCD, Filtro 0: Todos
        if (filtro == 1 && lista[i].classifNegro == 0) continue;
        if (filtro == 2 && lista[i].classifPCD == 0) continue;

        printf("%-5d %-30s %-15d %-15d %-15d\n", 
            lista[i].id, lista[i].nome, lista[i].classifGeral, lista[i].classifNegro, lista[i].classifPCD);
    }
}

int main() {
    Candidato listaGeral[] = {
        {1, "Ana Silva", 1, 0, 0},
        {2, "Joao Santos", 2, 1, 0},
        {3, "Maria Oliveira", 3, 0, 0},
        {4, "Pedro Costa", 4, 2, 0},
        {5, "Julia Lima", 5, 0, 0},
        {6, "Carlos Souza", 6, 0, 0},
        {7, "Beatriz Alves", 7, 3, 0},
        {8, "Lucas Pereira", 8, 0, 0},
        {9, "Fernanda Rocha", 9, 0, 0},
        {10, "Rafael Martins", 10, 0, 0},
        {11, "Juliana Ferreira", 11, 0, 0},
        {12, "Bruno Cardoso", 12, 0, 0},
        {13, "Amanda Ribeiro", 13, 0, 0},
        {14, "Diego Nascimento", 14, 0, 0},
        {15, "Camila Azevedo", 15, 0, 1},
        {16, "Thiago Barbosa", 16, 0, 0},
        {17, "Patricia Dias", 17, 0, 0},
        {18, "Rodrigo Monteiro", 18, 0, 0},
        {19, "Larissa Castro", 19, 0, 0},
        {20, "Marcos Correia", 20, 4, 0},
        {21, "Fulano", 21, 0, 0},
        {22, "Wesley", 22, 0, 2}
    };
    int totalCandidatos = 22;

    
    Candidato* listaNegros[22];
    int qtdNegros = 0;
    Candidato* listaPCDs[22];
    int qtdPCDs = 0;

    for(int i=0; i<totalCandidatos; i++) {
        if(listaGeral[i].classifNegro > 0) listaNegros[qtdNegros++] = &listaGeral[i];
        if(listaGeral[i].classifPCD > 0) listaPCDs[qtdPCDs++] = &listaGeral[i];
    }

    imprimirListaOrigem(listaGeral, totalCandidatos, "LISTA AMPLA CONCORRÊNCIA", 0);
    imprimirListaOrigem(listaGeral, totalCandidatos, "LISTA COTAS RACIAIS", 1);
    imprimirListaOrigem(listaGeral, totalCandidatos, "LISTA PCD", 2);


    
    Fila filaConvocacao;
    inicializarFila(&filaConvocacao);

    
    int jaConvocado[100] = {0}; 


    int idxGeral = 0;
    int idxNegro = 0;
    int idxPCD = 0;

  
    for (int vaga = 1; vaga <= totalCandidatos; vaga++) {
        int tipo = obterTipoVaga(vaga);
        Candidato *selecionado = NULL;
        char modalidadeTexto[30];

        
        if (tipo == 2) { 
            while(idxPCD < qtdPCDs && jaConvocado[listaPCDs[idxPCD]->id]) {
                idxPCD++;
            }
            if (idxPCD < qtdPCDs) {
                selecionado = listaPCDs[idxPCD];
                strcpy(modalidadeTexto, "Cota PCD");
            }
        } 
        else if (tipo == 1) { 
         
            while(idxNegro < qtdNegros && jaConvocado[listaNegros[idxNegro]->id]) {
                idxNegro++;
            }
            if (idxNegro < qtdNegros) {
                selecionado = listaNegros[idxNegro];
                strcpy(modalidadeTexto, "Cota Racial");
            }
        }

        if (selecionado == NULL) {
            while(idxGeral < totalCandidatos && jaConvocado[listaGeral[idxGeral].id]) {
                idxGeral++;
            }
            if (idxGeral < totalCandidatos) {
                selecionado = &listaGeral[idxGeral];
                strcpy(modalidadeTexto, "Ampla Concorrência");
            }
        }

        
        if (selecionado != NULL) {
            enfileirar(&filaConvocacao, *selecionado, modalidadeTexto, vaga);
            jaConvocado[selecionado->id] = 1;
        }
    }


    imprimirRelatorioFinal(&filaConvocacao);

   
    Node *temp = filaConvocacao.inicio;
    while(temp != NULL) {
        Node *prox = temp->prox;
        free(temp);
        temp = prox;
    }

    return 0;
}