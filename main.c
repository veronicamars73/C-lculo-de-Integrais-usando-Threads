#include <stdio.h>       
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
  
// Assinatura das funções
int uso();
void *funcao_id1(void *argumentos);
void *funcao_id2(void *argumentos);
double funcao_geo(double grau_rad);

// Variáveis globais
int index_global = 0;
double** vetor;
double area;

// Struct de parâmetros para as funções
typedef struct  {
	double local_n;
	double local_a;
	double altura_trapezios;
	int num_threads;
}InfosIntegral;
 
int main (int argc, char *argv[]) {
	// Uso: ./programa ponto_inicial ponto_final num_trapezios num_threads id_funcao
	// Função 1: f(x) = 5 / Função 2: f(x) = sen(x*2) + cos(x*5)

	// Verifica se o usuário inseriu a quantidade correta de parâmetros na linha de comando
	if(argc != 6){
		return uso();
	}

	double ponto_inicial, ponto_final, altura_trapezios;
	double somatorio_area = 0.0;
	int num_trapezios, num_threads, id_funcao;

	sscanf(argv[1], "%lf", &ponto_inicial);
	sscanf(argv[2], "%lf", &ponto_final);
	sscanf(argv[3], "%d", &num_trapezios);
	sscanf(argv[4], "%d", &num_threads);
	sscanf(argv[5], "%d", &id_funcao);

	InfosIntegral * info_obj = (InfosIntegral *) malloc(num_threads * sizeof(InfosIntegral));
	// Acrescentando o pi no final da função 2
	if(id_funcao == 2){
		ponto_final = ponto_final * acos(-1.0);
	}

	altura_trapezios = (ponto_final - ponto_inicial) / (num_trapezios);

	int i = 0;
	while(1){
		if(i==num_threads){
			break;
		}
		if(i==0){
			info_obj[i].local_n = num_trapezios / num_threads + (num_trapezios % num_threads);
			info_obj[i].local_a = ponto_inicial;
		}else{
			info_obj[i].local_n = num_trapezios / num_threads;
			info_obj[i].local_a = ponto_inicial + (info_obj[i].local_n * i * altura_trapezios) +
			((num_trapezios % num_threads) * altura_trapezios);
		}
		info_obj[i].altura_trapezios = altura_trapezios;
		info_obj[i].num_threads = num_threads;
		i++;
	}
	// Alocação dinâmica do vetor global
	vetor = (double**) calloc(num_threads, sizeof(double*));

	for (int i = 0; i < num_threads; ++i) {
		vetor[i] = NULL;
	}


	pthread_t threads[num_threads];

	// Laços para a criação de threads sincronizadas passando o struct como parâmetro
	if(id_funcao == 1){ // Função 1: f(x) = 5
		for(int i = 0 ; i < num_threads; ++i){

			pthread_create(&threads[i], NULL, funcao_id1, &info_obj[i]);
			//pthread_join(threads[i],NULL);
		}
	}else{
		if (id_funcao == 2) { // Função 2: f(x) = sen(x*2) + cos(x*5)
			for(int i = 0 ; i < num_threads; ++i){

				pthread_create(&threads[i], NULL, funcao_id2, &info_obj[i]);
				//pthread_join(threads[i],NULL);
			}
		}else {
			printf("Código de função inválido!\n");
			return 1;
		}
	}

	for (int i = 0; i < num_threads; ++i){
		pthread_join(threads[i], NULL);
	}
	
	// Calcula o somatório das áreas
	for(int i = 0; i < num_threads; ++i){
		somatorio_area += *(vetor[i]);
	}

	//printf("O valor total da área é: %1.2e\n", somatorio_area);
	printf("O valor total da área é: %1.2e\n", somatorio_area);

	// Libera o espaço de memória alocado dinamicamente
	free(vetor);
	free(info_obj);

	return 0;  
}

// Função para informar inserção incorreta dos parâmetros na linha de comando
int uso(){
	printf("USO INCORRETO!\n");
	printf("Uso Correto: ./<nome_programa> ponto_inicial ponto_final num_trapezios num_threads id_funcao\n");
	return 1;
}

// Função para calcular a área de f(x) = 5
void *funcao_id1(void *argumentos) {

	InfosIntegral * ifobj = (InfosIntegral *) argumentos;
	area = 0;
	area = (5+5)*ifobj->altura_trapezios/2; // [f(local_a)+f(local_a+h)]*(altura/2)
	for (int i = 1; i < ifobj->local_n; ++i){
		area += (5+5)*ifobj->altura_trapezios/2;// [f(prox_a)+f(prox_a+h)]*(altura/2)
	}

	// Insere área calculada pela thread no vetor dinamicamente alocado
	for (int i = 0; i < ifobj->num_threads; ++i) {
		if (vetor[i]==NULL){

			vetor[i] = &area;
		}
	}
	index_global++;

	pthread_exit(NULL);
}

// Função para calcular f(x) = sen(x*2) + cos(x*5)
double funcao_geo(double grau_rad){
	double result = sin(2*grau_rad) + cos(5*grau_rad);
	return result;
}

// Função para calcular a área de f(x) = sen(x*2) + cos(x*5)
void *funcao_id2(void *argumentos) {

	InfosIntegral * ifobj = (InfosIntegral *) argumentos;
	double prox_a;
	area = 0;
	area += (funcao_geo(ifobj->local_a)+funcao_geo(ifobj->local_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2; //f(local_a)+f(local_a+h)/2
	for (int i = 1; i < ifobj->local_n; ++i){
		prox_a = ifobj->local_a + i*ifobj->altura_trapezios;
		area += (funcao_geo(prox_a) + funcao_geo(prox_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
	}

	printf("Thread %d em andamento, area calculada = %lf\n\n", index_global, area);

	// Insere área calculada pela thread no vetor dinamicamente alocado

	for (int i = 0; i < ifobj->num_threads; ++i) {
		if (vetor[i]==NULL){

			vetor[i] = &area;
			area = 0;
		}
	}
	index_global++;

	pthread_exit(NULL);

}