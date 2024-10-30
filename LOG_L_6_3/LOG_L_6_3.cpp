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
int_fast8_t** unite_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size);
int_fast8_t** intersect_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size);
int_fast8_t** ring_sum_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size);

int main(void) {
    setlocale(LC_ALL, "Russian");
    int_fast8_t** G_1;
    int_fast8_t** G_2;
    int_fast8_t** G_united;
    int Size_G_1, Size_G_2, united_size;
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
    srand((unsigned int)time(NULL));
    G_1 = create_graph(Size_G_1);
    G_2 = create_graph(Size_G_2);
    initialize_graph(Size_G_1, G_1, edge_prob_1);
    initialize_graph(Size_G_2, G_2, edge_prob_2);

    printf("Матрица смежности графа G1: \n\n");
    print_graph(Size_G_1, G_1);
    printf("\n");
    printf("Матрица смежности графа G2: \n\n");
    print_graph(Size_G_2, G_2);
    printf("\n");

    // Объединение графов
    G_united = unite_graphs(Size_G_1, Size_G_2, G_1, G_2, &united_size);
    printf("Матрица смежности объединенного графа: \n\n");
    print_graph(united_size, G_united);

    int_fast8_t** G_intersected;
    int intersected_size;

    // Пересечение графов
    G_intersected = intersect_graphs(Size_G_1, Size_G_2, G_1, G_2, &intersected_size);
    printf("\nМатрица смежности пересечения графов: \n\n");
    print_graph(intersected_size, G_intersected);

    int_fast8_t** G_ring_sum;
    int ring_sum_size;

    // Кольцевая сумма графов
    G_ring_sum = ring_sum_graphs(Size_G_1, Size_G_2, G_1, G_2, &ring_sum_size);
    printf("\nМатрица смежности кольцевой суммы графов: \n\n");
    print_graph(ring_sum_size, G_ring_sum);

    for (int i = 0; i < Size_G_1; i++) {
        free(G_1[i]);
    }
    for (int i = 0; i < Size_G_2; i++) {
        free(G_2[i]);
    }
    for (int i = 0; i < united_size; i++) {
        free(G_united[i]);
    }
    for (int i = 0; i < intersected_size; i++) {
        free(G_intersected[i]);
    }
    for (int i = 0; i < ring_sum_size; i++) {
        free(G_ring_sum[i]);
    }

    free(G_1);
    free(G_2);
    free(G_united);
    free(G_intersected);
    free(G_ring_sum);
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

int_fast8_t** unite_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size) {
    *result_size = (size1 > size2) ? size1 : size2;
    int_fast8_t** united = create_graph(*result_size);

    for (int i = 0; i < *result_size; i++) {
        for (int j = 0; j < *result_size; j++) {
            if (i < size1 && j < size1 && i < size2 && j < size2) {
                united[i][j] = G1[i][j] || G2[i][j];
            }
            else if (i < size1 && j < size1) {
                united[i][j] = G1[i][j];
            }
            else if (i < size2 && j < size2) {
                united[i][j] = G2[i][j];
            }
            else {
                united[i][j] = 0;
            }
        }
    }

    return united;
}

int_fast8_t** intersect_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size) {
    *result_size = (size1 < size2) ? size1 : size2;
    int_fast8_t** intersected = create_graph(*result_size);

    for (int i = 0; i < *result_size; i++) {
        for (int j = 0; j < *result_size; j++) {
            intersected[i][j] = G1[i][j] && G2[i][j];
        }
    }

    return intersected;
}

int_fast8_t** ring_sum_graphs(int size1, int size2, int_fast8_t** G1, int_fast8_t** G2, int* result_size) {
    *result_size = (size1 > size2) ? size1 : size2;
    int_fast8_t** ring_sum = create_graph(*result_size);

    for (int i = 0; i < *result_size; i++) {
        for (int j = 0; j < *result_size; j++) {
            if (i < size1 && j < size1 && i < size2 && j < size2) {
                ring_sum[i][j] = G1[i][j] ^ G2[i][j];
            }
            else if (i < size1 && j < size1) {
                ring_sum[i][j] = G1[i][j];
            }
            else if (i < size2 && j < size2) {
                ring_sum[i][j] = G2[i][j];
            }
            else {
                ring_sum[i][j] = 0;
            }
        }
    }

    return ring_sum;
}