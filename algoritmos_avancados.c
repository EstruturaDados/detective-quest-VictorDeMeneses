#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 13

// --- ESTRUTURAS DE DADOS (STRUCTS) ---

// Nó da Árvore Binária de Busca (BST) para o inventário de pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esq, *dir;
} PistaNode;

// Nó da Árvore Binária que representa as Salas do Mapa
typedef struct Sala {
    char nome[50];
    char pista[100]; 
    struct Sala *esquerda, *direita;
} Sala;

// Elemento da Tabela Hash (Encadeamento simples para tratar colisões)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;


// --- FUNÇÕES DA TABELA HASH ---

/* * Função: funcaoHash
 * Objetivo: Calcula o índice na tabela a partir de uma string (pista) 
 * usando o método de espalhamento por soma de caracteres.
 */
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

/* * Função: inserirNaHash
 * Objetivo: Insere a associação Pista -> Suspeito dentro da tabela hash.
 */
void inserirNaHash(HashNode** tabela, const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    
    // Insere no início da lista encadeada daquela posição (Tratamento de colisão)
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

/* * Função: encontrarSuspeito
 * Objetivo: Consulta a tabela hash para descobrir qual suspeito está 
 * vinculado a uma determinada pista coletada.
 */
const char* encontrarSuspeito(HashNode** tabela, const char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return "Desconhecido";
}


// --- FUNÇÕES DA ÁRVORE DE PISTAS (BST) ---

/* * Função: inserirPista
 * Objetivo: Insere de forma recursiva uma pista coletada na árvore BST 
 * mantendo a ordem alfabética.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* textoPista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->texto, textoPista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(textoPista, raiz->texto) < 0) {
        raiz->esq = inserirPista(raiz->esq, textoPista);
    } else {
        raiz->dir = inserirPista(raiz->dir, textoPista);
    }
    return raiz;
}

/* * Função: exibirPistas
 * Objetivo: Exibe as pistas guardadas no inventário em ordem alfabética.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->dir);
    }
}


// --- FUNÇÕES DA LÓGICA DE JULGAMENTO RECURSIVA ---

/* * Função: contarPistasDoSuspeito
 * Objetivo: Percorre a árvore BST de pistas coletadas e, consultando a Hash,
 * conta quantas dessas pistas incriminam o suspeito acusado pelo jogador.
 */
int contarPistasDoSuspeito(PistaNode* raizPistas, HashNode** tabelaHash, const char* acusado) {
    if (raizPistas == NULL) return 0;

    int contagemAtual = 0;
    // Descobre quem esta pista incrimina usando a Hash
    const char* suspeitoVinculado = encontrarSuspeito(tabelaHash, raizPistas->texto);
    
    if (strcmp(suspeitoVinculado, acusado) == 0) {
        contagemAtual = 1;
    }

    // Soma o nó atual com os resultados das subárvores esquerda e direita
    return contagemAtual + 
           contarPistasDoSuspeito(raizPistas->esq, tabelaHash, acusado) + 
           contarPistasDoSuspeito(raizPistas->dir, tabelaHash, acusado);
}

/* * Função: verificarSuspeitoFinal
 * Objetivo: Conduz o encerramento do jogo, exibe o inventário, recolhe a acusação
 * e valida se há provas lógicas suficientes (mínimo de 2 pistas).
 */
void verificarSuspeitoFinal(PistaNode* inventario, HashNode** tabelaHash) {
    char acusacao[50];
    
    printf("\n================================================\n");
    printf("            FALA DO JUIZ: O JULGAMENTO          \n");
    printf("================================================\n");
    printf("Pistas que você conseguiu reunir:\n");
    
    if (inventario == NULL) {
        printf("- Nenhuma pista encontrada.\n");
    } else {
        exibirPistas(inventario);
    }
    
    printf("\nLista de Suspeitos na Mansão: [Mordomo], [Cozinheira], [Jardineiro]\n");
    printf("Detetive, quem é o culpado? Digite o nome exatamente como listado: ");
    scanf(" %49s", acusacao);

    // Processa a contagem de provas usando a árvore e a hash de forma casada
    int totalProvas = contarPistasDoSuspeito(inventario, tabelaHash, acusacao);

    printf("\nAnalisando o caso...\n");
    printf("Você apresentou %d prova(s) contra o(a) %s.\n", totalProvas, acusacao);

    if (totalProvas >= 2) {
        printf("\n[SUCESSO] Excelente trabalho! O(A) %s desabou em lágrimas e confessou.\n", acusacao);
        printf("Suas pistas eram sólidas e o caso foi fechado com maestria!\n");
    } else {
        printf("\n[FRACASSO] Caso ARQUIVADO por falta de provas robustas.\n");
        printf("Acusar sem ao menos duas pistas concretas gerou um erro judicial. O culpado escapou!\n");
    }
    printf("================================================\n");
}


// --- FUNÇÕES DO MAPA DA MANSÃO ---

/* * Função: criarSala
 * Objetivo: Aloca dinamicamente um cômodo com seu nome e sua pista padrão.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

/* * Função: explorarSalas
 * Objetivo: Loop principal que guia a movimentação entre cômodos e transfere 
 * as pistas encontradas para a árvore de inventário (BST).
 */
void explorarSalas(Sala* raiz, HashNode** tabelaHash) {
    Sala* atual = raiz;
    PistaNode* inventario = NULL;
    char escolha;

    printf("=== DETECTIVE QUEST: O JULGAMENTO FINAL ===\n");

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Coleta de pista dinâmica ao entrar no cômodo
        if (strlen(atual->pista) > 0) {
            printf("[Pista Encontrada] \"%s\"\n", atual->pista);
            inventario = inserirPista(inventario, atual->pista);
            strcpy(atual->pista, ""); // Evita que colete novamente se voltar à sala
        }

        printf("Direções para explorar:\n");
        if (atual->esquerda) printf(" [e] Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita)  printf(" [d] Direita: %s\n", atual->direita->nome);
        printf(" [s] Ir para o Julgamento Final\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita) {
            atual = atual->direita;
        } else {
            printf("[Aviso] Direção indisponível. Tente novamente.\n");
        }
    }

    // Dispara a fase final do jogo
    verificarSuspeitoFinal(inventario, tabelaHash);
}


// --- MAIN ---

int main() {
    // 1. Inicializa a Tabela Hash com ponteiros nulos
    HashNode* tabelaHash[TAMANHO_HASH] = {NULL};

    // 2. Alimenta as associações lógicas na Hash (Pista -> Suspeito)
    inserirNaHash(tabelaHash, "Bilhete de ameaca", "Mordomo");
    inserirNaHash(tabelaHash, "Pegadas com lama na sala", "Mordomo");
    inserirNaHash(tabelaHash, "Faca de prata limpa", "Cozinheira");
    inserirNaHash(tabelaHash, "Veneno de rato no armario", "Cozinheira");
    inserirNaHash(tabelaHash, "Luvas sujas de terra", "Jardineiro");

    // 3. Montagem manual e fixa da Mansão (Árvore Binária)
    Sala* hall       = criarSala("Hall de Entrada", "");
    Sala* biblioteca = criarSala("Biblioteca", "Bilhete de ameaca");
    Sala* cozinha    = criarSala("Cozinha", "Faca de prata limpa");
    Sala* escritorio = criarSala("Escritorio", "Pegadas com lama na sala");
    Sala* porao      = criarSala("Porao", "Veneno de rato no armario");

    hall->esquerda = biblioteca;
    hall->direita  = cozinha;
    
    biblioteca->esquerda = escritorio;
    cozinha->direita     = porao;

    // 4. Início da gameplay
    explorarSalas(hall, tabelaHash);

    return 0;
}