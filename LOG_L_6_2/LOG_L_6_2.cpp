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
void identify_vertices(int_fast8_t** graph, int* size, int v1, int v2);
void contract_edge(int_fast8_t** graph, int* size, int v1, int v2);
void split_vertex(int_fast8_t*** graph, int* size, int v);

int main(void) {
    setlocale(LC_ALL, "Russian");
    int_fast8_t** G_1;
    int Size_G_1;
    float edge_prob_1;

    fprintf(stdout, "Введите количество вершин графа G1: ");
    if (!scanf("%d", &Size_G_1) || Size_G_1 <= 1) {
        fprintf(stderr, "Ошибка ввода размера массива");
        exit(1);
    }

    fprintf(stdout, "Введите вероятность создания ребра графа G1 (от 0 до 100): ");
    if (!scanf("%f", &edge_prob_1) || edge_prob_1 < 0 || edge_prob_1 > 100) {
        fprintf(stderr, "Ошибка ввода вероятности");
        exit(1);
    }

    G_1 = create_graph(Size_G_1);
    initialize_graph(Size_G_1, G_1, edge_prob_1);
    printf("Исходная матрица смежности графа G1: \n\n");
    print_graph(Size_G_1, G_1);

    int choice, v1, v2;
    do {
        printf("\nВыберите операцию:\n");
        printf("1. Отождествление вершин\n");
        printf("2. Стягивание ребра\n");
        printf("3. Расщепление вершины\n");
        printf("0. Выход\n\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Введите номера двух вершин для отождествления: ");
            scanf("%d %d", &v1, &v2);
            v1--; v2--;
            if (v1 >= 0 && v1 < Size_G_1 && v2 >= 0 && v2 < Size_G_1 && v1 != v2) {
                identify_vertices(G_1, &Size_G_1, v1, v2);
                printf("Результат отождествления вершин:\n");
                print_graph(Size_G_1, G_1);
            }
            else {
                printf("Некорректные номера вершин.\n");
            }
            break;
        case 2:
            printf("Введите номера двух вершин для стягивания ребра: ");
            scanf("%d %d", &v1, &v2);
            v1--; v2--;
            if (v1 >= 0 && v1 < Size_G_1 && v2 >= 0 && v2 < Size_G_1 && v1 != v2 && G_1[v1][v2] == 1) {
                contract_edge(G_1, &Size_G_1, v1, v2);
                printf("Результат стягивания ребра:\n");
                print_graph(Size_G_1, G_1);
            }
            else {
                printf("Некорректные номера вершин или между ними нет ребра.\n");
            }
            break;
        case 3:
            printf("Введите номер вершины для расщепления: ");
            scanf("%d", &v1);
            v1--; v2--;
            if (v1 >= 0 && v1 < Size_G_1) {
                split_vertex(&G_1, &Size_G_1, v1);
                printf("Результат расщепления вершины:\n");
                print_graph(Size_G_1, G_1);
            }
            else {
                printf("Некорректный номер вершины.\n");
            }
            break;
        }
    } while (choice != 0);

    for (int i = 0; i < Size_G_1; i++) {
        free(G_1[i]);
    }
    free(G_1);
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
	srand((unsigned int)time(NULL));
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

void identify_vertices(int_fast8_t** graph, int* size, int v1, int v2) {
    int min_v = (v1 < v2) ? v1 : v2;
    int max_v = (v1 > v2) ? v1 : v2;


    for (int i = 0; i < *size; i++) {
        if (i != min_v && i != max_v) {
            graph[min_v][i] = graph[i][min_v] = (graph[min_v][i] || graph[max_v][i]);
        }
    }

    for (int i = max_v; i < *size - 1; i++) {
        for (int j = 0; j < *size; j++) {
            graph[i][j] = graph[i + 1][j];
        }
    }
    for (int j = max_v; j < *size - 1; j++) {
        for (int i = 0; i < *size - 1; i++) {
            graph[i][j] = graph[i][j + 1];
        }
    }

    for (int i = 0; i < *size - 1; i++) {
        if (i > min_v && i >= max_v) {
            graph[i][i - 1] = graph[i - 1][i];
        }
    }

    if (graph[min_v][max_v] && graph[max_v][min_v]) {
        graph[min_v][min_v] = 1;
    }

    (*size)--;
}

void contract_edge(int_fast8_t** graph, int* size, int v1, int v2) {
    int min_v = (v1 < v2) ? v1 : v2;
    int max_v = (v1 > v2) ? v1 : v2;

    for (int i = 0; i < *size; i++) {
        if (i != min_v && i != max_v) {
            graph[min_v][i] = graph[i][min_v] = (graph[min_v][i] || graph[max_v][i]);
        }
    }

    graph[min_v][min_v] = 0;

    for (int i = max_v; i < *size - 1; i++) {
        for (int j = 0; j < *size; j++) {
            graph[i][j] = graph[i + 1][j];
        }
    }
    for (int j = max_v; j < *size - 1; j++) {
        for (int i = 0; i < *size; i++) {
            graph[i][j] = graph[i][j + 1];
        }
    }
    (*size)--;
}

void split_vertex(int_fast8_t*** graph, int* size, int v) {
    (*size)++;

    *graph = (int_fast8_t**)realloc(*graph, *size * sizeof(int_fast8_t*));
    for (int i = 0; i < *size - 1; i++) {
        (*graph)[i] = (int_fast8_t*)realloc((*graph)[i], *size * sizeof(int_fast8_t));
        (*graph)[i][*size - 1] = 0;
    }
    (*graph)[*size - 1] = (int_fast8_t*)malloc(*size * sizeof(int_fast8_t));

    for (int i = 0; i < *size - 1; i++) {
        (*graph)[*size - 1][i] = (*graph)[v][i];
        (*graph)[i][*size - 1] = (*graph)[i][v];
    }

    (*graph)[v][*size - 1] = 1;
    (*graph)[*size - 1][v] = 1;

    (*graph)[*size - 1][*size - 1] = 0;
}