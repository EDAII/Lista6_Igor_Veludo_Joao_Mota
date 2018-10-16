// Alunos: Igor Veludo            13/0028240
//         Joao Pedro Mota Jardim 16/0031982

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 15
#define TAREFAS 10

typedef struct {
    unsigned int start;
    unsigned int finish;
    unsigned int index;
} job;

int mostraMenu();
void insereTarefa(unsigned int *starts,unsigned int *finishes,unsigned int *todos,int *validos);
void removeTarefa(unsigned int *starts,unsigned int *finishes,unsigned int *todos,int *validos);
int compare_jobs(const job *job1, const job *job2);
unsigned int interval_scheduling(const unsigned int *starts, const unsigned int *finishes,
        unsigned int *schedule, size_t len);
void print_schedule(const unsigned int *schedule, const unsigned int *starts,
        const unsigned int *finishes, size_t len);
void geraTarefas(unsigned int *starts,unsigned int *finishes);
void imprimeVet(int *vetor);
void imprimeGrafico(unsigned int *schedule, const unsigned int *starts,
        const unsigned int *finishes, int count);


int main(void){
  srand(time(NULL));
  int opcao;
  unsigned int starts[TAREFAS];
  unsigned int finishes[TAREFAS];
  const size_t len = sizeof(starts) / sizeof(starts[0]);
  unsigned int *schedule = malloc(len * sizeof(unsigned int));
  unsigned int jobs = 0;
  unsigned int todos[TAREFAS];
  int validos = TAREFAS;

  //Inicia o vetor de amostragem com 1 (Mostrar todas as tarefas no grafico)
  for(int i = 0; i < TAREFAS;i++){
    todos[i] = 1;
  }

  do{
    opcao = mostraMenu();
    switch(opcao){
      case 1:
        geraTarefas(starts,finishes);
        printf("\nindice do trabalho\n");
        imprimeGrafico(todos, starts, finishes, validos);
        printf("                    horario\n");
        break;
      case 2:
        insereTarefa(starts,finishes,todos,&validos);
        break;
      case 3:
        removeTarefa(starts,finishes,todos,&validos);
        break;
      case 4:
        jobs = interval_scheduling(starts, finishes, schedule, len);
        printf("Ha %d tarefas:\n", jobs);
        print_schedule(schedule, starts, finishes, len);
        break;
      case 5:
        printf("\nindice do trabalho\n");
        imprimeGrafico(todos, starts, finishes, validos);
        printf("                    horario\n");
    }
  }while(opcao != 0);
  free(schedule);
  return 0;
}

int mostraMenu(){
  int opcao;
  //system("clear");
  printf("\n");
  printf(" ==========================================\n");
  printf("|         >>>>>>>>> Menu <<<<<<<<<         |\n");
  printf("| 1 - Gerar Tarefas Aleatorias             |\n");
  printf("| 2 - Inserir/modificar Tarefa Manualmente |\n");
  printf("| 3 - Remover Tarefa Manualmente           |\n");
  printf("| 4 - Iniciar Interval Scheduling          |\n");
  printf("| 5 - Imprimir grafico                     |\n");
  printf("| 0 - Sair                                 |\n");
  printf(" ==========================================\n");
  printf("Sua opcao: ");
  scanf("%d",&opcao);
  return opcao;
}

void insereTarefa(unsigned int *starts,unsigned int *finishes,unsigned int *todos, int *validos){
    int posicao = 0,inicio = 0, fim = 0;
    printf("Qual o numero da tarefa deseja inserir/modificar manualmente? (entre 1 e %d)\n",TAREFAS);
    scanf("%d",&posicao);
    printf("Insira o horario de inicio da tarefa %d (entre 1 e %d): ",posicao, MAX);
    scanf("%d",&inicio);
    printf("Insira o horario de termino da tarefa %d (entre 1 e %d): ",posicao, MAX);
    scanf("%d",&fim);
    starts[posicao-1] = inicio;
    finishes[posicao-1] = fim;
    if(todos[posicao-1] != 1){
      todos[posicao-1] = 1;
      *validos = *validos + 1;
    }
    printf("Tarefa inserida ou modificada com sucesso\n");
}

void removeTarefa(unsigned int *starts,unsigned int *finishes,unsigned int *todos, int *validos){
    int posicao = 0;
    printf("Qual o numero da tarefa deseja remover manualmente? (entre 1 e %d)\n",TAREFAS);
    scanf("%d",&posicao);
    starts[posicao-1] = MAX+1;
    finishes[posicao-1] = MAX+1;
    if(todos[posicao-1] != 0){
      todos[posicao-1] = 0;
      *validos = *validos - 1;
    }
    printf("Tarefa removida com sucesso\n");
}

