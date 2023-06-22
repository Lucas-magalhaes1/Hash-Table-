#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 53 

typedef struct Node {
    char nome[100];
    struct Node* anterior; 
    struct Node* proximo;
} Node;

unsigned int funcaoHash(const char* chave);
void inserir(const char* nome);
int busca(const char* nome);
int contarElementos(const unsigned int chave);
void contarElementosEnomes(const unsigned int chave, int* contar, char** nomes);
unsigned int removerNome(const char* nome);
Node* particao(Node* start, Node* end);
void quicksort(Node* start, Node* end);
void troca(Node* a, Node* b);
void print_tabela_hash();
void limpar_tabela_hash();
Node* tabela_hash[TABLE_SIZE];


unsigned int funcaoHash(const char* chave) {
    unsigned int hash = 0;
    unsigned int len = strlen(chave);

    for (unsigned int i = 0; i < len; i++) {
        hash = hash * 31 + chave[i];
    }

    return hash % TABLE_SIZE;
}


void inserir(const char* nome) {
    unsigned int chave = funcaoHash(nome);

    Node* novo_node = (Node*)malloc(sizeof(Node));
    strcpy(novo_node->nome, nome);
    novo_node->anterior = NULL;
    novo_node->proximo = NULL;

    if (tabela_hash[chave] == NULL) {
        tabela_hash[chave] = novo_node;
    } else {
        novo_node->proximo = tabela_hash[chave];
        tabela_hash[chave]->anterior = novo_node;
        tabela_hash[chave] = novo_node;
    }
}


int busca(const char* nome) {
    unsigned int chave = funcaoHash(nome);

    Node* atual = tabela_hash[chave];
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return 1;
        }
        atual = atual->proximo;
    }

    return 0;
}


int contarElementos(const unsigned int chave) {
    int contar = 0;
    Node* atual = tabela_hash[chave];
    while (atual != NULL) {
        contar++;
        atual = atual->proximo;
    }
    return contar;
}


void contarElementosEnomes(const unsigned int chave, int* contar, char** nomes) {
    *contar = 0;
    Node* atual = tabela_hash[chave];
    while (atual != NULL) {
        (*contar)++;
        atual = atual->proximo;
    }

    *nomes = (char**)malloc((*contar) * sizeof(char*));
    atual = tabela_hash[chave];
    int i = 0;
    while (atual != NULL) {
        (*nomes)[i] = strdup(atual->nome);
        atual = atual->proximo;
        i++;
    }
}


unsigned int removerNome(const char* nome) {
    unsigned int chave = funcaoHash(nome);

    Node* atual = tabela_hash[chave];
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (atual->anterior == NULL) {
                tabela_hash[chave] = atual->proximo;
                if (atual->proximo != NULL) {
                    atual->proximo->anterior = NULL;
                }
            } else {
                atual->anterior->proximo = atual->proximo;
                if (atual->proximo != NULL) {
                    atual->proximo->anterior = atual->anterior;
                }
            }
            free(atual);
            return chave;
        }
        atual = atual->proximo;
    }

    return -1; 
}


void troca(Node* a, Node* b) {
    char temp[100];
    strcpy(temp, a->nome);
    strcpy(a->nome, b->nome);
    strcpy(b->nome, temp);
}



Node* particao(Node* start, Node* end) {
    char pivot[100];
    strcpy(pivot, end->nome);

    Node* i = start->anterior;
    for (Node* j = start; j != end; j = j->proximo) {
        if (strcmp(j->nome, pivot) <= 0) {
            i = (i == NULL) ? start : i->proximo;
            if (i != j) {
                troca(i, j);
            }
        }
    }

    i = (i == NULL) ? start : i->proximo;
    if (i != end) {
        troca(i, end);
    }
    return i;
}


void quicksort(Node* start, Node* end) {
    if (start && end && start != end && start != end->proximo) {
        Node* pivot = particao(start, end);
        quicksort(start, pivot->anterior);
        quicksort(pivot->proximo, end);
    }
}


