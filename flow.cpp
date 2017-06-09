#include <iostream>
#include <vector>

class Edge {
public:
	size_t from, to;
	int flow, capacity;
	size_t reverse_edge_index;

	Edge(size_t from, size_t to, int capacity, size_t reverse_edge_index) :
		from(from),
		to(to),
		flow(0),
		capacity(capacity),
		reverse_edge_index(reverse_edge_index)
	{}
};

typedef std::vector< int > Indexes;
typedef std::vector< Indexes > Graph;
typedef std::vector< Edge > Edges;

const int INF = 1000 * 1000 * 1000;

int min(int a, int b) {
	return (a < b ? a : b);
}

int flow_bfs(Edges * edges, const Graph & graph, const size_t source, const size_t target) {
	const int graph_size = graph.size();

	std::vector< int > max_flow(graph_size, INF);

	Indexes reverse_parent_edge_indexes(graph_size);
	std::vector< char > used(graph_size, false);

	Indexes queue;
	queue.push_back(source);

	for (size_t queue_index = 0; queue_index < queue.size(); ++queue_index) {
		size_t from = queue[queue_index];
		int max_from_flow = max_flow[from];

		for (size_t index = 0; index < graph[from].size(); ++index) {
			size_t edge_index = graph[from][index];
			Edge * edge = &edges->operator[](edge_index);

			size_t to = edge->to;
			int possible_edge_flow = edge->capacity - edge->flow;

			if (possible_edge_flow > 0 && !used[to]) {
				max_flow[to] = min(max_from_flow, possible_edge_flow);

				reverse_parent_edge_indexes[to] = edge->reverse_edge_index;
				used[to] = true;

				queue.push_back(to);
			}
		}
	}

	if (max_flow[target] == INF) return 0;

	int flow_delta = max_flow[target];
	for (size_t cur = target; cur != source; ) {
		Edge * reverse_parent_edge = &edges->operator[](reverse_parent_edge_indexes[cur]);
		size_t parent = reverse_parent_edge->to;

		Edge * parent_edge = &edges->operator[](reverse_parent_edge->reverse_edge_index);

		parent_edge->flow += flow_delta;
		reverse_parent_edge->flow -= flow_delta;

		cur = parent;
	}

	return flow_delta;
}

void add_edge(Edges * edges, Graph * graph, const size_t from, const size_t to, const int capacity){
	size_t from_to_edge_index = edges->size();
	size_t to_from_edge_index = from_to_edge_index + 1;

	Edge from_to_edge(from, to, capacity, to_from_edge_index);
	Edge to_from_edge(to, from, 0, from_to_edge_index);

	edges->push_back(from_to_edge);
	edges->push_back(to_from_edge);

	graph->operator[](from).push_back(from_to_edge_index);
	graph->operator[](to).push_back(to_from_edge_index);
}


int find_max_flow(Edges * edges, const Graph & graph, const size_t source, const size_t target) {
	int flow = 0;
	while (true) {
		int flow_delta = flow_bfs(edges, graph, source, target);
		if (flow_delta == 0) {
			break;
		} else {
			flow += flow_delta;
		}
	}
	return flow;
}

void add_capacities(Edges * edges, Graph * graph, const size_t students_count, const size_t target, const int capacity) {
	for (size_t i = 1; i <= students_count; ++i) {
		add_edge(edges, graph, i, target, capacity);
	}
}

int main() {
	int nodes_count, edges_count;

	std::cin >> nodes_count >> edges_count;

	Graph graph(nodes_count, Indexes());
	Edges edges;

	for (size_t i = 0; i < edges_count; i++) {
		int from, to, cap;
		std::cin >> from >> to >> cap;

		add_edge(&edges, &graph, from - 1, to - 1, cap);
	}


	int max_flow = find_max_flow(&edges, graph, 0, nodes_count - 1);
	std::cout <<  max_flow << "\n";

	for (size_t i = 0; i < edges_count * 2; i+=2) {
		std::cout << edges[i].flow;
        if (i + 2 < edges_count * 2) {
            std::cout << " ";
        }
	}

	std::cout << "\n";
	return 0;
}