/* Compara as tarefas pelo tempo de termino */
int compare_jobs(const job *job1, const job *job2){
    if (job1->finish < job2->finish) {
        return -1;
    }
    if (job1->finish == job2->finish) {
        return 0;
    }
    return 1;
}

unsigned int interval_scheduling(const unsigned int *starts, const unsigned int *finishes,
        unsigned int *schedule, size_t len){
    unsigned int i;
    job *jobs;
    job *last;
    unsigned int count = 0;

    jobs = malloc(len * sizeof(job));
    if (jobs == NULL) {
        return 0;
    }
    for (i = 0; i < len; i++) {
        jobs[i].start = starts[i];
        jobs[i].finish = finishes[i];
        jobs[i].index = i;
    }
    /* Organiza as tarefas pelo tempo de termino */
    qsort(jobs, len, sizeof(job), (int(*)(const void *, const void *))compare_jobs);
    memset(schedule, 0, len * sizeof(unsigned int));
    for (i = 0; i < len; i++) {
        /* uma tarefa e compativel se seu inicio >= o fim da ultima tarefa adicionada
           e se ela existir(nao estar com valores invalidos)*/
        if (jobs[i].start < MAX+1){
          if (i == 0 || jobs[i].start >= last->finish) {
              schedule[jobs[i].index] = 1;
              last = &(jobs[i]);
              count++;
          }
        }
    }
    imprimeGrafico(schedule, starts, finishes, count);
    free(jobs);
    return count;
}

void print_schedule(const unsigned int *schedule, const unsigned int *starts,
        const unsigned int *finishes, size_t len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        if (schedule[i]) {
            printf("Tarefa %u comecando as %u e terminando as %u\n", i+1,
                    starts[i], finishes[i]);
        }
    }
}

void geraTarefas(unsigned int *starts,unsigned int *finishes){
  int i = 0;
  for(i = 0 ; i < TAREFAS ; i++){
    starts[i] = (rand() % (MAX-7)) + 1;
    finishes[i] = starts[i] + (rand() % 7 + 1);
  }
  printf("\ncomeco:\n");
  imprimeVet(starts);
  printf("fim:\n");
  imprimeVet(finishes);
}

void imprimeVet(int *vetor){
  int i;
  for(i = 0 ; i < TAREFAS ; i++)
    printf("%d ", vetor[i]);
    printf("\n");
}
void imprimeGrafico(unsigned int *schedule, const unsigned int *starts,
        const unsigned int *finishes, int compativeis){
  int linha,coluna,tamanho,atual = 0,i,j;
  char matriz[TAREFAS+2][MAX+1];
  //prenche todos os espacos da matriz com espa�os em brancos
  for(linha = 0; linha < TAREFAS+2;linha++){
    for(coluna = 0; coluna < MAX+1;coluna++){
       matriz[linha][coluna] = ' ';
    }
  }
  //preenche a primeira coluna da matriz com | para ser o eixo y
  for(linha = 0; linha < TAREFAS+2;linha++){
    matriz[linha][0] = '|';
  }
  //o cruzamento do eixo x e y do grafico
  matriz[0][0] = 'L';
  //preenchendo a primeira linha com _ para ser o eixo X
  for(coluna = 1; coluna < MAX+1;coluna++){
    matriz[0][coluna] = '_';
  }
  //preenche as tarefas no grafico
  for(i = 0; i < compativeis; i ++){
    while(schedule[atual] == 0){
      atual++;
    }
    tamanho = finishes[atual] - starts[atual];
    for(j = 0; j < tamanho+1;j++){
      matriz[atual+2][starts[atual]+j] = '*';
    }
    atual++;
  }
  //print do grafico
  for(linha = TAREFAS+1; linha >= 0;linha--){
    //primeira parte imprime os numeros do eixo y
    if (linha - 1 >= 10){
      printf("%d",linha-1);
    }
    else if(linha - 1 >= 0){
      printf(" %d",linha-1);
    }
    //para nao imprimir o -1
    else{
      printf("  ");
    }
    for(coluna = 0; coluna < MAX+1;coluna++){
       printf("%c ",matriz[linha][coluna]);
    }
    printf("\n");
  }
  printf("  ");
  for(coluna = 0; coluna < MAX+1;coluna++){
     printf("%d ",coluna);
  }
  printf("\n");
}