void sort_tabela_hash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* head = tabela_hash[i];

        
        Node* tail = NULL;
        while (head && head->proximo) {
            head = head->proximo;
        }
        tail = head;

        
        quicksort(tabela_hash[i], tail);
    }
}


void print_tabela_hash() {
    printf("Histograma:\n");
    printf("-----------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Chave [%d]: %d elementos\n", i, contarElementos(i));
    }
}


void limpar_tabela_hash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* atual = tabela_hash[i];
        while (atual != NULL) {
            Node* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        tabela_hash[i] = NULL;
    }
}



int comparar_nomes(const void* a, const void* b) {
    const char* nome1 = *(const char**)a;
    const char* nome2 = *(const char**)b;
    return strcasecmp(nome1, nome2);
}



void print_all_nomes_sorted() {
    const int MAX_NAMES = 100788; 
    char* all_nomes[MAX_NAMES]; 
    int contar = 0;

   
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* atual = tabela_hash[i];
        while (atual != NULL && contar < MAX_NAMES) {
            all_nomes[contar] = strdup(atual->nome);
            contar++;
            atual = atual->proximo;
        }
    }

    
    qsort(all_nomes, contar, sizeof(char*), comparar_nomes);

    printf("Nomes ordenados:\n");
    printf("----------------\n");
    for (int i = 0; i < contar; i++) {
        printf("%s\n", all_nomes[i]);
        free(all_nomes[i]); // Libera a memória alocada para cada nome
    }

  
    memset(all_nomes, 0, sizeof(all_nomes));
}



int main() {
  
    for (int i = 0; i < TABLE_SIZE; i++) {
        tabela_hash[i] = NULL;
    }
  
    FILE* file = fopen("nomes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char nome[100];
    while (fgets(nome, sizeof(nome), file)) {
        nome[strcspn(nome, "\r\n")] = '\0';
        inserir(nome);
    }

    fclose(file);

    int escolha;

    do {
        printf("Menu:\n");
        printf("1. Inserir nome\n");
        printf("2. Consultar nome\n");
        printf("3. Verificar quantidade de elementos por chave\n");
        printf("4. Remover nome\n");
        printf("5. Imprimir chaves\n");
        printf("6. Ordenar elementos\n");
        printf("7. Imprimir todos os nomes de todas as chaves \n"); 
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o nome a ser inserido: ");
                scanf(" %[^\n]", nome);
                inserir(nome);
                printf("Nome inserido com sucesso na chave %d.\n", funcaoHash(nome));
                break;
            case 2: {
                printf("Digite o nome a ser consultado: ");
                scanf(" %[^\n]", nome);
                int chave = funcaoHash(nome);
                int found = busca(nome);
                if (found) {
                    printf("Consulta: \"%s\" encontrado na chave %d.\n",     
 nome, chave);
                } else {
                    printf("Consulta: \"%s\" não encontrado.\n", nome);
                }
                break;
            }
            case 3: {
                printf("Digite a chave a ser verificada: ");
                scanf("%d", &escolha);
                int contar = contarElementos(escolha);
                printf("Elementos na chave %d:\n", escolha);
                Node* atual = tabela_hash[escolha];
                while (atual != NULL) {
                    printf("%s\n", atual->nome);
                    atual = atual->proximo;
                }
                printf("Chave [%d]: %d elementos\n", escolha, contar);
                break;
            }
            case 4: {
                printf("Digite o nome a ser removido: ");
                scanf(" %[^\n]", nome);
                unsigned int chave = removerNome(nome);
                if (chave == -1) {
                    printf("O nome \"%s\" não foi encontrado.\n", nome);
                } else {
                    printf("Nome \"%s\" removido com sucesso da chave %d.\n", nome, chave);
                }
                break;
            }
            case 5:
                print_tabela_hash();
                break;
            case 6: {
                sort_tabela_hash();
                printf("Elementos em cada chave ordenados.\n");
                break;
            }
            case 7:
                print_all_nomes_sorted();  
                break;
            case 8:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        printf("\n");
    } while (escolha != 8);

    limpar_tabela_hash();

    return 0;
}


