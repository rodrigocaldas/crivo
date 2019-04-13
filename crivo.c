#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#define NUM 30
#define NUM_THREAD 2

/*												LÓGICA DO PROGRAMA
Ter um vetor em que cada posição represente o número posterior a ele (posição 0 representa o número 1, posição 99 representa o número 100),
cada posição do vetor começa com um valor igual a 1, deste modo estamos assumindo que todos os números são primos e devemos provar o contrário.
A variável k é o número que pegamos como base para marcar todos os seus múltiplos como não primos(alterando o valor dentro do vetor para 0),
o valor de k é a id da thread+1, dessa forma pulamos o número 1 como é preciso pular e a variável é acrescida pelo número de threads criadas.
Com 4 Threads criada: T1 começa com k=1, depois k=5, depois k=9. T2 começa com k=2, depois k=6, depois k=10. E assim por diante.*/

/* Realizando uma média de tempo de execução depois de 3 execuções do programa, com NUM igual a 100.000.000 (Sem a impressão dos valores primos):
Com 2 threads: 9,7713
Com 3 threads: 9,6660
Com 4 threads: 9.7030*/

int vetor[NUM];
pthread_barrier_t barreira;

/*Função responsável por pegar um valor base (k) e marcar todos os seus múltiplos como não primos, a partir de k²*/
void *funcao_crivo(void *arg){
	//printf("thread %d mandando um olá.\n",pthread_self());
	//Cada thread possui um k próprio pulando o k=1
	int k=pthread_self()+1;
	int i;
	//printf("O valor de k é: %d na thread: %d\n",k,pthread_self());
	
	//Assume-se em primeira instância que todos os números são primos, logo toda posição do vetor recebe o valor 1
	for(i=pthread_self()-1;i<NUM;i=i+NUM_THREAD){
		vetor[i]=1;
		//printf("olá thread: %d na posição: %d com o valor: %d\n", pthread_self(), i, vetor[i]);
	}
	
	
	printf("thread: %d esperando\n", pthread_self());
	/*BARREIRA AQUI para esperar o preenchimento do vetor*/ 
	pthread_barrier_wait(&barreira);
	printf("thread: %d foi\n", pthread_self());
	
	
	//A partir de k até o valor máximo divido por k, altera no vetor o valor para 0 dos múltiplos de k
	while(k <= sqrt(NUM)) {
		//printf("O valor de k é: %d na thread: %d\n",k,pthread_self());
		for(i = k; i <= (NUM/k); i++){
			//printf("estamos no i:%d\n",i);
			vetor[(k*i)-1]=0;
		}	
		k=k+NUM_THREAD;
	}
}

int main(){
	setlocale(LC_ALL, "Portuguese");	
	int i;	
	pthread_t crivo[NUM_THREAD];
	pthread_barrier_init(&barreira, NULL, NUM_THREAD);
	
	
	//Criação das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_create(&crivo[i],NULL,funcao_crivo,NULL);
	}	
	//Encerramento das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_join(crivo[i],NULL);
	}

	//Imprime o vetor inteiro
	for(i=0;i<NUM;i++){
		printf("posição do vetor:%d  valor do vetor:%d\n",i,vetor[i]);	
	}

	//Imprime apenas os números primos	
	printf ("Os números primos até %d são: ",NUM);
	for(i=0;i<NUM;i++){
		if(vetor[i]==1){
			printf("%d ",i+1);	
		}
	}
	
	return 0;
}
