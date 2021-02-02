#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <windows.h>
using namespace std;
#define CHECK_TIME_START QueryPerformanceFrequency ((_LARGE_INTEGER*)&freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a=(float)((float) (end - start)/freq)
#define SWAP(a, b) {struct Edge temp = a; a = b; b = temp;}
void qpush(int vertex, struct QNode** front, struct QNode** back);
int qpop(struct QNode** front, struct QNode** back);
void make_heap(struct Edge* edges, long long size);
struct Edge pop_heap(struct Edge* edges, long long* size);
static void adjust(struct Edge* edges, int index, int n);
int Find(int* parent, int v);
void Union(int* parent, int v1, int v2);

struct GraphNode {
	int to, weight;
	struct GraphNode* link;
};
struct Component {
	int n_vertices;
	long long sum;
};
struct QNode {
	int vertex;
	struct QNode* link;
};
struct Edge {
	int v1, v2, weight;
	bool operator < (const struct Edge& other) const{
		return this->weight < other.weight;
	}
};
void qpush(int vertex, struct QNode** front, struct QNode** back) {
	struct QNode* node = (struct QNode*)malloc(sizeof(struct QNode));
	node->vertex = vertex;
	node->link = NULL;
	if (*front == NULL) {
		*front = node;
		*back = node;
	}
	else {
		(*back)->link = node;
		*back = node;
	}
}
int qpop(struct QNode** front, struct QNode** back) {
	struct QNode* temp = *front;
	int vertex = temp->vertex;
	*front = (*front)->link;
	free(temp);
	return vertex;
}
void make_heap(struct Edge* edges, long long size) {
	for (int i = size / 2; i >= 1; i--) {
		adjust(edges, i, size);
	}
}
static void 
adjust(struct Edge* edges, int index, int n) {
	struct Edge cur = edges[index];
	int child = index * 2;
	while (child <= n) {
		if (child < n && edges[child + 1] < edges[child])
			child++;
		if (edges[child] < cur) {
			edges[child / 2] = edges[child];
			child *= 2;
		}
		else break;
	}
	edges[child / 2] = cur;
}
struct Edge pop_heap(struct Edge* edges, long long* size) {
	struct Edge top = edges[1];
	SWAP(edges[1], edges[*size]);
	(*size)--;
	adjust(edges, 1, *size);
	return top;
}
int Find(int* parent, int v) {
	if (parent[v] == -1) return v;
	return parent[v] = Find(parent, parent[v]);
}
void Union(int* parent, int v1, int v2) {
	parent[Find(parent, v1)] = Find(parent, v2);
}

int main() {
	__int64 start, freq, end;
	double result_time = 0;

	int n_vertices, MAX_WEIGHT, n_components;
	long long n_edges;
	string iname, oname;
	struct GraphNode** graph;
	struct Edge* edges; //starts from index 1 since it is a heap
	struct Component* component_info;
	int* component;
	int* parent;
	int kscanned;

	/* input */
	ifstream infile("commands.txt");
	infile >> iname >> oname;
	infile.close();
	infile.open(iname);
	infile >> n_vertices >> n_edges >> MAX_WEIGHT;

	graph = (struct GraphNode**)malloc(sizeof(struct GraphNode*) * n_vertices);
	for (int i = 0; i < n_vertices; i++)
		graph[i] = NULL;
	edges = (struct Edge*)malloc((n_edges + 1) * sizeof(struct Edge));
	for (int index = 1; index <= n_edges; index++) {
		int v1, v2, weight;
		infile >> v1 >> v2 >> weight;
		//add to v1
		struct GraphNode* node = (struct GraphNode*)malloc(sizeof(struct GraphNode));
		node->to = v2;
		node->weight = weight;
		node->link = graph[v1];
		graph[v1] = node;
		//add to v2
		node = (struct GraphNode*)malloc(sizeof(struct GraphNode));
		node->to = v1;
		node->weight = weight;
		node->link = graph[v2];
		graph[v2] = node;
		//add edge
		edges[index].v1 = v1;
		edges[index].v2 = v2;
		edges[index].weight = weight;
	}
	infile.close();

	CHECK_TIME_START;
	/* mark components */
	n_components = 0;
	component_info = (struct Component*)malloc(n_vertices* sizeof(struct Component));
	component = (int*)malloc(n_vertices * sizeof(int));
	for (int i = 0; i < n_vertices; i++)
		component[i] = -1;
	for (int i = 0; i < n_vertices; i++) {
		if (component[i] == -1) {
			//bfs
			int cnt = 0;
			struct QNode* front, * back;
			front = back = NULL;
			qpush(i, &front, &back);
			component[i] = n_components;
			cnt++;
			while (front != NULL) {
				int v = qpop(&front, &back);
				for (struct GraphNode* node = graph[v]; node != NULL; node = node->link) {
					int next = node->to;
					if (component[next] == -1) {
						component[next] = n_components;
						qpush(next, &front, &back);
						cnt++;
					}
				}
			}
			n_components++;
			component_info[n_components - 1].n_vertices = cnt;
			component_info[n_components - 1].sum = 0;
		}
	}
	//erase graph since we don't need it anymore
	for (int i = 0; i < n_vertices; i++) {
		struct GraphNode* ptr = graph[i];
		while (ptr != NULL) {
			struct GraphNode* temp = ptr;
			ptr = ptr->link;
			free(temp);
		}
	}
	free(graph);

	/* make set */
	parent = (int*)malloc(n_vertices * sizeof(int));
	for (int i = 0; i < n_vertices; i++)
		parent[i] = -1;

	/* make spanning tree */
	//CHECK_TIME_START;
	make_heap(edges, n_edges);
	//CHECK_TIME_END(result_time);
	int n_tree_edge = 0;//the number of tree edges in MST so far
	long long size = n_edges;
	for (int i = 0; i < n_edges; i++) {
		struct Edge e = pop_heap(edges, &size);
		int root1 = Find(parent, e.v1);
		int root2 = Find(parent, e.v2);
		if (root1 != root2) {
			Union(parent, e.v1, e.v2);
			int com = component[root1];
			component_info[com].sum += e.weight;
			n_tree_edge++;
			if (n_tree_edge >= n_vertices - n_components) {
				kscanned = i + 1;
				break;
			}
		}
	}
	//free
	free(edges);
	free(parent);
	free(component);

	CHECK_TIME_END(result_time);

	/* evaluate answer */
	ofstream outFile(oname);
	outFile << n_components << '\n';
	int max, maxIdx;
	max = maxIdx = -1;
	for (int i = 0; i < n_components; i++) {
		outFile << component_info[i].n_vertices << " " << component_info[i].sum << '\n';
		if (max < component_info[i].n_vertices) {
			max = component_info[i].n_vertices;
			maxIdx = i;
		}
	}
	outFile.close();
	//print
	cout << "file name\t\t\tsuccess\t\tMST weight\ttime\t\tkscanned\n";
	cout << iname << '\t' << "YES\t\t" << component_info[maxIdx].sum << '\t' << result_time << "\t" <<  kscanned << '\n';
	return 0;
}