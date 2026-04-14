/*
 * DCC 403 – Seminário: Programação Concorrente com Threads
 * Tema 5: Servidor Multi-threaded Simulado
 * Aluno: Gabryel Martins Assis
 *
 * Descrição:
 *   Simula um servidor que atende múltiplos clientes de forma concorrente.
 *   Cada cliente é representado por uma thread. O tempo de atendimento é
 *   simulado com sleep(). Um mutex protege o log de saída no terminal.
 *
 * Compilação:
 *   gcc -o servidor servidor_multithread.c -lpthread
 *
 * Execução:
 *   ./servidor
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* ──────────────────────────────────────────────
   Configurações do servidor
   ────────────────────────────────────────────── */
#define NUM_CLIENTES      8   /* número total de clientes que chegam */
#define MAX_ATENDIMENTO   4   /* tempo máximo de atendimento (segundos) */
#define MIN_ATENDIMENTO   1   /* tempo mínimo de atendimento (segundos) */

/* ──────────────────────────────────────────────
   Estrutura que representa um cliente
   ────────────────────────────────────────────── */
typedef struct {
    int id;               /* identificador único do cliente      */
    int tempo_servico;    /* tempo de atendimento sorteado       */
} Cliente;

/* ──────────────────────────────────────────────
   Mutex global – protege a saída no terminal
   ────────────────────────────────────────────── */
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

/* ──────────────────────────────────────────────
   Utilitário: retorna timestamp atual como string
   ────────────────────────────────────────────── */
static void timestamp(char *buf, size_t size) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    strftime(buf, size, "%H:%M:%S", t);
}

/* ──────────────────────────────────────────────
   Função executada por cada thread-cliente
   ────────────────────────────────────────────── */
void *atender_cliente(void *arg) {
    Cliente *cli = (Cliente *) arg;
    char ts[16];

    /* — Chegada do cliente — */
    timestamp(ts, sizeof(ts));
    pthread_mutex_lock(&mutex_log);
    printf("[%s] 🟡 Cliente %d chegou ao servidor  (tempo de serviço: %ds)\n",
           ts, cli->id, cli->tempo_servico);
    pthread_mutex_unlock(&mutex_log);

    /* — Processamento simulado — */
    sleep(cli->tempo_servico);

    /* — Saída do cliente — */
    timestamp(ts, sizeof(ts));
    pthread_mutex_lock(&mutex_log);
    printf("[%s] ✅ Cliente %d foi atendido e saiu (duração: %ds)\n",
           ts, cli->id, cli->tempo_servico);
    pthread_mutex_unlock(&mutex_log);

    pthread_exit(NULL);
}

/* ──────────────────────────────────────────────
   Main – cria threads e aguarda conclusão
   ────────────────────────────────────────────── */
int main(void) {
    pthread_t threads[NUM_CLIENTES];
    Cliente   clientes[NUM_CLIENTES];
    char ts[16];

    srand((unsigned int) time(NULL));

    timestamp(ts, sizeof(ts));
    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Servidor Multi-threaded – DCC 403      ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("[%s] 🚀 Servidor iniciado. Aguardando clientes...\n\n", ts);

    /* Cria uma thread para cada cliente */
    for (int i = 0; i < NUM_CLIENTES; i++) {
        clientes[i].id           = i + 1;
        clientes[i].tempo_servico = MIN_ATENDIMENTO +
                                    rand() % (MAX_ATENDIMENTO - MIN_ATENDIMENTO + 1);

        int ret = pthread_create(&threads[i], NULL, atender_cliente, &clientes[i]);
        if (ret != 0) {
            fprintf(stderr, "[ERRO] Falha ao criar thread para cliente %d\n", i + 1);
            exit(EXIT_FAILURE);
        }

        /* Pequena pausa para simular chegada gradual dos clientes */
        usleep(200000); /* 0.2 s */
    }

    /* Aguarda todas as threads terminarem */
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Destruição do mutex */
    pthread_mutex_destroy(&mutex_log);

    timestamp(ts, sizeof(ts));
    printf("\n[%s] 🔒 Todos os clientes foram atendidos. Servidor encerrado.\n", ts);

    return 0;
}
