#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#define NUM 100
#define NUM_THREAD 4

int vetor[NUM]; //Vetor representando os n�meros de 1 at� o valor m�ximo(com isso cada posi��o do vetor representa o n�mero seguinte)
pthread_barrier_t barreira; //Barreira para segurar as threads em ponto cr�tico

/*Fun��o respons�vel por pegar um valor base (k) e marcar todos os seus m�ltiplos como n�o primos, a partir de k�, ignorando os n�meros pares*/
void *funcao_crivo(void *arg){
	int i;
	//printf("thread %d mandando um ol�.\n",pthread_self()); 
	//printf("O valor de k �: %d na thread: %d\n",k,pthread_self());
	
	//Assume-se em primeira inst�ncia que todos os n�meros impares mais o 2 s�o primos, logo tais posi��es do vetor recebem o valor 1 e o restante recebe 0
	for(i=pthread_self()-1;i<NUM;i=i+NUM_THREAD){
		if( i%2==0 || i==1){
			vetor[i]=1;
		}else{
			vetor[i]=0;
		}
		//printf("thread: %d na posi��o: %d com o valor: %d\n", pthread_self(), i, vetor[i]);
	}
	
	/*Esperar que todas as threads terminem de preencher o vetor para assim dar in�cio a verifica��o dos n�meros primos*/
	printf("thread: %d esperando\n", pthread_self()); 
	pthread_barrier_wait(&barreira);
	printf("thread: %d foi\n", pthread_self());
		
	int k=pthread_self()+pthread_self()+1; //Cada thread possui um k pr�prio come�ando com k=3 e pegando apenas n�meros impares ap�s isso
	//A partir de k at� o valor m�ximo divido por k, altera no vetor o valor para 0 dos m�ltiplos de k
	while(k <= sqrt(NUM)) {
		//printf("O valor de k �: %d na thread: %d\n",k,pthread_self());
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
	
	//Cria��o das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_create(&crivo[i],NULL,funcao_crivo,NULL);
	}	
	//Encerramento das threads
	for(i=0;i<NUM_THREAD;i++){
		pthread_join(crivo[i],NULL);
	}

	//Imprime o vetor inteiro
	for(i=0;i<NUM;i++){
		printf("posi��o do vetor:%d  valor do vetor:%d\n",i,vetor[i]);	
	}

	//Imprime apenas os n�meros primos	
	printf ("Os n�meros primos at� %d s�o: ",NUM);
	for(i=0;i<NUM;i++){
		if(vetor[i]==1){
			printf("%d ",i+1);	
		}
	}	
	return 0;
}
