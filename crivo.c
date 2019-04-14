#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#define NUM 100
#define NUM_THREAD 4

int vetor[NUM]; //Vetor representando os números de 1 até o valor máximo(com isso cada posição do vetor representa o número seguinte)
pthread_barrier_t barreira; //Barreira para segurar as threads em ponto crítico

/*Função responsável por pegar um valor base (k) e marcar todos os seus múltiplos como não primos, a partir de k², ignorando os números pares*/
void *funcao_crivo(void *arg){
	int i;
	//printf("thread %d mandando um olá.\n",pthread_self()); 
	//printf("O valor de k é: %d na thread: %d\n",k,pthread_self());
	
	//Assume-se em primeira instância que todos os números impares mais o 2 são primos, logo tais posições do vetor recebem o valor 1 e o restante recebe 0
	for(i=pthread_self()-1;i<NUM;i=i+NUM_THREAD){
		if( i%2==0 || i==1){
			vetor[i]=1;
		}else{
			vetor[i]=0;
		}
		//printf("thread: %d na posição: %d com o valor: %d\n", pthread_self(), i, vetor[i]);
	}
	
	/*Esperar que todas as threads terminem de preencher o vetor para assim dar início a verificação dos números primos*/
	printf("thread: %d esperando\n", pthread_self()); 
	pthread_barrier_wait(&barreira);
	printf("thread: %d foi\n", pthread_self());
		
	int k=pthread_self()+pthread_self()+1; //Cada thread possui um k próprio começando com k=3 e pegando apenas números impares após isso
	//A partir de k até o valor máximo divido por k, altera no vetor o valor para 0 dos múltiplos de k
	while(k <= sqrt(NUM)) {
		//printf("O valor de k é: %d na thread: %d\n",k,pthread_self());
		for(i = k; i <= (NUM/k); i++){
			//printf("estamos no i:%d\n",i);
			vetor[(k*i)-1]=0;
		}	
		k=k+(NUM_THREAD*2);
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
