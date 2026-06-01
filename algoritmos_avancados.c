#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura básica de cada cômodo
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função que cria uma sala na memória
Sala* criarSala(const char* nomeSala) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nomeSala);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função de navegação interativa
void explorarSalas(Sala* raiz) {
    Sala* atual = raiz;
    char escolha;

    printf("=== DETECTIVE QUEST ===\n");

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se não houver caminhos para os dois lados, é um nó-folha (fim do jogo)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim da linha! Investigação encerrada.\n");
            break;
        }

        // Menu de opções
        printf("Para onde deseja ir?\n");
        printf("[e] Esquerda para: %s\n", atual->esquerda->nome);
        printf("[d] Direita para: %s\n", atual->direita->nome);
        printf("[s] Sair do jogo\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        // Controles de decisão
        if (escolha == 's') {
            printf("Você saiu do jogo.\n");
            break;
        } else if (escolha == 'e') {
            atual = atual->esquerda;
        } else if (escolha == 'd') {
            atual = atual->direita;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função principal que monta a mansão e inicia o jogo
int main() {
    // 1. Criação dos cômodos
    Sala* hall = criarSala("Hall de Entrada");
    Sala* estar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* porao = criarSala("Porão");

    // 2. Montagem manual do mapa (Árvore Binária)
    hall->esquerda = estar;
    hall->direita = cozinha;

    estar->esquerda = biblioteca; // Fim de linha pela esquerda
    cozinha->direita = porao;     // Fim de linha pela direita

    // 3. Início do jogo
    explorarSalas(hall);

    return 0;
}