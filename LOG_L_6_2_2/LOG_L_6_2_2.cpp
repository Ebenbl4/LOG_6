#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <locale>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

Node** create_adj_list(int size);
void initialize_adj_list(int size, Node** adj_list, float edge_prob);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
void identify_vertices(Node** adj_list, int* size, int v1, int v2);
void contract_edge(Node** adj_list, int* size, int v1, int v2);
void split_vertex(Node*** adj_list, int* size, int v);
void add_edge(Node** adj_list, int src, int dest);
bool has_edge(Node** adj_list, int src, int dest);
void remove_edge(Node** adj_list, int src, int dest);

int main(void) {
    setlocale(LC_ALL, "Russian");
    Node** adj_list;
    int size;
    float edge_prob;
	srand((unsigned int)time(NULL));

    printf("Введите количество вершин графа: ");
    if (!scanf("%d", &size) || size <= 1) {
        fprintf(stderr, "Ошибка ввода размера\n");
        exit(1);
    }

    printf("Введите вероятность создания ребра (от 0 до 100): ");
    if (!scanf("%f", &edge_prob) || edge_prob < 0 || edge_prob > 100) {
        fprintf(stderr, "Ошибка ввода вероятности\n");
        exit(1);
    }

    adj_list = create_adj_list(size);
    initialize_adj_list(size, adj_list, edge_prob);
    printf("Исходный список смежности графа:\n");
    print_adj_list(size, adj_list);

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
            if (v1 >= 0 && v1 < size && v2 >= 0 && v2 < size && v1 != v2) {
                identify_vertices(adj_list, &size, v1, v2);
                printf("Результат отождествления вершин:\n");
                print_adj_list(size, adj_list);
            }
            else {
                printf("Некорректные номера вершин.\n");
            }
            break;

        case 2:
            printf("Введите номера двух вершин для стягивания ребра: ");
            scanf("%d %d", &v1, &v2);
            v1--; v2--;
            if (v1 >= 0 && v1 < size && v2 >= 0 && v2 < size && v1 != v2 && has_edge(adj_list, v1, v2)) {
                contract_edge(adj_list, &size, v1, v2);
                printf("Результат стягивания ребра:\n");
                print_adj_list(size, adj_list);
            }
            else {
                printf("Некорректные номера вершин или между ними нет ребра.\n");
            }
            break;
        case 3:
            printf("Введите номер вершины для расщепления: ");
            scanf("%d", &v1);
            v1--;
            if (v1 >= 0 && v1 < size) {
                split_vertex(&adj_list, &size, v1);
                printf("Результат расщепления вершины:\n");
                print_adj_list(size, adj_list);
            }
            else {
                printf("Некорректный номер вершины.\n");
            }
            break;
        }
    } while (choice != 0);

    free_adj_list(size, adj_list);
    return 0;
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

Node** create_adj_list(int size) {
    Node** adj_list = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        adj_list[i] = NULL;
    }
    return adj_list;
}

void add_edge(Node** adj_list, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = adj_list[src];
    adj_list[src] = newNode;
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

bool has_edge(Node** adj_list, int src, int dest) {
    Node* current = adj_list[src];
    while (current != NULL) {
        if (current->vertex == dest) return true;
        current = current->next;
    }
    return false;
}

void remove_edge(Node** adj_list, int src, int dest) {
    Node* current = adj_list[src];
    Node* prev = NULL;

    while (current != NULL && current->vertex != dest) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            adj_list[src] = current->next;
        }
        else {
            prev->next = current->next;
        }
        free(current);
    }
}

void identify_vertices(Node** adj_list, int* size, int v1, int v2) {
    Node* current = adj_list[v2];
    while (current != NULL) {
        if (current->vertex != v1 && !has_edge(adj_list, v1, current->vertex)) {
            add_edge(adj_list, v1, current->vertex);
            add_edge(adj_list, current->vertex, v1);
        }
        current = current->next;
    }

    for (int i = 0; i < *size; i++) {
        remove_edge(adj_list, i, v2);
    }

    for (int i = v2; i < *size - 1; i++) {
        adj_list[i] = adj_list[i + 1];
    }

    for (int i = 0; i < *size - 1; i++) {
        Node* current = adj_list[i];
        while (current != NULL) {
            if (current->vertex > v2) {
                current->vertex--;
            }
            current = current->next;
        }
    }

    (*size)--;
}

void contract_edge(Node** adj_list, int* size, int v1, int v2) {
    Node* current = adj_list[v2];
    while (current != NULL) {
        if (current->vertex != v1 && !has_edge(adj_list, v1, current->vertex)) {
            add_edge(adj_list, v1, current->vertex);
            add_edge(adj_list, current->vertex, v1);
        }
        current = current->next;
    }

    for (int i = 0; i < *size; i++) {
        remove_edge(adj_list, i, v2);
    }

    Node* temp = adj_list[v2];
    while (temp != NULL) {
        Node* next = temp->next;
        free(temp);
        temp = next;
    }

    for (int i = v2; i < *size - 1; i++) {
        adj_list[i] = adj_list[i + 1];
    }

    for (int i = 0; i < *size - 1; i++) {
        Node* current = adj_list[i];
        while (current != NULL) {
            if (current->vertex > v2) {
                current->vertex--;
            }
            current = current->next;
        }
    }

    (*size)--;
}

void split_vertex(Node*** adj_list, int* size, int v) {
    *adj_list = (Node**)realloc(*adj_list, (*size + 1) * sizeof(Node*));
    (*adj_list)[*size] = NULL;

    Node* current = (*adj_list)[v];
    int edge_count = 0;
    while (current != NULL) {
        edge_count++;
        current = current->next;
    }

    int edges_to_move = edge_count / 2;
    current = (*adj_list)[v];

    for (int i = 0; i < edges_to_move && current != NULL; i++) {
        add_edge(*adj_list, *size, current->vertex);
        add_edge(*adj_list, current->vertex, *size);

        int vertex_to_remove = current->vertex;
        current = current->next;
        remove_edge(*adj_list, v, vertex_to_remove);
        remove_edge(*adj_list, vertex_to_remove, v);
    }

    add_edge(*adj_list, v, *size);
    add_edge(*adj_list, *size, v);

    (*size)++;
}