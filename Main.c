#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct AdjListNode
{
	int dest;
	struct AdjListNode* next;
};

struct AdjList
{
	bool* domain;
	int final_color;
	struct AdjListNode* head;
};

struct Graph
{
	int V;
	int domain_count;
	int colored_count;
	struct AdjList* array;
};
typedef struct Graph graph_t;

struct AdjListNode* newAdjListNode(int dest)
{
	struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

struct Graph* createGraph(int V, int c)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;
	graph->domain_count = c;
	graph->colored_count = 0;

	graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

	for (int i = 0; i < V; ++i)
	{
		graph->array[i].head = NULL;
		graph->array[i].domain = (bool*) malloc(c * sizeof(bool));
		graph->array[i].final_color = 0;
		for (int j = 1; j <= c; ++j)
		{
			graph->array[i].domain[j] = true;
		}
	}

	return graph;
}

void addEdge(struct Graph* graph, int src, int dest)
{
	struct AdjListNode* newNode = newAdjListNode(dest);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;
	newNode = newAdjListNode(src);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

void printGraph(struct Graph* graph)
{
	int v;
	for (v = 0; v < graph->V; ++v)
	{
		struct AdjListNode* pCrawl = graph->array[v].head;
		printf("\n Adjacency list of vertex %d\n head ", v);
		while (pCrawl)
		{
			printf("-> %d", pCrawl->dest);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}

bool check_constraint(int curr_v, graph_t* g, int curr_c)
{
	struct AdjListNode* temp = g->array[curr_v].head;
	while (temp)
	{
		if (g->array[temp->dest].final_color == curr_c) return false;
		temp = temp->next;
	}
	return true;
}

bool forward_checking(int curr_v, graph_t* g, int curr_c)
{
	struct AdjListNode* temp = g->array[curr_v].head;
	while (temp)
	{
		if (g->array[temp->dest].domain[curr_c - 1])
		{
			g->array[temp->dest].domain[curr_c - 1] = false;
			bool all_false = true;
			for (int i = 0; i < g->domain_count; i++)
			{
				if (g->array[temp->dest].domain[i]) all_false = false;
			}
			if (all_false == true) return false;
		}
		temp = temp->next;
	}
	return true;
}

bool solve_csp_util(graph_t* g, int curr_v)
{
	if (g->colored_count == g->V) return true;
	//if (curr_v == g->V) return true;
	for (int curr_c = 1; curr_c <= g->domain_count; curr_c++)
	{
		if (check_constraint(curr_v, g, curr_c))
		{
			if (forward_checking(curr_v, g, curr_c))
			{
				g->array[curr_v].final_color = curr_c;
				g->colored_count = g->colored_count + 1;

				//mrv
				int new_v = -1;
				int false_count = 0;
				int max_false_count = 0;
				for (int i = 0; i < g->V; ++i)
				{
					false_count = 0;
					if (i != curr_v && g->array[i].final_color == 0)
					{
						for (int j = 0; j < g->domain_count; j++)
						{
							if (g->array[i].domain[j] == false) false_count++;
						}
						if (false_count != 0)
						{
							if (false_count > max_false_count)
							{
								max_false_count = false_count;
								new_v = i;
							}
						}
						else
						{
							new_v = i;
						}
					}
				}
				curr_v = new_v;

				if (solve_csp_util(g, curr_v))
				{
					return true;
				}
				g->array[curr_v].final_color = 0;
			}
		}
	}
	return false;
}

void print_solution(graph_t *g)
{
	printf("Solution is:\n");
	for (int i = 0; i < g->V; i++)
	{
		printf("Node %d: %d\n", i, g->array[i].final_color);
	}
}

bool solve_csp(graph_t* g)
{
	if (!solve_csp_util(g, 0))
		return false;

	print_solution(g);
	return true;
}

int main()
{
	// read array from file
	char vertices_count_s[10];
	char color_count_s[10];
	char link_s[3];
	FILE* fptr;

	if ((fptr = fopen("Map.txt", "r")) == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	fscanf(fptr, "%s %s\n", vertices_count_s, color_count_s);

	// number of vertices
	int vertices_count = atoi(vertices_count_s);

	// color count
	int color_count = atoi(color_count_s);

	// create constraint graph
	struct Graph* graph = createGraph(vertices_count, color_count);

	int** array = (int**)malloc(vertices_count * sizeof(int*));

	for (int i = 0; i < vertices_count; ++i)
	{
		array[i] = (int*)malloc(vertices_count * sizeof(int));
		for (int j = 0; j < vertices_count; ++j)
		{
			fscanf(fptr, "%s", link_s);
			array[i][j] = atoi(link_s);
		}
	}

	for (int i = 1; i < vertices_count; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if(array[i][j] == 1) addEdge(graph, i, j);
		}
	}
	//printGraph(graph);

	FILE* fptr2 = fopen("Colored.json", "w");

	clock_t begin = clock();
	bool func = solve_csp(graph);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Elapsed: %f miliseconds\n", time_spent*1000);

	if (func == true)
	{
		fprintf(fptr2, "vertices = \'[");
		for (int i = 0; i < graph->V; ++i)
		{
			fprintf(fptr2, "{\"color\" : %d", graph->array[i].final_color);
			if(i == graph->V - 1) fprintf(fptr2, "}");
			else fprintf(fptr2, "},");
			//graph->array[i].final_color
		}
		fprintf(fptr2, "]\';");
		printf("A solution was found. Job finished!\n");
	}
	else
	{
		fprintf(fptr2, "No solution found");
		printf("No solution found\n");
	}
	return 0;
}
