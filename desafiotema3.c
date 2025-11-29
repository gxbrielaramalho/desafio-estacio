#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura Peça
typedef struct {
    char nome;      // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;         // Identificador único da peça
} Peca;

// Definição da Fila Circular
#define TAM_FILA 5
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} FilaCircular;

// Definição da Pilha
#define TAM_PILHA 3
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
    int quantidade;
} Pilha;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca(int id);
int filaVazia(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void enfileirar(FilaCircular *fila, Peca peca);
Peca desenfileirar(FilaCircular *fila);
void empilhar(Pilha *pilha, Peca peca);
Peca desempilhar(Pilha *pilha);
void exibirEstado(FilaCircular *fila, Pilha *pilha);
void jogarPeca(FilaCircular *fila, int *proximoId);
void reservarPeca(FilaCircular *fila, Pilha *pilha, int *proximoId);
void usarPecaReservada(Pilha *pilha);
void trocarPecaFrente(FilaCircular *fila, Pilha *pilha);
void trocarMultipla(FilaCircular *fila, Pilha *pilha);
void preencherFila(FilaCircular *fila, int *proximoId);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    
    FilaCircular fila;
    Pilha pilha;
    int opcao;
    int proximoId = 0; // Contador para IDs únicos
    
    // Inicialização das estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Preenche a fila inicialmente
    preencherFila(&fila, &proximoId);
    
    printf("=== GERENCIADOR DE PECAS ===\n");
    printf("Sistema integrado: Fila Circular (%d peças) + Pilha (%d peças)\n\n", TAM_FILA, TAM_PILHA);
    
    do {
        exibirEstado(&fila, &pilha);
        
        printf("\n=== OPCOES DISPONIVEIS ===\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                jogarPeca(&fila, &proximoId);
                break;
            case 2:
                reservarPeca(&fila, &pilha, &proximoId);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaFrente(&fila, &pilha);
                break;
            case 5:
                trocarMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        printf("\n");
        
    } while (opcao != 0);
    
    return 0;
}

/**
 * Inicializa a fila circular
 * @param fila Ponteiro para a fila a ser inicializada
 */
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->quantidade = 0;
}

/**
 * Inicializa a pilha
 * @param pilha Ponteiro para a pilha a ser inicializada
 */
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->quantidade = 0;
}

/**
 * Gera uma peça aleatória com ID único
 * @param id ID único para a peça
 * @return Peça gerada
 */
Peca gerarPeca(int id) {
    Peca peca;
    peca.id = id;
    
    // Gera um tipo de peça aleatório: 'I', 'O', 'T', 'L'
    char tipos[] = {'I', 'O', 'T', 'L'};
    peca.nome = tipos[rand() % 4];
    
    return peca;
}

/**
 * Verifica se a fila está vazia
 * @param fila Ponteiro para a fila
 * @return 1 se vazia, 0 caso contrário
 */
int filaVazia(FilaCircular *fila) {
    return fila->quantidade == 0;
}

/**
 * Verifica se a fila está cheia
 * @param fila Ponteiro para a fila
 * @return 1 se cheia, 0 caso contrário
 */
int filaCheia(FilaCircular *fila) {
    return fila->quantidade == TAM_FILA;
}

/**
 * Verifica se a pilha está vazia
 * @param pilha Ponteiro para a pilha
 * @return 1 se vazia, 0 caso contrário
 */
int pilhaVazia(Pilha *pilha) {
    return pilha->quantidade == 0;
}

/**
 * Verifica se a pilha está cheia
 * @param pilha Ponteiro para a pilha
 * @return 1 se cheia, 0 caso contrário
 */
int pilhaCheia(Pilha *pilha) {
    return pilha->quantidade == TAM_PILHA;
}

/**
 * Adiciona uma peça na fila circular
 * @param fila Ponteiro para a fila
 * @param peca Peça a ser adicionada
 */
void enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila->pecas[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->quantidade++;
}

/**
 * Remove e retorna a peça da frente da fila
 * @param fila Ponteiro para a fila
 * @return Peça removida
 */
Peca desenfileirar(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->quantidade--;
    
    return peca;
}

/**
 * Adiciona uma peça no topo da pilha
 * @param pilha Ponteiro para a pilha
 * @param peca Peça a ser adicionada
 */
void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    
    pilha->topo = (pilha->topo + 1) % TAM_PILHA;
    pilha->pecas[pilha->topo] = peca;
    pilha->quantidade++;
}

/**
 * Remove e retorna a peça do topo da pilha
 * @param pilha Ponteiro para a pilha
 * @return Peça removida
 */
