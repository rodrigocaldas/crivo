#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#define NUM 100
#define NUM_THREAD 4

/*												L�GICA DO PROGRAMA
Ter um vetor em que cada posi��o represente o n�mero posterior a ele (posi��o 0 representa o n�mero 1, posi��o 99 representa o n�mero 100),
cada posi��o do vetor come�a com um valor igual a 1, deste modo estamos assumindo que todos os n�meros s�o primos e devemos provar o contr�rio.
A vari�vel k � o n�mero que pegamos como base para marcar todos os seus m�ltiplos como n�o primos(alterando o valor dentro do vetor para 0),
o valor de k � a id da thread+1, dessa forma pulamos o n�mero 1 como � preciso pular e a vari�vel � acrescida pelo n�mero de threads criadas.
Com 4 Threads criada: T1 come�a com k=1, depois k=5, depois k=9. T2 come�a com k=2, depois k=6, depois k=10. E assim por diante.*/

/* Realizando uma m�dia de tempo de execu��o depois de 3 execu��es do programa, com NUM igual a 100.000.000 (Sem a impress�o dos valores primos):
Com 2 threads: 9,7713
Com 3 threads: 9,6660
Com 4 threads: 9.7030*/

int vetor[NUM];
pthread_barrier_t barreira;

/*Fun��o respons�vel por pegar um valor base (k) e marcar todos os seus m�ltiplos como n�o primos, a partir de k�*/
void *funcao_crivo(void *arg){
	//printf("thread %d mandando um ol�.\n",pthread_self());
	//Cada thread possui um k pr�prio pulando o k=1
	int k=pthread_self()+pthread_self()+1;
	int i;
	//printf("O valor de k �: %d na thread: %d\n",k,pthread_self());
	
	//Assume-se em primeira inst�ncia que todos os n�meros s�o primos, logo toda posi��o do vetor recebe o valor 1
	for(i=pthread_self()-1;i<NUM;i=i+NUM_THREAD){
		if( i%2==0 || i==1){
			vetor[i]=1;
		}else{
			vetor[i]=0;
		}
		//printf("ol� thread: %d na posi��o: %d com o valor: %d\n", pthread_self(), i, vetor[i]);
	}
	
	
	printf("thread: %d esperando\n", pthread_self());
	/*BARREIRA AQUI para esperar o preenchimento do vetor*/ 
	pthread_barrier_wait(&barreira);
	printf("thread: %d foi\n", pthread_self());
	
	
	//A partir de k at� o valor m�ximo divido por k, altera no vetor o valor para 0 dos m�ltiplos de k
	while(k <= sqrt(NUM)) {
		//printf("O valor de k �: %d na thread: %d\n",k,pthread_self());
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
	
	
	//Cria��o das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_create(&crivo[i],NULL,funcao_crivo,NULL);
	}	
	//Encerramento das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_join(crivo[i],NULL);
	}

	//Imprime o vetor inteiro
	//for(i=0;i<NUM;i++){
	//	printf("posi��o do vetor:%d  valor do vetor:%d\n",i,vetor[i]);	
	//}

	//Imprime apenas os n�meros primos	
	printf ("Os n�meros primos at� %d s�o: ",NUM);
	for(i=0;i<NUM;i++){
		if(vetor[i]==1){
			printf("%d ",i+1);	
		}
	}
	
	return 0;
}
