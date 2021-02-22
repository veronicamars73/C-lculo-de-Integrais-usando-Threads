#ifndef STRUCT_H
#define STRUCT_H

// Struct de parâmetros para as funções
typedef struct  {
	double local_n;
	double local_a;
	double altura_trapezios;
	int num_threads;
	double* area;
}InfosIntegral;

#endif