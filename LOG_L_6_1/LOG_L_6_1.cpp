#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

typedef struct Node {
	int vertex;
	struct Node* next;
} Node;

void init_graph(int size, int_fast8_t** graph_ptr, float edge_prob);
int_fast8_t** create_graph(int size);
void print_graph(int size, int_fast8_t** graph_pointer);
Node** conv_to_adj_list(int size, int_fast8_t** graph);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);

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

	srand((unsigned int)time(NULL));
	G_1 = create_graph(Size_G_1);
	G_2 = create_graph(Size_G_2);
	init_graph(Size_G_1, G_1, edge_prob_1);
	init_graph(Size_G_2, G_2, edge_prob_2);
	printf("\nМатрица смежности графа G1: \n\n");
	print_graph(Size_G_1, G_1);
	printf("\nМатрица смежности графа G2: \n\n");
	print_graph(Size_G_2, G_2);

	printf("\nСписок смежности графа G1:\n\n");
	Node** adj_list_1 = conv_to_adj_list(Size_G_1, G_1);
	print_adj_list(Size_G_1, adj_list_1);

	printf("\nСписок смежности графа G2:\n\n");
	Node** adj_list_2 = conv_to_adj_list(Size_G_2, G_2);
	print_adj_list(Size_G_2, adj_list_2);

	for (int i = 0; i < Size_G_1; i++) {
		free(G_1[i]);
	}

	for (int i = 0; i < Size_G_2; i++) {
		free(G_2[i]);
	}

	free(G_1);
	free(G_2);
	free_adj_list(Size_G_1, adj_list_1);
	free_adj_list(Size_G_2, adj_list_2);
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

void init_graph(int size, int_fast8_t** graph_ptr, float edge_prob) {
	
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

Node** conv_to_adj_list(int size, int_fast8_t** graph) {
	Node** adj_list = (Node**)malloc(size * sizeof(Node*));
	for (int i = 0; i < size; i++) {
		adj_list[i] = NULL;
		for (int j = 0; j < size; j++) {
			if (graph[i][j]) {
				Node* new_node = (Node*)malloc(sizeof(Node));
				new_node->vertex = j;
				new_node->next = adj_list[i];
				adj_list[i] = new_node;
			}
		}
	}
	return adj_list;
}

void print_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		printf("%d: ", i + 1);
		Node* current = adj_list[i];
		while (current != NULL) {
			printf("%d ", current->vertex + 1);
			current = current->next;
		}
		printf("\n");
	}
}

void free_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		Node* current = adj_list[i];
		while (current != NULL) {
			Node* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(adj_list);
}