Peca desempilhar(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("Erro: Pilha vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = pilha->pecas[pilha->topo];
    pilha->topo = (pilha->topo - 1 + TAM_PILHA) % TAM_PILHA;
    pilha->quantidade--;
    
    return peca;
}

/**
 * Exibe o estado atual da fila e da pilha
 * @param fila Ponteiro para a fila
 * @param pilha Ponteiro para a pilha
 */
void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("\n=== ESTADO ATUAL ===\n");
    
    // Exibe a fila
    printf("Fila de pecas:\t");
    if (filaVazia(fila)) {
        printf "[Vazia]");
    } else {
        int i = fila->frente;
        int count = 0;
        while (count < fila->quantidade) {
            printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
            i = (i + 1) % TAM_FILA;
            count++;
        }
    }
    printf("\n");
    
    // Exibe a pilha
    printf("Pilha de reserva:\t(Topo -> base): ");
    if (pilhaVazia(pilha)) {
        printf("[Vazia]");
    } else {
        // Para exibir da base ao topo (mais intuitivo)
        Peca temp[TAM_PILHA];
        int idx = 0;
        
        // Copia as peças para um array temporário na ordem correta
        int pos = (pilha->topo - pilha->quantidade + 1 + TAM_PILHA) % TAM_PILHA;
        for (int i = 0; i < pilha->quantidade; i++) {
            temp[idx++] = pilha->pecas[pos];
            pos = (pos + 1) % TAM_PILHA;
        }
        
        // Exibe do topo para a base
        for (int i = idx - 1; i >= 0; i--) {
            printf("[%c %d] ", temp[i].nome, temp[i].id);
        }
    }
    printf("\n");
}

/**
 * Preenche a fila com peças até atingir a capacidade máxima
 * @param fila Ponteiro para a fila
 * @param proximoId Ponteiro para o próximo ID a ser usado
 */
void preencherFila(FilaCircular *fila, int *proximoId) {
    while (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca((*proximoId)++));
    }
}

/**
 * Joga a peça da frente da fila (remove) e gera uma nova
 * @param fila Ponteiro para a fila
 * @param proximoId Ponteiro para o próximo ID a ser usado
 */
void jogarPeca(FilaCircular *fila, int *proximoId) {
    if (filaVazia(fila)) {
        printf("Nao ha pecas na fila para jogar!\n");
        return;
    }
    
    Peca pecaJogada = desenfileirar(fila);
    printf("Peca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
    
    // Gera uma nova peça para manter a fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca((*proximoId)++));
    }
}

/**
 * Reserva a peça da frente da fila movendo-a para a pilha
 * @param fila Ponteiro para a fila
 * @param pilha Ponteiro para a pilha
 * @param proximoId Ponteiro para o próximo ID a ser usado
 */
void reservarPeca(FilaCircular *fila, Pilha *pilha, int *proximoId) {
    if (filaVazia(fila)) {
        printf("Nao ha pecas na fila para reservar!\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("Pilha de reserva cheia! Nao e possivel adicionar mais pecas.\n");
        return;
    }
    
    Peca pecaReservada = desenfileirar(fila);
    empilhar(pilha, pecaReservada);
    printf("Peca reservada: [%c %d]\n", pecaReservada.nome, pecaReservada.id);
    
    // Gera uma nova peça para manter a fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca((*proximoId)++));
    }
}

/**
 * Usa a peça do topo da pilha (remove)
 * @param pilha Ponteiro para a pilha
 */
void usarPecaReservada(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("Nao ha pecas na pilha de reserva!\n");
        return;
    }
    
    Peca pecaUsada = desempilhar(pilha);
    printf("Peca usada da reserva: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
}

/**
 * Troca a peça da frente da fila com a peça do topo da pilha
 * @param fila Ponteiro para a fila
 * @param pilha Ponteiro para a pilha
 */
void trocarPecaFrente(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Erro: E necessario ter pecas na fila E na pilha para trocar!\n");
        return;
    }
    
    // Remove as peças temporariamente
    Peca pecaFila = desenfileirar(fila);
    Peca pecaPilha = desempilhar(pilha);
    
    // Insere nas estruturas opostas
    enfileirar(fila, pecaPilha);
    empilhar(pilha, pecaFila);
    
    printf("Troca realizada: [%c %d] (fila) <-> [%c %d] (pilha)\n", 
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
}

/**
 * Troca os 3 primeiros da fila com as 3 peças da pilha
 * @param fila Ponteiro para a fila
 * @param pilha Ponteiro para a pilha
 */
void trocarMultipla(FilaCircular *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->quantidade < 3) {
        printf("Erro: E necessario ter pelo menos 3 pecas na fila E 3 na pilha!\n");
        return;
    }
    
    printf("Realizando troca multipla entre 3 pecas...\n");
    
    // Arrays temporários para armazenar as peças
    Peca tempFila[3], tempPilha[3];
    
    // Remove 3 peças da fila
    for (int i = 0; i < 3; i++) {
        tempFila[i] = desenfileirar(fila);
    }
    
    // Remove 3 peças da pilha
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = desempilhar(pilha);
    }
    
    // Insere as peças da pilha na fila
    for (int i = 0; i < 3; i++) {
        enfileirar(fila, tempPilha[i]);
    }
    
    // Insere as peças da fila na pilha
    for (int i = 0; i < 3; i++) {
        empilhar(pilha, tempFila[i]);
    }
    
    printf("Troca multipla concluida com sucesso!\n");
}