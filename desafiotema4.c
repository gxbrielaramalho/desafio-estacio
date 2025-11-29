#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para um cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[30];              // Nome do cômodo
    char pista[50];             // Pista encontrada no cômodo
    struct Sala* esquerda;      // Ponteiro para cômodo à esquerda
    struct Sala* direita;       // Ponteiro para cômodo à direita
} Sala;

// Definição da estrutura para uma pista (Árvore BST)
typedef struct Pista {
    char descricao[50];         // Descrição da pista
    struct Pista* esquerda;     // Ponteiro para pista menor
    struct Pista* direita;      // Ponteiro para pista maior
} Pista;

// Definição da estrutura para a tabela hash (Pista -> Suspeito)
typedef struct EntradaHash {
    char pista[50];             // Chave: descrição da pista
    char suspeito[30];          // Valor: nome do suspeito
    struct EntradaHash* proximo; // Encadeamento para colisões
} EntradaHash;

#define TAMANHO_HASH 10         // Tamanho da tabela hash

// Protótipos das funções
Sala* criarSala(const char* nome, const char* pista);
void explorarSalas(Sala* raiz);
Pista* inserirPista(Pista* raiz, const char* pista);
void adicionarPista(Pista** raizPistas, const char* pista);
void inserirNaHash(EntradaHash** tabela, const char* pista, const char* suspeito);
char* encontrarSuspeito(EntradaHash** tabela, const char* pista);
void verificarSuspeitoFinal(Pista* raizPistas, EntradaHash** tabela);
void liberarArvoreSalas(Sala* raiz);
void liberarArvorePistas(Pista* raiz);
void liberarTabelaHash(EntradaHash** tabela);
int hash(const char* chave);
void listarPistas(Pista* raiz);

// Variáveis globais para controle do jogo
Pista* pistasColetadas = NULL;  // Árvore BST de pistas coletadas

int main() {
    printf("=== DETECTIVE QUEST - INVESTIGACAO FINAL ===\n\n");
    
    // Criação manual do mapa da mansão (Árvore Binária)
    Sala* entrada = criarSala("Entrada", "Mancha de lama no tapete");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho quebrado");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto");
    Sala* quarto = criarSala("Quarto Principal", "Relogio parado as 21:47");
    Sala* escritorio = criarSala("Escritorio", "Documento rasgado");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra molhada");
    
    // Montagem da estrutura da mansão
    entrada->esquerda = salaEstar;
    entrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quarto;
    cozinha->esquerda = escritorio;
    cozinha->direita = jardim;
    
    // Inicialização da tabela hash
    EntradaHash* tabelaHash[TAMANHO_HASH] = {NULL};
    
    // População da tabela hash com associações pista->suspeito
    inserirNaHash(tabelaHash, "Mancha de lama no tapete", "Jardineiro");
    inserirNaHash(tabelaHash, "Copo de vinho quebrado", "Mordomo");
    inserirNaHash(tabelaHash, "Faca desaparecida do bloco", "Chef");
    inserirNaHash(tabelaHash, "Livro sobre venenos aberto", "Bibliotecario");
    inserirNaHash(tabelaHash, "Relogio parado as 21:47", "Herdeiro");
    inserirNaHash(tabelaHash, "Documento rasgado", "Socio");
    inserirNaHash(tabelaHash, "Pegadas na terra molhada", "Jardineiro");
    
    // Exploração interativa da mansão
    printf("Voce esta investigando o misterioso caso da Mansao Blackwood.\n");
    printf("Explore os comodos para coletar pistas e descubra o culpado!\n\n");
    
    explorarSalas(entrada);
    
    // Fase final - Acusação
    printf("\n=== FASE FINAL - ACUSACAO ===\n");
    verificarSuspeitoFinal(pistasColetadas, tabelaHash);
    
    // Liberação de memória
    liberarArvoreSalas(entrada);
    liberarArvorePistas(pistasColetadas);
    liberarTabelaHash(tabelaHash);
    
    printf("\nInvestigacao encerrada. Obrigado por jogar Detective Quest!\n");
    return 0;
}

