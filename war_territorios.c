#include <stdio.h>
#include <string.h>

// Definição da struct Territorio
// Esta estrutura armazena todas as informações relevantes sobre um território
struct Territorio {
    char nome[30];      // Nome do território (máximo 29 caracteres + '\0')
    char cor[10];       // Cor do exército que ocupa o território
    int tropas;         // Quantidade de tropas no território
};

int main() {
    // Declaração do vetor de structs para armazenar 5 territórios
    struct Territorio territorios[5];
    
    printf("=== SISTEMA DE CADASTRO DE TERRITORIOS - WAR ===\n\n");
    
    // Entrada de dados: cadastro dos 5 territórios
    // Utilizamos um loop for para percorrer todas as posições do vetor
    for(int i = 0; i < 5; i++) {
        printf("--- Cadastro do Territorio %d ---\n", i + 1);
        
        // Leitura do nome do território
        printf("Digite o nome do territorio: ");
        scanf("%29s", territorios[i].nome);
        
        // Leitura da cor do exército
        printf("Digite a cor do exercito: ");
        scanf("%9s", territorios[i].cor);
        
        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        printf("\n"); // Linha em branco para melhor organização
    }
    
    // Exibição dos dados cadastrados
    printf("=== TERRITORIOS CADASTRADOS ===\n\n");
    
    // Loop para exibir todos os territórios cadastrados
    for(int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do exercito: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("----------------------------\n");
    }
    
    return 0;
}