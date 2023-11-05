/*
 * Aventura Bipartida em um Mundo Conectado
 * Nome:  Guilherme de Sousa Santos
 * RA:    11201921175
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// define alguns tipos de dados
typedef int Vertex;
typedef struct edge Edge;
typedef struct graph *Graph;
typedef struct node *link;
typedef struct queue Queue;

// define as estruturas dos dados
struct edge {
  Vertex u;
  Vertex v;
};

struct graph {
  int V;
  int E;
  link *adj;
};

struct node {
  Vertex w;
  link next;
};

struct queue {
  int size;
  int capacity;
  Vertex *array;
};

// define a interface das funcoes
Edge edge(Vertex, Vertex);
Graph graph(int);
int graph_has_edge(Graph, Edge);
link list_insert(link, Vertex);
void graph_insert_edge(Graph, Edge);

Queue create_queue(int);
void enqueue(Queue *, Vertex);
Vertex dequeue(Queue *);
int is_empty(Queue *);

void check_if_bipartite(Graph);
int is_bipartite_BFS(Graph, Vertex, Vertex *, Vertex *, Vertex *);
void print_bipartitions(int, Vertex *);
void print_non_bipartite_path(Graph, Vertex *);

int main(void) {
  int n_habitants, n_conexions;
  Graph G;

  scanf("%d %d", &n_habitants, &n_conexions);

  G = graph(n_habitants);

  for (int i = 0; i < n_conexions; i++) {
    int u, v;
    scanf("%d %d", &u, &v);

    Edge e1 = edge(u, v);
    graph_insert_edge(G, e1);
    Edge e2 = edge(v, u);
    graph_insert_edge(G, e2);
  }

  check_if_bipartite(G);

  return 0;
}

Edge edge(Vertex u, Vertex v) {
  Edge e;
  e.u = u;
  e.v = v;
  return e;
}

int graph_has_edge(Graph G, Edge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);

  for (link p = G->adj[e.u]; p != NULL; p = p->next) {
    if (p->w == e.v) {
      return 1;
    }
  }

  return 0;
}

Graph graph(int V) {
  Graph G = malloc(sizeof(*G));

  G->V = V;
  G->E = 0;
  G->adj = malloc(V * sizeof(link));

  for (Vertex u = 0; u < V; u++) {
    G->adj[u] = NULL;
  }

  return G;
}

link list_insert(link head, Vertex w) {
  link p = malloc(sizeof(*p));
  p->w = w;
  p->next = head;
  return p;
}

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);

  if (!graph_has_edge(G, e)) {
    G->adj[e.u] = list_insert(G->adj[e.u], e.v);
    G->adj[e.v] = list_insert(G->adj[e.v], e.u);
    G->E += 1;
  }
}

Queue create_queue(int capacity) {
  Queue q;
  q.size = 0;
  q.capacity = capacity;
  q.array = malloc(capacity * sizeof(int));
  return q;
}

void enqueue(Queue *q, Vertex w) {
  assert(q->size < q->capacity);

  q->array[q->size] = w;
  q->size += 1;
  return;
}

Vertex dequeue(Queue *q) {
  assert(q->size > 0);

  Vertex w = q->array[0];
  q->size -= 1;

  for (int i = 0; i < q->size; i++) {
    q->array[i] = q->array[i + 1];
  }

  return w;
}

int is_empty(Queue *q) {
  return q->size == 0;
}

void check_if_bipartite(Graph G) {
  Vertex *color = (Vertex *) malloc(G->V * sizeof(Vertex));
  Vertex *conflict = (Vertex *) malloc(G->V * sizeof(Vertex));
  Vertex *parent = (Vertex *) malloc(G->V * sizeof(Vertex));

  for (Vertex v = 0; v < G->V; v++) {
    color[v] = -1;
    conflict[v] = 0;
    parent[v] = -1;
  }

  int has_conflict = 0;

  for (Vertex v = 0; v < G->V; v++) {
    if (color[v] == -1) {
      if (!is_bipartite_BFS(G, v, color, conflict, parent)) {
        has_conflict = 1;
        break;
      }
    }
  }

  if (!has_conflict) {
    // imprime as biparticoes se nao ha conflito
    print_bipartitions(G->V, color);
  } else {
    // imprime um caminho que comprove o impedimento
    print_non_bipartite_path(G, conflict);
  }

  free(color);
  free(conflict);

  return;
}

// verifica se o grafo eh bipartido usando BFS
int is_bipartite_BFS(Graph G, Vertex v, Vertex *color, Vertex *conflict, Vertex *parent) {
  Queue q;

  q = create_queue(G->V);
  color[v] = 0;
  enqueue(&q, v);

  while (!is_empty(&q)) {
    Vertex u = dequeue(&q);
    link list = G->adj[u];

    while (list != NULL) {
      Vertex w = list->w;

      if (color[w] == -1) {
        color[w] = 1 - color[u];
        parent[w] = u;
        enqueue(&q, w);
      } else if (color[w] == color[u]) {
        conflict[u] = 1; // vertice conflitando cor
        conflict[w] = 1; // vertice conflitando cor
        return 0;
      }
      list = list->next;
    }
  }

  return 1;
}

void print_bipartitions(int V, Vertex *color) {
  int *bipartition1 = (int *) malloc(V * sizeof(int));
  int *bipartition2 = (int *) malloc(V * sizeof(int));
  int b1_count = 0, b2_count = 0;

  for (Vertex i = 0; i < V; i++) {
    if (color[i] == 0) {
      bipartition1[b1_count++] = i;
    } else {
      bipartition2[b2_count++] = i;
    }
  }

  printf("PAZ!\n");
  for (Vertex i = 0; i < b1_count; i++) {
    printf("%d ", bipartition1[i]);
  }
  printf("\n");

  for (Vertex i = 0; i < b2_count; i++) {
    printf("%d ", bipartition2[i]);
  }
  printf("\n");
}

void print_non_bipartite_path(Graph G, Vertex *conflict) {
  printf("GUERRA!\n");

  for (Vertex v = 0; v < G->V; v++) {
    if (conflict[v] == 1) {
      link list = G->adj[v];
      Vertex w = list->w;
      printf("%d %d %d\n", v, w, w);
      break;
    }
  }
}