/**
 * Cria dinamicamente um novo cômodo da mansão
 * @param nome Nome do cômodo
 * @param pista Pista associada ao cômodo
 * @return Ponteiro para a sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * Navega pela árvore de salas de forma interativa
 * Coleta pistas e as armazena na BST de pistas
 * @param raiz Nó atual da árvore de salas
 */
void explorarSalas(Sala* raiz) {
    if (raiz == NULL) return;
    
    char comando;
    int sair = 0;
    Sala* atual = raiz;
    
    while (!sair) {
        printf("\nVoce esta no: %s\n", atual->nome);
        
        // Verifica se há pista no cômodo e se ainda não foi coletada
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            adicionarPista(&pistasColetadas, atual->pista);
            printf(">>> Pista adicionada ao seu caderno! <<<\n");
            strcpy(atual->pista, ""); // Remove a pista para não coletar novamente
        } else {
            printf("Nenhuma pista nova encontrada aqui.\n");
        }
        
        // Opções de navegação
        printf("\nOpcoes de exploracao:\n");
        if (atual->esquerda != NULL) 
            printf("(e) Ir para %s (esquerda)\n", atual->esquerda->nome);
        if (atual->direita != NULL) 
            printf("(d) Ir para %s (direita)\n", atual->direita->nome);
        printf("(s) Sair da exploracao\n");
        printf("Escolha: ");
        
        scanf(" %c", &comando);
        
        switch (comando) {
            case 'e':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Nao ha comodo a esquerda!\n");
                }
                break;
            case 'd':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Nao ha comodo a direita!\n");
                }
                break;
            case 's':
                sair = 1;
                printf("Saindo da exploracao...\n");
                break;
            default:
                printf("Comando invalido! Use 'e', 'd' ou 's'.\n");
        }
    }
}

/**
 * Insere uma pista na árvore BST de forma ordenada
 * @param raiz Raiz da árvore de pistas
 * @param pista Descrição da pista a ser inserida
 * @return Nova raiz da árvore
 */
