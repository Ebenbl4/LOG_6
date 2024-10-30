#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

void initialize_graph(int size, int_fast8_t** graph_ptr, float edge_prob);
int_fast8_t** create_graph(int size);
void print_graph(int size, int_fast8_t** graph_pointer);
int_fast8_t** decart_mult(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** G_1;
	int_fast8_t** G_2;
	int Size_G_1, Size_G_2;
	float edge_prob_1, edge_prob_2;

	fprintf(stdout, "Введите количество вершин графа G1: ");
	if (!scanf("%d", &Size_G_1) || Size_G_1 <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите количество вершин графа G2: ");
	if (!scanf("%d", &Size_G_2) || Size_G_2 <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите вероятность создания ребра графа G1 (от 0 до 100): ");
	if (!scanf("%f", &edge_prob_1) || edge_prob_1 < 0 || edge_prob_1 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}
	fprintf(stdout, "Введите вероятность создания ребра графа G2 (от 0 до 100): ");
	if (!scanf("%f", &edge_prob_2) || edge_prob_2 < 0 || edge_prob_2 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}
	G_1 = create_graph(Size_G_1);
	G_2 = create_graph(Size_G_2);
	srand((unsigned int)time(NULL));
	initialize_graph(Size_G_1, G_1, edge_prob_1);
	initialize_graph(Size_G_2, G_2, edge_prob_2);
	printf("Матрица смежности графа G1: \n\n");
	print_graph(Size_G_1, G_1);
	printf("\nМатрица смежности графа G2: \n\n");
	print_graph(Size_G_2, G_2);
	printf("\nДекартово произведение графов G1 и G2:\n\n");
	int_fast8_t** G_3 = decart_mult(Size_G_1, Size_G_2, G_1, G_2);
	print_graph(Size_G_1 * Size_G_2, G_3);

	for (int i = 0; i < Size_G_1; i++) {
		free(G_1[i]);
	}
	for (int i = 0; i < Size_G_2; i++) {
		free(G_2[i]);
	}
	for (int i = 0; i < Size_G_1 * Size_G_2; i++) {
		free(G_3[i]);
	}
	free(G_1);
	free(G_2);
	free(G_3);

	return 0;
}

int_fast8_t** create_graph(int size) {
	int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
	for (int i = 0; i < size; i++) {
		array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
	}
	if (array == NULL) {
		fprintf(stderr, "Ошибка создания массива");
		exit(1);
	}
	return array;
}

void initialize_graph(int size, int_fast8_t** graph_ptr, float edge_prob) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_prob) {
				graph_ptr[i][j] = 1;
				graph_ptr[j][i] = 1;
			}
			else {
				graph_ptr[i][j] = 0;
				graph_ptr[j][i] = 0;
			}
		}
		graph_ptr[i][i] = 0;
	}
}

void print_graph(int size, int_fast8_t** graph_pointer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(stdout, "%d ", graph_pointer[j][i]);
		}
		fprintf(stdout, "\n");
	}
}

int_fast8_t** decart_mult(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2) {
	int new_size = size1 * size2;
	int_fast8_t** result = create_graph(new_size);

	for (int i = 0; i < new_size; i++) {
		for (int j = 0; j < new_size; j++) {
			result[i][j] = 0;
		}
	}

	for (int i1 = 0; i1 < size1; i1++) {
		for (int i2 = 0; i2 < size2; i2++) {
			for (int j1 = 0; j1 < size1; j1++) {
				for (int j2 = 0; j2 < size2; j2++) {
					if ((i1 == j1 && G2[i2][j2]) || (i2 == j2 && G1[i1][j1])) {
						result[i1 * size2 + i2][j1 * size2 + j2] = 1;
					}
				}
			}
		}
	}
	return result;
}