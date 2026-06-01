#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUTURAS DE DADOS (STRUCTS) ---

// Nó para a Árvore Binária de Busca (BST) que guardará as pistas ordenadas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esq, *dir;
} PistaNode;

// Nó para a Árvore Binária que representa o Mapa da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista contida no cômodo (vazia se não houver)
    struct Sala *esquerda, *direita;
} Sala;


// --- FUNÇÕES DA ÁRVORE DE PISTAS (BST) ---

/* * Função: inserirPista
 * Objetivo: Insere de forma recursiva uma pista na árvore BST,
 * garantindo a ordenação alfabética (menores à esquerda, maiores à direita).
 */
PistaNode* inserirPista(PistaNode* raiz, const char* textoPista) {
    if (raiz == NULL) {
        PistaNode* novoNode = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novoNode->texto, textoPista);
        novoNode->esq = novoNode->dir = NULL;
        return novoNode;
    }

    // Compara a nova pista com o texto do nó atual para decidir o lado
    if (strcmp(textoPista, raiz->texto) < 0) {
        raiz->esq = inserirPista(raiz->esq, textoPista);
    } else {
        raiz->dir = inserirPista(raiz->dir, textoPista);
    }

    return raiz;
}

/* * Função: exibirPistas
 * Objetivo: Percorre a árvore BST em-ordem (Esquerda -> Raiz -> Direita).
 * Isso garante que as pistas sejam exibidas em ordem alfabética.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->dir);
    }
}


// --- FUNÇÕES DO MAPA DA MANSÃO ---

/* * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo da mansão com seu respectivo nome e pista.
 */
Sala* criarSala(const char* nomeSala, const char* pistaSala) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nomeSala);
    strcpy(novaSala->pista, pistaSala);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

/* * Função: explorarSalasComPistas
 * Objetivo: Controla a navegação do jogador pelos cômodos e faz a coleta
 * automática de pistas, inserindo-as na árvore BST.
 */
void explorarSalasComPistas(Sala* mapaRaiz) {
    Sala* atual = mapaRaiz;
    PistaNode* inventarioPistas = NULL; // Raiz da nossa BST de pistas
    char escolha;

    printf("=== DETECTIVE QUEST: EXPANSÃO COLETA DE PISTAS ===\n");

    while (atual != NULL) {
        printf("\n------------------------------------------------\n");
        printf("Você está na sala: %s\n", atual->nome);

        // Sistema de coleta automática de pista
        if (strlen(atual->pista) > 0) {
            printf("[Pista Encontrada] \"%s\"\n", atual->pista);
            // Salva na BST de forma ordenada
            inventarioPistas = inserirPista(inventarioPistas, atual->pista);
            // Limpa a pista do cômodo para não coletar repetidamente
            strcpy(atual->pista, ""); 
        }

        // Menu de Opções de movimentação
        printf("Para onde deseja ir?\n");
        if (atual->esquerda != NULL) printf(" [e] Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" [d] Direita: %s\n", atual->direita->nome);
        printf(" [s] Sair e encerrar investigação\n");
        
        printf("Escolha: ");
        scanf(" %c", &escolha);

        // Controle de decisões de navegação
        if (escolha == 's' || escolha == 'S') {
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("\n[Aviso] Caminho inválido ou bloqueado. Tente novamente!\n");
        }
    }

    // Tela final ao sair
    printf("\n================================================\n");
    printf("            RELATÓRIO FINAL DO DETETIVE         \n");
    printf("================================================\n");
    printf("Pistas coletadas (em ordem alfabética):\n");
    
    if (inventarioPistas == NULL) {
        printf("- Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(inventarioPistas);
    }
    printf("================================================\n");
}


// --- FUNÇÃO PRINCIPAL ---

int main() {
    // 1. Criação dos cômodos com pistas associadas
    Sala* hall       = criarSala("Hall de Entrada", ""); 
    Sala* biblioteca = criarSala("Biblioteca", "Diário antigo com a página 42 rasgada");
    Sala* cozinha    = criarSala("Cozinha", "Faca de prata limpa recentemente");
    Sala* escritorio = criarSala("Escritório", "Bilhete de ameaça na gaveta");
    Sala* porao      = criarSala("Porão", "Pegadas de lama perto da janela");

    // 2. Montagem da Árvore Binária (Mapa da Mansão)
    hall->esquerda = biblioteca;
    hall->direita  = cozinha;

    biblioteca->esquerda = escritorio;
    cozinha->direita     = porao;

    // 3. Início do Jogo
    explorarSalasComPistas(hall);

    return 0;
}