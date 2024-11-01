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

Node** create_adj_list(int size);
void add_edge(Node** adj_list, int src, int dest);
void initialize_adj_list(int size, Node** adj_list, float edge_prob);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
int has_edge(Node** adj_list, int src, int dest);
Node** unite_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2);
Node** intersect_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2);
Node** ring_sum_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2);

int main(void) {
	setlocale(LC_ALL, "Russian");
	srand((unsigned int)time(NULL));
	int Size_G_1, Size_G_2;
	float edge_prob_1, edge_prob_2;

	fprintf(stdout, "Введите количество вершин в графе G1: ");
	if (!scanf("%d", &Size_G_1) || Size_G_1 <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите количество вершин в графе G2: ");
	if (!scanf("%d", &Size_G_2) || Size_G_2 <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите вероятность создания ребра в графе G1 (от 0 до 100): ");
	if (!scanf("%f", &edge_prob_1) || edge_prob_1 < 0 || edge_prob_1 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}
	fprintf(stdout, "Введите вероятность создания ребра в графе G2 (от 0 до 100): ");
	if (!scanf("%f", &edge_prob_2) || edge_prob_2 < 0 || edge_prob_2 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	Node** adj_list_1 = create_adj_list(Size_G_1);
	Node** adj_list_2 = create_adj_list(Size_G_2);
	initialize_adj_list(Size_G_1, adj_list_1, edge_prob_1);
	initialize_adj_list(Size_G_2, adj_list_2, edge_prob_2);
	printf("\nСписок смежности графа G1:\n\n");
	print_adj_list(Size_G_1, adj_list_1);
	printf("\nСписок смежности графа G2:\n\n");
	print_adj_list(Size_G_2, adj_list_2);

	int max_size = (Size_G_1 > Size_G_2) ? Size_G_1 : Size_G_2;
	Node** united_graph = unite_graphs(adj_list_1, adj_list_2, Size_G_1, Size_G_2);
	printf("\nСписок смежности объединения графов G1 и G2:\n\n");
	print_adj_list(max_size, united_graph);

	int min_size = (Size_G_1 < Size_G_2) ? Size_G_1 : Size_G_2;
	Node** intersected_graph = intersect_graphs(adj_list_1, adj_list_2, Size_G_1, Size_G_2);
	printf("\nСписок смежности пересечения графов G1 и G2:\n\n");
	print_adj_list(min_size, intersected_graph);

	Node** ring_sum_graph = ring_sum_graphs(adj_list_1, adj_list_2, Size_G_1, Size_G_2);
	printf("\nСписок смежности кольцевой суммы графов G1 и G2:\n\n");
	print_adj_list(max_size, ring_sum_graph);

	free_adj_list(Size_G_1, adj_list_1);
	free_adj_list(Size_G_2, adj_list_2);
	free_adj_list(max_size, united_graph);
	free_adj_list(min_size, intersected_graph);
	free_adj_list(max_size, ring_sum_graph);

	return 0;
}

Node** create_adj_list(int size) {
	Node** adj_list = (Node**)malloc(size * sizeof(Node*));
	for (int i = 0; i < size; i++) {
		adj_list[i] = NULL;
	}
	return adj_list;
}

void initialize_adj_list(int size, Node** adj_list, float edge_prob) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_prob) {
				add_edge(adj_list, i, j);
				add_edge(adj_list, j, i);
			}
		}
	}
}

void add_edge(Node** adj_list, int src, int dest) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->vertex = dest;
	newNode->next = adj_list[src];
	adj_list[src] = newNode;
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

int has_edge(Node** adj_list, int src, int dest) {
	Node* current = adj_list[src];
	while (current != NULL) {
		if (current->vertex == dest) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

Node** unite_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2) {
	int max_size = (size_1 > size_2) ? size_1 : size_2;
	Node** result = create_adj_list(max_size);

	for (int i = 0; i < size_1; i++) {
		Node* current = adj_list_1[i];
		while (current != NULL) {
			add_edge(result, i, current->vertex);
			current = current->next;
		}
	}

	for (int i = 0; i < size_2; i++) {
		Node* current = adj_list_2[i];
		while (current != NULL) {
			if (!has_edge(result, i, current->vertex)) {
				add_edge(result, i, current->vertex);
			}
			current = current->next;
		}
	}

	return result;
}

Node** intersect_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2) {
	int min_size = (size_1 < size_2) ? size_1 : size_2;
	Node** result = create_adj_list(min_size);

	for (int i = 0; i < min_size; i++) {
		for (int j = i + 1; j < min_size; j++) {
			if (has_edge(adj_list_1, i, j) && has_edge(adj_list_2, i, j)) {
				add_edge(result, i, j);
				add_edge(result, j, i);
			}
		}
	}

	return result;
}

Node** ring_sum_graphs(Node** adj_list_1, Node** adj_list_2, int size_1, int size_2) {
	int max_size = (size_1 > size_2) ? size_1 : size_2;
	Node** result = create_adj_list(max_size);

	for (int i = 0; i < max_size; i++) {
		for (int j = i + 1; j < max_size; j++) {
			bool edge_in_g1 = (i < size_1 && j < size_1) ? has_edge(adj_list_1, i, j) : false;
			bool edge_in_g2 = (i < size_2 && j < size_2) ? has_edge(adj_list_2, i, j) : false;

			if (edge_in_g1 != edge_in_g2) {
				add_edge(result, i, j);
				add_edge(result, j, i);
			}
		}
	}

	return result;
}