#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Componente
typedef struct {
    char nome[30];      // Nome do componente
    char tipo[20];      // Tipo do componente
    int prioridade;     // Prioridade (1-10)
} Componente;

// Variáveis globais para contagem de comparações
int comparacoes_bubble = 0;
int comparacoes_insertion = 0;
int comparacoes_selection = 0;
int comparacoes_busca = 0;

// Protótipos das funções
void bubbleSortNome(Componente componentes[], int n);
void insertionSortTipo(Componente componentes[], int n);
void selectionSortPrioridade(Componente componentes[], int n);
int buscaBinariaPorNome(Componente componentes[], int n, char nome[]);
void mostrarComponentes(Componente componentes[], int n);
void medirTempo(void (*algoritmo)(Componente[], int), Componente componentes[], int n, char nomeAlgoritmo[]);
void cadastrarComponentes(Componente componentes[], int *quantidade);
void limparBuffer();

int main() {
    Componente componentes[20];
    int quantidade = 0;
    int opcao;
    char nomeBusca[30];
    int ordenadoPorNome = 0; // Flag para verificar se está ordenado por nome
    
    printf("=== SISTEMA DE MONTAGEM DA TORRE DE RESGATE ===\n\n");
    
    // Cadastro inicial de componentes
    cadastrarComponentes(componentes, &quantidade);
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Mostrar componentes\n");
        printf("2. Ordenar por Nome (Bubble Sort)\n");
        printf("3. Ordenar por Tipo (Insertion Sort)\n");
        printf("4. Ordenar por Prioridade (Selection Sort)\n");
        printf("5. Buscar componente por nome (Busca Binaria)\n");
        printf("6. Medir desempenho de todos os algoritmos\n");
        printf("7. Cadastrar novos componentes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1:
                printf("\n=== COMPONENTES CADASTRADOS ===\n");
                mostrarComponentes(componentes, quantidade);
                break;
                
            case 2:
                printf("\n=== ORDENANDO POR NOME (BUBBLE SORT) ===\n");
                comparacoes_bubble = 0;
                medirTempo(bubbleSortNome, componentes, quantidade, "Bubble Sort");
                ordenadoPorNome = 1;
                printf("\nComponentes ordenados por nome!\n");
                mostrarComponentes(componentes, quantidade);
                break;
                
            case 3:
                printf("\n=== ORDENANDO POR TIPO (INSERTION SORT) ===\n");
                comparacoes_insertion = 0;
                medirTempo(insertionSortTipo, componentes, quantidade, "Insertion Sort");
                printf("\nComponentes ordenados por tipo!\n");
                mostrarComponentes(componentes, quantidade);
                break;
                
            case 4:
                printf("\n=== ORDENANDO POR PRIORIDADE (SELECTION SORT) ===\n");
                comparacoes_selection = 0;
                medirTempo(selectionSortPrioridade, componentes, quantidade, "Selection Sort");
                printf("\nComponentes ordenados por prioridade!\n");
                mostrarComponentes(componentes, quantidade);
                break;
                
            case 5:
                if (!ordenadoPorNome) {
                    printf("\nAVISO: Para busca binaria, os componentes devem estar ordenados por nome!\n");
                    printf("Execute primeiro a opcao 2 (Ordenar por Nome).\n");
                    break;
                }
                printf("\n=== BUSCA BINARIA POR NOME ===\n");
                printf("Digite o nome do componente a buscar: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove quebra de linha
                
                comparacoes_busca = 0;
                clock_t inicio_busca = clock();
                int posicao = buscaBinariaPorNome(componentes, quantidade, nomeBusca);
                clock_t fim_busca = clock();
                double tempo_busca = ((double)(fim_busca - inicio_busca)) / CLOCKS_PER_SEC;
                
                if (posicao != -1) {
                    printf("\n>>> COMPONENTE ENCONTRADO! <<<\n");
                    printf("Posicao: %d\n", posicao + 1);
                    printf("Nome: %s\n", componentes[posicao].nome);
                    printf("Tipo: %s\n", componentes[posicao].tipo);
                    printf("Prioridade: %d\n", componentes[posicao].prioridade);
                    
                    if (strcmp(nomeBusca, "chip central") == 0) {
                        printf("\n>>> TORRE DE RESGATE ATIVADA! <<<\n");
                        printf("Componente-chave encontrado. Iniciando sequencia de fuga...\n");
                    }
                } else {
                    printf("\nComponente '%s' nao encontrado!\n", nomeBusca);
                }
                printf("Comparacoes na busca: %d\n", comparacoes_busca);
                printf("Tempo de busca: %.6f segundos\n", tempo_busca);
                break;
                
            case 6:
                printf("\n=== ANALISE DE DESEMPENHO ===\n");
                
                // Cria cópia do array para testes
                Componente copia1[20], copia2[20], copia3[20];
                memcpy(copia1, componentes, sizeof(Componente) * quantidade);
                memcpy(copia2, componentes, sizeof(Componente) * quantidade);
                memcpy(copia3, componentes, sizeof(Componente) * quantidade);
                
                printf("\n1. Bubble Sort (Nome):\n");
                comparacoes_bubble = 0;
                medirTempo(bubbleSortNome, copia1, quantidade, "Bubble Sort");
                
                printf("\n2. Insertion Sort (Tipo):\n");
                comparacoes_insertion = 0;
                medirTempo(insertionSortTipo, copia2, quantidade, "Insertion Sort");
                
                printf("\n3. Selection Sort (Prioridade):\n");
                comparacoes_selection = 0;
                medirTempo(selectionSortPrioridade, copia3, quantidade, "Selection Sort");
                
                printf("\n=== RESUMO DAS COMPARACOES ===\n");
                printf("Bubble Sort: %d comparacoes\n", comparacoes_bubble);
                printf("Insertion Sort: %d comparacoes\n", comparacoes_insertion);
                printf("Selection Sort: %d comparacoes\n", comparacoes_selection);
                break;
                
            case 7:
                cadastrarComponentes(componentes, &quantidade);
                ordenadoPorNome = 0; // Reseta flag ao adicionar novos componentes
                break;
                
            case 0:
                printf("\nSaindo do sistema...\n");
                printf("Torre de resgate montada com sucesso! Boa sorte na fuga!\n");
                break;
                
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}

/**
 * Cadastra componentes manualmente
 * @param componentes Array de componentes
 * @param quantidade Ponteiro para a quantidade de componentes
 */
void cadastrarComponentes(Componente componentes[], int *quantidade) {
    printf("\n=== CADASTRO DE COMPONENTES ===\n");
    printf("Quantos componentes deseja cadastrar? (max 20): ");
    scanf("%d", quantidade);
    limparBuffer();
    
    if (*quantidade > 20) {
        printf("Quantidade reduzida para 20 (limite maximo).\n");
        *quantidade = 20;
    }
    
    for (int i = 0; i < *quantidade; i++) {
        printf("\n--- Componente %d ---\n", i + 1);
        
        printf("Nome: ");
        fgets(componentes[i].nome, 30, stdin);
        componentes[i].nome[strcspn(componentes[i].nome, "\n")] = 0;
        
        printf("Tipo (controle/suporte/propulsao/energia): ");
        fgets(componentes[i].tipo, 20, stdin);
        componentes[i].tipo[strcspn(componentes[i].tipo, "\n")] = 0;
        
        printf("Prioridade (1-10): ");
        scanf("%d", &componentes[i].prioridade);
        limparBuffer();
        
        // Validação da prioridade
        if (componentes[i].prioridade < 1) componentes[i].prioridade = 1;
        if (componentes[i].prioridade > 10) componentes[i].prioridade = 10;
    }
    
    printf("\nCadastro concluido! %d componentes registrados.\n", *quantidade);
}

/**
 * Bubble Sort para ordenar por nome
 * @param componentes Array de componentes
 * @param n Quantidade de componentes
 */
void bubbleSortNome(Componente componentes[], int n) {
    int i, j;
    Componente temp;
    
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            comparacoes_bubble++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                // Troca os elementos
                temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
}

/**
 * Insertion Sort para ordenar por tipo
 * @param componentes Array de componentes
 * @param n Quantidade de componentes
 */
void insertionSortTipo(Componente componentes[], int n) {
    int i, j;
    Componente chave;
    
    for (i = 1; i < n; i++) {
        chave = componentes[i];
        j = i - 1;
        
        // Move elementos maiores que a chave para a direita
        while (j >= 0) {
            comparacoes_insertion++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }
}

/**
 * Selection Sort para ordenar por prioridade
 * @param componentes Array de componentes
 * @param n Quantidade de componentes
 */
void selectionSortPrioridade(Componente componentes[], int n) {
    int i, j, min_idx;
    Componente temp;
    
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            comparacoes_selection++;
            if (componentes[j].prioridade < componentes[min_idx].prioridade) {
                min_idx = j;
            }
        }
        // Troca o elemento mínimo com o primeiro elemento
        temp = componentes[min_idx];
        componentes[min_idx] = componentes[i];
        componentes[i] = temp;
    }
}

/**
 * Busca binária por nome (requer array ordenado por nome)
 * @param componentes Array de componentes ordenado por nome
 * @param n Quantidade de componentes
 * @param nome Nome do componente a buscar
 * @return Índice do componente ou -1 se não encontrado
 */
int buscaBinariaPorNome(Componente componentes[], int n, char nome[]) {
    int esquerda = 0;
    int direita = n - 1;
    
    while (esquerda <= direita) {
        comparacoes_busca++;
        int meio = esquerda + (direita - esquerda) / 2;
        
        int comparacao = strcmp(componentes[meio].nome, nome);
        
        if (comparacao == 0) {
            return meio; // Elemento encontrado
        } else if (comparacao < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    return -1; // Elemento não encontrado
}

/**
 * Exibe todos os componentes formatados
 * @param componentes Array de componentes
 * @param n Quantidade de componentes
 */
void mostrarComponentes(Componente componentes[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    
    printf("\n%-4s %-30s %-20s %-10s\n", "No.", "Nome", "Tipo", "Prioridade");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-4d %-30s %-20s %-10d\n", 
               i + 1, 
               componentes[i].nome, 
               componentes[i].tipo, 
               componentes[i].prioridade);
    }
}

/**
 * Mede o tempo de execução de um algoritmo de ordenação
 * @param algoritmo Ponteiro para a função do algoritmo
 * @param componentes Array de componentes
 * @param n Quantidade de componentes
 * @param nomeAlgoritmo Nome do algoritmo para exibição
 */
void medirTempo(void (*algoritmo)(Componente[], int), Componente componentes[], int n, char nomeAlgoritmo[]) {
    clock_t inicio = clock();
    algoritmo(componentes, n);
    clock_t fim = clock();
    
    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Tempo de execucao: %.6f segundos\n", tempo_execucao);
}

/**
 * Limpa o buffer de entrada
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}