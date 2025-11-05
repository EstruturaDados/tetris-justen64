#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Configurações do jogo
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;  // Tipo: 'I', 'O', 'T', 'L'
    int id;     // ID único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
    int proximoId;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void push(Pilha *pilha, Peca peca);
Peca pop(Pilha *p);
Peca gerarPeca(Fila *f);
void manterFilaCheia(Fila *f);
void trocarUnica(Fila *f, Pilha *p);
void trocarMultipla(Fila *f, Pilha *p);
void exibirEstado(Fila *f, Pilha *p);
void exibirMenu();

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;

    // Inicializa gerador aleatório
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    manterFilaCheia(&fila);

    do {
        printf("\n");
        exibirEstado(&fila, &pilha);
        exibirMenu();

        printf("Opcao escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça da frente da fila
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf(">>> Voce jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                    manterFilaCheia(&fila);
                } else {
                    printf(">>> A fila esta vazia! Nao ha peca para jogar.\n");
                }
                break;

            case 2: // Enviar peça da fila para a pilha de reserva
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf(">>> Peca [%c %d] reservada!\n", reservada.nome, reservada.id);
                    manterFilaCheia(&fila);
                } else if (pilhaCheia(&pilha)) {
                    printf(">>> Pilha de reserva cheia! Nao foi possivel reservar.\n");
                } else {
                    printf(">>> A fila esta vazia! Nao ha peca para reservar.\n");
                }
                break;

            case 3: // Usar peça da pilha de reserva
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf(">>> Voce usou a peca reservada [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf(">>> Nao ha pecas na reserva!\n");
                }
                break;

            case 4: // Trocar peça da frente da fila com o topo da pilha
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                    trocarUnica(&fila, &pilha);
                    printf(">>> Troca unica realizada!\n");
                } else {
                    printf(">>> Nao e possivel trocar: verifique se ha pecas na fila e na pilha.\n");
                }
                break;

            case 5: // Trocar os 3 primeiros da fila com as 3 da pilha
                if (fila.tamanho >= 3 && pilha.topo >= 2) {
                    trocarMultipla(&fila, &pilha);
                    printf("Acao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
                } else {
                    printf(">>> Nao e possivel trocar multipla: precisa de pelo menos 3 pecas em cada estrutura.\n");
                }
                break;

            case 0: // Sair
                printf(">>> Encerrando o Tetris Stack. Ate a proxima!\n");
                break;

            default:
                printf(">>> Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Inicializa a fila circular
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
    f->proximoId = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona peça ao final da fila
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

// Remove peça da frente da fila
Peca dequeue(Fila *f) {
    Peca vazia = {' ', -1};
    if (filaVazia(f)) return vazia;
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->tamanho--;
    return removida;
}

// Adiciona peça ao topo da pilha

void push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}


// Remove peça do topo da pilha
Peca pop(Pilha *p) {
    Peca vazia = {' ', -1};
    if (pilhaVazia(p)) return vazia;
    Peca removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// Gera uma nova peça aleatória
Peca gerarPeca(Fila *f) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = f->proximoId++;
    return nova;
}

// Mantém a fila cheia adicionando novas peças
void manterFilaCheia(Fila *f) {
    while (!filaCheia(f)) {
        enqueue(f, gerarPeca(f));
    }
}

// Troca a peça da frente da fila com o topo da pilha
void trocarUnica(Fila *f, Pilha *p) {
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
}

// Realiza a troca múltipla entre os 3 primeiros da fila e os 3 da pilha
void trocarMultipla(Fila *f, Pilha *p) {
    // Índices dos 3 primeiros da fila
    int q1_idx = f->frente;
    int q2_idx = (q1_idx + 1) % TAMANHO_FILA;
    int q3_idx = (q2_idx + 1) % TAMANHO_FILA;

    // Peças da fila
    Peca q1 = f->itens[q1_idx];
    Peca q2 = f->itens[q2_idx];
    Peca q3 = f->itens[q3_idx];

    // Peças da pilha (0=base, 1=meio, 2=topo)
    Peca s_base = p->itens[0];
    Peca s_mid = p->itens[1];
    Peca s_top = p->itens[2];

    // Coloca peças da pilha na fila (top -> mid -> base)
    f->itens[q1_idx] = s_top;
    f->itens[q2_idx] = s_mid;
    f->itens[q3_idx] = s_base;

    // Coloca peças da fila na pilha (q3 no topo, q2 meio, q1 base)
    p->itens[2] = q3;
    p->itens[1] = q2;
    p->itens[0] = q1;
}

// Exibe o estado atual
void exibirEstado(Fila *f, Pilha *p) {
    printf("Estado atual:\n\n");

    // Exibe fila
    printf("Fila de pecas");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % TAMANHO_FILA;
        printf("[%c %d]", f->itens[idx].nome, f->itens[idx].id);
    }

    // Exibe pilha
    printf(" Pilha de reserva(Topo -> base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int j = p->topo; j >= 0; j--) {
            printf("[%c %d] ", p->itens[j].nome, p->itens[j].id);
        }
    }
    printf("\n");
    printf("Tabela: Visualizacao atual da fila de pecas e da pilha de reserva.\n");
    printf("Curadoria de TI.\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n\n");
    printf("Codigo\tAcao\n");
    printf("1\tJogar peca da frente da fila\n");
    printf("2\tEnviar peca da fila para a pilha de reserva\n");
    printf("3\tUsar peca da pilha de reserva\n");
    printf("4\tTrocar peca da frente da fila com o topo da pilha\n");
    printf("5\tTrocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0\tSair\n");
    printf("Tabela: Comandos para movimentar pecas entre a fila e a pilha de reserva.\n");
    printf("Curadoria de TI.\n\n");
}
