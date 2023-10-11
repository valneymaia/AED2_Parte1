#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAVES 1000

typedef struct passageiro {
    char nome[50];
    int idade;
    char planeta_origem[50];
    int id;
} Passageiro;

typedef struct recurso {
    char nome[50];
    int quantidade;
} Recurso;

typedef struct espaco_nave {
    int prioridade;
    Passageiro *passageiros;
    int num_passageiros;
    Recurso *recursos;
    int num_recursos;
} EspacoNave;

typedef struct fila_prioridade {
    EspacoNave *naves[MAX_NAVES];
    int tamanho;
} FilaPrioridade;

FilaPrioridade *criar_fila() {
    FilaPrioridade *fila = malloc(sizeof(FilaPrioridade));
    fila->tamanho = 0;
    return fila;
}

void trocar(EspacoNave **a, EspacoNave **b) {
    EspacoNave *temp = *a;
    *a = *b;
    *b = temp;
}

void subir_na_fila(FilaPrioridade *fila, int i) {
    while (i > 0 && fila->naves[i]->prioridade > fila->naves[(i - 1) / 2]->prioridade) {
        trocar(&fila->naves[i], &fila->naves[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void descer_na_fila(FilaPrioridade *fila, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < fila->tamanho && fila->naves[esquerda]->prioridade > fila->naves[maior]->prioridade) {
        maior = esquerda;
    }

    if (direita < fila->tamanho && fila->naves[direita]->prioridade > fila->naves[maior]->prioridade) {
        maior = direita;
    }

    if (maior != i) {
        trocar(&fila->naves[i], &fila->naves[maior]);
        descer_na_fila(fila, maior);
    }
}

void inserir_na_fila(FilaPrioridade *fila, EspacoNave *nave) {
    if (fila->tamanho == MAX_NAVES) {
        printf("Erro: a fila está cheia.\n");
        return;
    }

    fila->naves[fila->tamanho] = nave;
    subir_na_fila(fila, fila->tamanho);
    fila->tamanho++;
}

EspacoNave *remover_da_fila(FilaPrioridade *fila) {
    if (fila->tamanho == 0) {
        return NULL;
    }

    EspacoNave *nave = fila->naves[0];
    fila->tamanho--;
    fila->naves[0] = fila->naves[fila->tamanho];
    descer_na_fila(fila, 0);
    return nave;
}

void imprimir_fila(FilaPrioridade *fila) {
    for (int i = 0; i < fila->tamanho; i++) {
        printf("Prioridade: %d\n", fila->naves[i]->prioridade);
        printf("Passageiros:\n");
        for (int j = 0; j < fila->naves[i]->num_passageiros; j++) {
            printf("  Nome: %s, Idade: %d, Planeta de origem: %s, ID: %d\n", 
                fila->naves[i]->passageiros[j].nome, 
                fila->naves[i]->passageiros[j].idade, 
                fila->naves[i]->passageiros[j].planeta_origem, 
                fila->naves[i]->passageiros[j].id
            );
        }
        printf("Recursos:\n");
        for (int j = 0; j < fila->naves[i]->num_recursos; j++) {
            printf("  Nome: %s, Quantidade: %d\n", 
                fila->naves[i]->recursos[j].nome, 
                fila->naves[i]->recursos[j].quantidade
            );
        }
        printf("\n");
    }
}


void atribuir_nova_prioridade(EspacoNave *nave) {
    if ((double)rand() / RAND_MAX < 0.1) {
        nave->prioridade = rand() % 100; // Novo valor de prioridade aleatório entre 0 e 99
    }
}

int main() {
    srand(time(NULL));

    FilaPrioridade *fila = criar_fila();

    // Ler os dados iniciais do arquivo
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while (!feof(arquivo)) {
         EspacoNave *nave = malloc(sizeof(EspacoNave));

    // Ler a prioridade da espaçonave
    if (fscanf(arquivo, "%d", &nave->prioridade) != 1) {
        free(nave);
        break;
    }

    // Ler o número de passageiros
    if (fscanf(arquivo, "%d", &nave->num_passageiros) != 1) {
        free(nave);
        break;
    }
    
    nave->passageiros = malloc(nave->num_passageiros * sizeof(Passageiro));
    for (int i = 0; i < nave->num_passageiros; i++) {
        // Ler as informações do passageiro
        if (fscanf(arquivo, "%s %d %s %d", 
            nave->passageiros[i].nome, 
            &nave->passageiros[i].idade, 
            nave->passageiros[i].planeta_origem, 
            &nave->passageiros[i].id) != 4) {
            free(nave->passageiros);
            free(nave);
            break;
        }
    }

    // Ler o número de recursos transportados
    if (fscanf(arquivo, "%d", &nave->num_recursos) != 1) {
        free(nave->passageiros);
        free(nave);
        break;
    }
    
    nave->recursos = malloc(nave->num_recursos * sizeof(Recurso));
    for (int i = 0; i < nave->num_recursos; i++) {
        // Ler as informações do recurso
        if (fscanf(arquivo, "%s %d", 
            nave->recursos[i].nome, 
            &nave->recursos[i].quantidade) != 2) {
            free(nave->recursos);
            free(nave->passageiros);
            free(nave);
            break;
        }
    }

        atribuir_nova_prioridade(nave);
        inserir_na_fila(fila, nave);
    }

    fclose(arquivo);

    // Menu para o usuário inserir ou remover espaçonaves da fila
    int opcao;
    do {
        printf(" .  *  . . .  .");
        printf("Menu: * . .  * . *  . * . .  *\n");
        printf("1. Inserir espaconave na fila\n");
        printf("2. Remover e imprimir espaconave com maior prioridade\n");
        printf("3. Imprimir todas as espaconaves na fila\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        printf("\n* .  *  . . .    .");
        printf(" * . .  * .  *  . * . .  *\n");
        scanf("%d", &opcao);

        if (opcao == 1) {
            EspacoNave *nave = malloc(sizeof(EspacoNave));

            printf("Digite a prioridade da espaçonave: ");
            scanf("%d", &nave->prioridade);

            printf("Digite o numero de passageiros: ");
            scanf("%d", &nave->num_passageiros);
            nave->passageiros = malloc(nave->num_passageiros * sizeof(Passageiro));
            for (int i = 0; i < nave->num_passageiros; i++) {
                printf("Digite o nome do passageiro %d: ", i + 1);
                scanf("%s", nave->passageiros[i].nome);
                printf("Digite a idade do passageiro %d: ", i + 1);
                scanf("%d", &nave->passageiros[i].idade);
                printf("Digite o planeta de origem do passageiro %d: ", i + 1);
                scanf("%s", nave->passageiros[i].planeta_origem);
                printf("Digite o ID do passageiro %d: ", i + 1);
                scanf("%d", &nave->passageiros[i].id);
            }

            printf("Digite o numero de recursos transportados: ");
            scanf("%d", &nave->num_recursos);
            nave->recursos = malloc(nave->num_recursos * sizeof(Recurso));
            for (int i = 0; i < nave->num_recursos; i++) {
                printf("Digite o nome do recurso %d: ", i + 1);
                scanf("%s", nave->recursos[i].nome);
                printf("Digite a quantidade do recurso %d: ", i + 1);
                scanf("%d", &nave->recursos[i].quantidade);
            }
            atribuir_nova_prioridade(nave);
            inserir_na_fila(fila, nave);
        } else if (opcao == 2) {
            EspacoNave *nave = remover_da_fila(fila);
            if (nave == NULL) {
                printf("A fila esta vazia.\n");
            } else {
                printf("Espaçonave removida:\n");
                printf("Prioridade: %d\n", nave->prioridade);
                printf("Passageiros:\n");
                for (int i = 0; i < nave->num_passageiros; i++) {
                    printf("  Nome: %s, Idade: %d, Planeta de origem: %s, ID: %d\n", 
                        nave->passageiros[i].nome, 
                        nave->passageiros[i].idade, 
                        nave->passageiros[i].planeta_origem, 
                        nave->passageiros[i].id
                    );
                }
                printf("Recursos:\n");
                for (int i = 0; i < nave->num_recursos; i++) {
                    printf("  Nome: %s, Quantidade: %d\n", 
                        nave->recursos[i].nome, 
                        nave->recursos[i].quantidade
                    );
                }
            }
        } else if (opcao == 3) {
            imprimir_fila(fila);
        }
    } while (opcao != 4);

    return 0;
}