Pista* inserirPista(Pista* raiz, const char* pista) {
    if (raiz == NULL) {
        Pista* novaPista = (Pista*)malloc(sizeof(Pista));
        if (novaPista == NULL) {
            printf("Erro de alocacao de memoria!\n");
            exit(1);
        }
        strcpy(novaPista->descricao, pista);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    // Comparação lexicográfica para ordenação
    int comparacao = strcmp(pista, raiz->descricao);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se for igual, não insere duplicata
    
    return raiz;
}

/**
 * Adiciona uma pista à árvore BST (wrapper function)
 * @param raizPistas Ponteiro para a raiz da árvore de pistas
 * @param pista Pista a ser adicionada
 */
void adicionarPista(Pista** raizPistas, const char* pista) {
    *raizPistas = inserirPista(*raizPistas, pista);
}

/**
 * Função hash para mapear strings em índices da tabela
 * @param chave String a ser hasheada
 * @return Índice na tabela hash
 */
int hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * Insere uma associação pista-suspeito na tabela hash
 * @param tabela Tabela hash
 * @param pista Chave: descrição da pista
 * @param suspeito Valor: nome do suspeito
 */
void inserirNaHash(EntradaHash** tabela, const char* pista, const char* suspeito) {
    int indice = hash(pista);
    
    EntradaHash* novaEntrada = (EntradaHash*)malloc(sizeof(EntradaHash));
    if (novaEntrada == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    
    strcpy(novaEntrada->pista, pista);
    strcpy(novaEntrada->suspeito, suspeito);
    novaEntrada->proximo = tabela[indice];
    tabela[indice] = novaEntrada;
}

/**
 * Busca um suspeito associado a uma pista na tabela hash
 * @param tabela Tabela hash
 * @param pista Pista a ser buscada
 * @return Nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(EntradaHash** tabela, const char* pista) {
    int indice = hash(pista);
    EntradaHash* atual = tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * Lista todas as pistas coletadas em ordem
 * @param raiz Raiz da árvore de pistas
 */
void listarPistas(Pista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("- %s\n", raiz->descricao);
        listarPistas(raiz->direita);
    }
}

/**
 * Conduz a fase final de acusação do jogo
 * Verifica se há evidências suficientes para sustentar a acusação
 * @param raizPistas Raiz da árvore de pistas coletadas
 * @param tabela Tabela hash de associações pista-suspeito
 */
void verificarSuspeitoFinal(Pista* raizPistas, EntradaHash** tabela) {
    printf("\n=== SUAS PISTAS COLETADAS ===\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista coletada!\n");
        return;
    }
    listarPistas(raizPistas);
    
    // Contagem de pistas por suspeito
    int contagemJardineiro = 0;
    int contagemMordomo = 0;
    int contagemChef = 0;
    int contagemBibliotecario = 0;
    int contagemHerdeiro = 0;
    int contagemSocio = 0;
    
    // Função recursiva para contar pistas por suspeito
    void contarPistas(Pista* pista) {
        if (pista == NULL) return;
        
        char* suspeito = encontrarSuspeito(tabela, pista->descricao);
        if (suspeito != NULL) {
            if (strcmp(suspeito, "Jardineiro") == 0) contagemJardineiro++;
            else if (strcmp(suspeito, "Mordomo") == 0) contagemMordomo++;
            else if (strcmp(suspeito, "Chef") == 0) contagemChef++;
            else if (strcmp(suspeito, "Bibliotecario") == 0) contagemBibliotecario++;
            else if (strcmp(suspeito, "Herdeiro") == 0) contagemHerdeiro++;
            else if (strcmp(suspeito, "Socio") == 0) contagemSocio++;
        }
        
        contarPistas(pista->esquerda);
        contarPistas(pista->direita);
    }
    
    contarPistas(raizPistas);
    
    printf("\n=== RESUMO DAS EVIDENCIAS ===\n");
    printf("Jardineiro: %d pistas\n", contagemJardineiro);
    printf("Mordomo: %d pistas\n", contagemMordomo);
    printf("Chef: %d pistas\n", contagemChef);
    printf("Bibliotecario: %d pistas\n", contagemBibliotecario);
    printf("Herdeiro: %d pistas\n", contagemHerdeiro);
    printf("Socio: %d pistas\n", contagemSocio);
    
    // Fase de acusação
    char acusado[30];
    printf("\nQuem voce acusa pelo crime? ");
    scanf("%29s", acusado);
    
    int pistasDoAcusado = 0;
    if (strcmp(acusado, "Jardineiro") == 0) pistasDoAcusado = contagemJardineiro;
    else if (strcmp(acusado, "Mordomo") == 0) pistasDoAcusado = contagemMordomo;
    else if (strcmp(acusado, "Chef") == 0) pistasDoAcusado = contagemChef;
    else if (strcmp(acusado, "Bibliotecario") == 0) pistasDoAcusado = contagemBibliotecario;
    else if (strcmp(acusado, "Herdeiro") == 0) pistasDoAcusado = contagemHerdeiro;
    else if (strcmp(acusado, "Socio") == 0) pistasDoAcusado = contagemSocio;
    
    printf("\n=== VERDICT FINAL ===\n");
    if (pistasDoAcusado >= 2) {
        printf("PARABENS! Sua acusacao contra %s foi sustentada!\n", acusado);
        printf("Voce reuniu %d pistas incriminadoras. Caso resolvido!\n", pistasDoAcusado);
    } else {
        printf("INSUFICIENTE! A acusacao contra %s nao pode ser sustentada.\n", acusado);
        printf("Apenas %d pista(s) apontam para ele. Continue investigando!\n", pistasDoAcusado);
    }
}

// Funções de liberação de memória
void liberarArvoreSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvoreSalas(raiz->esquerda);
        liberarArvoreSalas(raiz->direita);
        free(raiz);
    }
}

void liberarArvorePistas(Pista* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

void liberarTabelaHash(EntradaHash** tabela) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        EntradaHash* atual = tabela[i];
        while (atual != NULL) {
            EntradaHash* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}