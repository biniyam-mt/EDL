#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <algorithm>
#include "graph.h"

using namespace std;
vector<int> tripCountFreq;
void init_freq(int n)
{
    cout << "Initializing frequency table...\n";
    for (int i = 0; i < n; i++)
    {
        tripCountFreq.push_back(0);
    }
}

// Function to perform Dijkstra's algorithm
void dijkstra(Graph *graph, int start)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distances(graph->numNodes, INT_MAX);

    pq.push(make_pair(0, start));
    distances[start] = 0;
    int count = 0;
    while (!pq.empty())
    {
        int curNode = pq.top().second;
        pq.pop();

        for (Node neighbor : graph->adjacencyList[curNode])
        {
            int nextNode = neighbor.destination;
            int edgeWeight = neighbor.weight;

            if (distances[nextNode] > distances[curNode] + edgeWeight)
            {
                distances[nextNode] = distances[curNode] + edgeWeight;
                pq.push(make_pair(distances[nextNode], nextNode));
            }
        }
        count++;
    }
    // cout << "Count: " << count << endl;
    tripCountFreq[count]++;
}

int main()
{
    // From file
    string data = "rand_graph_1000";
    string filename = "data/" + data + ".edges";

    Graph *graph = new Graph(filename);

    init_freq(graph->numEdges * 2);
    int executionCount = 10000;

    // Perform Dijkstra's algorithm starting from node 0
    for (int i = 0; i < executionCount; i++)
    {
        int start = randNumber(1, graph->numNodes - 1);
        float progress = static_cast<float>(i + 1) / executionCount;
        cout << i + 1 << "/" << executionCount;
        printProgressBar(progress, " computing: ");
        dijkstra(graph, start);
    }
    string path = "./output/freq_" + data + ".csv";
    writeVectorToCSV(tripCountFreq, path);

    return 0;
}
