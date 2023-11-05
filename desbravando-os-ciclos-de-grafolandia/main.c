/*
 * Desbravando os Ciclos de Grafolândia
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

// define a interface das funcoes utilizadas no programa
Edge edge(Vertex, Vertex);
Graph graph(int);
int graph_has_edge(Graph, Edge);
link list_insert(link, Vertex);
void graph_insert_edge(Graph, Edge);

Queue create_queue(int);
void enqueue(Queue *, Vertex);
Vertex dequeue(Queue *);
int is_empty(Queue *);

void check_if_has_cycle(Graph);
int has_cycle_BFS(Graph, Vertex, Vertex *, Vertex *);
void print_output(int);

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

  check_if_has_cycle(G);

  return 0;
}

Edge edge(Vertex u, Vertex v) {
  Edge e;
  e.u = u;
  e.v = v;
  return e;
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

// verifica se o grafo tem ciclo
void check_if_has_cycle(Graph G) {
  Vertex *visited = (Vertex *) malloc(G->V * sizeof(Vertex));
  Vertex *parent = (Vertex *) malloc(G->V * sizeof(Vertex));

  for (Vertex v = 0; v < G->V; v++) {
    visited[v] = 0;
    parent[v] = -1;
  }

  int has_cycle = 0;

  for (Vertex v = 0; v < G->V; v++) {
    if (visited[v] == 0) {
      if (has_cycle_BFS(G, v, visited, parent)) {
        has_cycle = 1;
        break;
      }
    }
  }

  print_output(has_cycle);
  free(visited);
  free(parent);

  return;
}

// busca em largura no grafo
int has_cycle_BFS(Graph G, Vertex v, Vertex *visited, Vertex *parent) {
  Queue q;

  q = create_queue(G->V);
  visited[v] = 1;
  enqueue(&q, v);

  while (!is_empty(&q)) {
    Vertex u = dequeue(&q);
    link list = G->adj[u];

    while (list != NULL) {
      Vertex w = list->w;
      
      if (visited[w] == 0) {
        visited[w] = 1;
        parent[w] = u;
        enqueue(&q, w);
      } else if (parent[u] != w) {
        return 1;
      }
      list = list->next;
    }
  }

  return 0;
}

void print_output(int has_cycle) {
  if (has_cycle == 1) {
    printf("GUERRA!\n");
  } else {
    printf("PAZ!\n");
  }
}