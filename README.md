# seminario_SO_Servidor-multi-threaded
Neste repositório se encontra o código fonte da simulação de um servidor multi thread na lingugem C
## Disciplina
 
Universidade Federal de Roraima 

#Servidor Multi-threaded Simulado

> **Sistemas Operacionais I – Programação Concorrente com Threads**  
> Aluno: Gabryel Martins Assis

Simulação de um servidor concorrente que atende múltiplos clientes simultaneamente utilizando **POSIX Threads (pthreads)** em C.

Cada cliente é representado por uma thread independente. O tempo de atendimento é simulado com `sleep()` e um **mutex** protege a saída no terminal contra condições de corrida.

---

## Exemplo do que seria a saída

```
║   Servidor Multi-threaded – DCC 403      ║

[14:32:01] Servidor iniciado. Aguardando clientes...

[14:32:01] Cliente 1 chegou ao servidor  (tempo de serviço: 3s)
[14:32:01] Cliente 2 chegou ao servidor  (tempo de serviço: 1s)
[14:32:02] Cliente 3 chegou ao servidor  (tempo de serviço: 4s)
[14:32:02] Cliente 2 foi atendido e saiu (duração: 1s)
[14:32:04] Cliente 1 foi atendido e saiu (duração: 3s)
[14:32:06] Cliente 3 foi atendido e saiu (duração: 4s)

[14:32:06] Todos os clientes foram atendidos. Servidor encerrado.
```

> Note que os clientes **saem em ordem diferente da chegada** — isso demonstra a execução paralela das threads.

---

## Conceitos de Concorrência Utilizados

| Recurso | Função |
|---|---|
| `pthread_create` | Cria uma thread por cliente |
| `pthread_join` | Aguarda todas as threads terminarem antes de encerrar |
| `pthread_mutex_lock/unlock` | Protege o `printf` contra condição de corrida |
| `sleep()` | Simula o tempo de processamento de cada cliente |
| `pthread_mutex_destroy` | Libera o mutex ao encerrar o servidor |

---

## Estrutura do Projeto

```
.
├── servidor_multithread.c   # Código-fonte principal
└── README.md                # Este arquivo
```

---

## Configurações

No topo do arquivo `servidor_multithread.c` você pode ajustar os números de clientes e o tempo que quiser no código:

```c
#define NUM_CLIENTES      8   // número total de clientes
#define MAX_ATENDIMENTO   4   // tempo máximo de atendimento (segundos)
#define MIN_ATENDIMENTO   1   // tempo mínimo de atendimento (segundos)
```

---

## Desafios e Decisões de Projeto

- **Condição de corrida no terminal:** sem o mutex, múltiplas threads escreveriam ao mesmo tempo, embaralhando o texto. Resolvido com `pthread_mutex_lock/unlock`.
- **Encerramento prematuro:** sem `pthread_join`, o `main()` poderia encerrar antes das threads terminarem. O join garante que o servidor só fecha após todos os clientes serem atendidos.
- **Passagem de argumentos:** cada cliente recebe um ponteiro para sua própria `struct Cliente`, evitando que threads diferentes compartilhem o mesmo endereço de memória.

---

## Análise de Desempenho

| Modelo | Tempo total |
|---|---|
| Sequencial | `t1 + t2 + ... + tn` |
| Concorrente (este projeto) | `≈ max(t1, t2, ..., tn)` |

O ganho de desempenho cresce proporcionalmente ao número de clientes simultâneos.
 
Seminário 2026
