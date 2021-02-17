
#include <stdio.h>       
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
  

int uso();
void *funcao_id1(void *argumentos);
void *funcao_id2(void *argumentos);
float funcao_geo(float grau_rad);

int sum = 1;

typedef struct  {
	float ponto_inicial;
	float ponto_final;
	int num_trapezios;
	int num_threads;
	int id_thread;
	float altura_trapezios;
	pthread_t *threads;
}InfosIntegral;
 
int main (int argc, char *argv[]) {
	// uso: ./programa ponto_inicial ponto_final num_trapezios num_threads id_funcao
	// Funcao 1: f(x) = 5 / Funcao 2: f(x) = sen(x*2) + cos(x*5)

	if(argc != 6){
		return uso();
	}

	float ponto_inicial, ponto_final;
	int num_trapezios, num_threads, id_funcao;
	InfosIntegral info_obj;

	sscanf(argv[1], "%f", &ponto_inicial);
	sscanf(argv[2], "%f", &ponto_final);
	sscanf(argv[3], "%d", &num_trapezios);
	sscanf(argv[4], "%d", &num_threads);
	sscanf(argv[5], "%d", &id_funcao);

	info_obj.ponto_inicial = ponto_inicial;
	info_obj.ponto_final = ponto_final * M_PI;
	info_obj.num_threads = num_threads;
	info_obj.num_trapezios = num_trapezios;
	info_obj.altura_trapezios = (ponto_final - ponto_inicial) / (num_trapezios);
	//printf("%f, %f,%f\n", info_obj.altura_trapezios, info_obj);

	pthread_t threads[num_threads];
	info_obj.threads = threads;

	int i;
	if(id_funcao==1){
		for(i=0;i<num_threads;i++){
			info_obj.id_thread = i;
			printf("Criando %d\n", i);
			pthread_create(&threads[i], NULL, funcao_id1, &info_obj);
			pthread_join(threads[i],NULL);
			//sleep(.1);
		}
	}else{
		if (id_funcao == 2) {
			for(i=0;i<num_threads;i++){
				info_obj.id_thread = i;
				printf("Criando %d\n", i);
				pthread_create(&threads[i], NULL, funcao_id2, &info_obj);
				pthread_join(threads[i],NULL);
			}
		}else {
			printf("Código de função inválido!\n");
			return 1;
		}
	}
	pthread_join(info_obj.threads[i-1],NULL);
	
	return 0;  
}

int uso(){
	printf("USO INCORRETO!\n");
	printf("Uso Correto: ./<nome_programa> ponto_inicial ponto_final num_trapezios num_threads id_funcao\n");
	return 1;
}

void *funcao_id1(void *argumentos) {
	InfosIntegral * ifobj = (InfosIntegral *) argumentos;
	float area;
	if (ifobj->id_thread != 0){
		
		int local_n = ifobj->num_trapezios / ifobj->num_threads;


		float local_a = ifobj->altura_trapezios * ((local_n * ifobj->id_thread) +
			ifobj->num_trapezios % ifobj->num_threads);

		float local_b = local_a + (ifobj->num_trapezios / ifobj->num_threads)*ifobj->altura_trapezios;

		//printf("%f\n", ifobj->altura_trapezios);
		area = (5+5)*ifobj->altura_trapezios/2; //f(local_a)+f(local_b)/2
		for (int i = 1; i < local_n; ++i){
			float prox_a = local_a+i*ifobj->altura_trapezios;
			area += (5+5)*ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}

		printf("%d em andamento, area calculada = %f\n", ifobj->id_thread, area);
		//pthread_exit((void *) area);
		pthread_exit(NULL);

	}else{
		int local_n = ifobj->num_trapezios / ifobj->num_threads;
		local_n = local_n + (ifobj->num_trapezios % ifobj->num_threads);

		float local_a = ifobj->altura_trapezios * (((ifobj->num_trapezios / ifobj->num_threads)
			*ifobj->id_thread)+ifobj->num_trapezios % ifobj->num_threads);

		float local_b = local_a + (ifobj->num_trapezios / ifobj->num_threads)*ifobj->altura_trapezios;

		area = (5+5)*ifobj->altura_trapezios/2; //f(local_a)+f(local_b)/2
		for (int i = 1; i < local_n; ++i){
			float prox_a = local_a+i*ifobj->altura_trapezios;
			area += (5+5)*ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}
		
		printf("%d em andamento, area calculada = %f\n", ifobj->id_thread, area);
		//pthread_exit(area);
		pthread_exit(NULL);
	}
}

float funcao_geo(float grau_rad){
	float result = sin(2*grau_rad) + cos(5*grau_rad);
	return result;
}

void *funcao_id2(void *argumentos) {
	InfosIntegral * ifobj = (InfosIntegral *) argumentos;
	float area;
	if (ifobj->id_thread != 0){
		
		int local_n = ifobj->num_trapezios / ifobj->num_threads;


		float local_a = ifobj->altura_trapezios * ((local_n * ifobj->id_thread) +
			ifobj->num_trapezios % ifobj->num_threads);

		float local_b = local_a + (ifobj->num_trapezios / ifobj->num_threads)*ifobj->altura_trapezios;

		//printf("%f\n", ifobj->altura_trapezios);
		area = (funcao_geo(local_a)+funcao_geo(local_b))*ifobj->altura_trapezios/2; //f(local_a)+f(local_b)/2
		for (int i = 1; i < local_n; ++i){
			float prox_a = local_a+i*ifobj->altura_trapezios;
			area += (funcao_geo(prox_a)+funcao_geo(prox_a + ifobj->altura_trapezios))
			*ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}

		printf("%d em andamento, area calculada = %f\n", ifobj->id_thread, area);
		//pthread_exit((void *) area);
		pthread_exit(NULL);

	}else{
		int local_n = ifobj->num_trapezios / ifobj->num_threads;
		local_n = local_n + (ifobj->num_trapezios % ifobj->num_threads);

		float local_a = ifobj->altura_trapezios * (((ifobj->num_trapezios / ifobj->num_threads)
			*ifobj->id_thread)+ifobj->num_trapezios % ifobj->num_threads);

		float local_b = local_a + (ifobj->num_trapezios / ifobj->num_threads)*ifobj->altura_trapezios;

		area = (funcao_geo(local_a)+funcao_geo(local_b))*ifobj->altura_trapezios/2; //f(local_a)+f(local_b)/2
		for (int i = 1; i < local_n; ++i){
			float prox_a = local_a+i*ifobj->altura_trapezios;
			area += (funcao_geo(prox_a)+funcao_geo(prox_a + ifobj->altura_trapezios))
			*ifobj->altura_trapezios/2;//+= (f(prox_a)+f(prox_a+h))*altura/2
		}
		
		printf("%d em andamento, area calculada = %f\n", ifobj->id_thread, area);
		//pthread_exit(area);
		pthread_exit(NULL);
	}
}