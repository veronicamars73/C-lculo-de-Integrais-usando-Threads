#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/utils.h"

// Função para informar inserção incorreta dos parâmetros na linha de comando
int uso(){
	printf("USO INCORRETO!\n");
	printf("Uso Correto: ./<nome_programa> ponto_inicial ponto_final num_trapezios num_threads id_funcao\n");
	return 1;
}

// Função para calcular f(x) = sen(x*2) + cos(x*5)
double funcao_geo(double grau_rad){
	double result = sin(2*grau_rad) + cos(5*grau_rad);
	return result;
}

// Função para trocar elemento de posição
void swap(double** xp, double** yp){ 
    double* temp = *xp; 
    *xp = *yp; 
    *yp = temp;
}

// Função para ordenar o vetor
void selectionSort(double **arr, int n)
{
    int i, j, min_idx;

    // Percorre o array
    for (i = 0; i < n - 1; i++){

        // Procura o menor elemento na parte não ordenada do array
        min_idx = i;

        for (j = i + 1; j < n; j++){
            if (*arr[j] < *arr[min_idx]){
                min_idx = j;
            }
        }

        // Troca o elemento de posição
        swap(&arr[min_idx], &arr[i]);
    }
}