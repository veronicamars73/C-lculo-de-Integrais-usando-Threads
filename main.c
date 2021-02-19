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
double* vetor;

// Struct de parâmetros para as funções
typedef struct  {
	double ponto_inicial;
	double ponto_final;
	int num_trapezios;
	int num_threads;
	int id_thread;
	double altura_trapezios;
	pthread_t *threads;
}InfosIntegral;
 
int main (int argc, char *argv[]) {
	// Uso: ./programa ponto_inicial ponto_final num_trapezios num_threads id_funcao
	// Função 1: f(x) = 5 / Função 2: f(x) = sen(x*2) + cos(x*5)

	// Verifica se o usuário inseriu a quantidade correta de parâmetros na linha de comando
	if(argc != 6){
		return uso();
	}

	double ponto_inicial, ponto_final, somatorio_area = 0.0;
	int num_trapezios, num_threads, id_funcao;
	InfosIntegral info_obj;

	sscanf(argv[1], "%lf", &ponto_inicial);
	sscanf(argv[2], "%lf", &ponto_final);
	sscanf(argv[3], "%d", &num_trapezios);
	sscanf(argv[4], "%d", &num_threads);
	sscanf(argv[5], "%d", &id_funcao);

	// Alocação dinâmica do vetor global
	vetor = (double*) calloc(num_threads, sizeof(double));

	// Atribuição dos valores das variáveis do struct com os parâmetros da linha de comando
	info_obj.ponto_inicial = ponto_inicial;
	info_obj.ponto_final = ponto_final * M_PI;
	info_obj.num_threads = num_threads;
	info_obj.num_trapezios = num_trapezios;
	info_obj.altura_trapezios = (ponto_final - ponto_inicial) / (num_trapezios);

	pthread_t threads[num_threads];
	info_obj.threads = threads;

	// Laços para a criação de threads sincronizadas passando o struct como parâmetro
	if(id_funcao == 1){ // Função 1: f(x) = 5
		for(int i = 0 ; i < num_threads; ++i){
			info_obj.id_thread = i;
			printf("Criando thread %d...\n", i);
			pthread_create(&threads[i], NULL, funcao_id1, &info_obj);
			pthread_join(threads[i],NULL);
		}
	}else{
		if (id_funcao == 2) { // Função 2: f(x) = sen(x*2) + cos(x*5)
			for(int i = 0 ; i < num_threads; ++i){
				info_obj.id_thread = i;
				printf("Criando thread %d...\n", i);
				pthread_create(&threads[i], NULL, funcao_id2, &info_obj);
				pthread_join(threads[i],NULL);
			}
		}else {
			printf("Código de função inválido!\n");
			return 1;
		}
	}

	// Calcula o somatório das áreas
	for(int i = 0; i < num_threads; ++i){
		somatorio_area += vetor[i];
	}

	//printf("O valor total da área é: %1.2e\n", somatorio_area);
	printf("O valor total da área é: %lf\n", somatorio_area);

	// Libera o espaço de memória alocado dinamicamente
	free(vetor);

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
	double area;
	int local_n;

	// Verifica se não é a primeira thread
	if (ifobj->id_thread != 0){ 
		
		// Calcula o número de trapézios da thread
		local_n = ifobj->num_trapezios / ifobj->num_threads;
		
		// Calcula a área do intervalo
		area = (5+5)*ifobj->altura_trapezios/2; // [f(local_a)+f(local_a+h)]*(altura/2)
		for (int i = 1; i < local_n; ++i){
			area += (5+5)*ifobj->altura_trapezios/2;// [f(prox_a)+f(prox_a+h)]*(altura/2)
		}

	}else{
		// Na primeira thread, local_n será somada ao resto da divisão para cobrir os casos de divisão não inteira
		local_n = ifobj->num_trapezios / ifobj->num_threads + (ifobj->num_trapezios % ifobj->num_threads);
		
		// Calcula a área do intervalo
		area = (5+5)*ifobj->altura_trapezios/2; // [f(local_a)+f(local_a+h)]*(altura/2)
		for (int i = 1; i < local_n; ++i){
			area += (5+5)*ifobj->altura_trapezios/2;// [f(prox_a)+f(prox_a+h)]*(altura/2)
		}
	}

	printf("Thread %d em andamento, area calculada = %lf\n", ifobj->id_thread, area);

	// Insere área calculada pela thread no vetor dinamicamente alocado
	vetor[index_global] = area;
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
	double local_a, prox_a, area;
	int local_n;

	if (ifobj->id_thread != 0){
		
		// Calcula o número de trapézios da thread
		local_n = ifobj->num_trapezios / ifobj->num_threads;

		// Calcula o ponto incial da thread atual
		local_a = ifobj->ponto_inicial + (local_n * ifobj->id_thread) + (ifobj->num_trapezios % ifobj->num_threads);

		// Calcula a área do intervalo
		area = (funcao_geo(local_a)+funcao_geo(local_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2; //f(local_a)+f(local_a+h)/2
		for (int i = 1; i < local_n; ++i){
			prox_a = local_a + i*ifobj->altura_trapezios;
			area += (funcao_geo(prox_a) + funcao_geo(prox_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}

	}else{
		// Na primeira thread, local_n será somada ao resto da divisão para cobrir os casos de divisão não inteira
		local_n = ifobj->num_trapezios / ifobj->num_threads + (ifobj->num_trapezios % ifobj->num_threads);
		
		// Calcula  o ponto inical da thread
		local_a = ifobj->ponto_inicial;
		
		// Calcula a área do intervalo
		area = (funcao_geo(local_a)+funcao_geo(local_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2; // f(local_a)+f(local_a+h)/2
		for (int i = 1; i < local_n; ++i){
			prox_a = local_a+i*ifobj->altura_trapezios;
			area += (funcao_geo(prox_a)+funcao_geo(prox_a + ifobj->altura_trapezios)) * ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}
	}

	printf("Thread %d em andamento, area calculada = %lf\n", ifobj->id_thread, area);

	// Insere área calculada pela thread no vetor dinamicamente alocado
	vetor[index_global] = area;
	index_global++;

	pthread_exit(NULL);

}