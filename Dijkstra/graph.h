#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <algorithm>
#include "../util.h"

using namespace std;

// Structure to represent a node in the graph
struct Node
{
    int destination;
    int weight;
    Node(int dest, int w) : destination(dest), weight(w) {}
};

// Structure to represent a graph
class Graph
{
public:
    int numNodes;
    int numEdges;
    vector<vector<Node>> adjacencyList;

    bool edgeExists(int node1, int node2)
    {
        // Iterate over the adjacency list of node1
        for (const auto &neighbor : adjacencyList[node1])
        {
            // Check if node2 is a destination in the adjacency list of node1
            if (neighbor.destination == node2)
            {
                return true; // Edge exists
            }
        }
        for (const auto &neighbor : adjacencyList[node2])
        {
            // Check if node2 is a destination in the adjacency list of node1
            if (neighbor.destination == node1)
            {
                return true; // Edge exists
            }
        }
        return false; // Edge does not exist
    }
    // Graph(int nmOfNodes, int maxWeight)
    Graph(int n, int maxWeight) : numNodes(n), adjacencyList(n)
    {
        int maxEdge = (n * (n - 1)) / 2;
        int randEdge = randNumber(0, maxEdge);
        numEdges = randEdge;

        cout << "size: " << n << endl;
        cout << "maxEdge: " << maxEdge << endl;
        cout << "randEdge: " << randEdge << endl;
        cout << "adj List size: " << adjacencyList.size() << endl;

        int count = 0;

        while (count < randEdge)
        {
            // cout << count << endl;
            float progress = static_cast<float>(count) / randEdge;
            printProgressBar(progress, " Creating Graph: ");
            int startNode = randNumber(0, numNodes - 1);
            int endNode = randNumber(0, numNodes - 1);
            int weight = randNumber(0, maxWeight);

            if (startNode != endNode && !edgeExists(startNode, endNode))
            {
                // cout << count << " " << startNode;
                // cout << " " << endNode;
                // cout << " " << weight << endl;
                addEdge(startNode, endNode, weight);
                count++;
            }
        }
    }
    // Graph(filename)
    Graph(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            exit(EXIT_FAILURE);
        }

        // Read the number of nodes from the first line
        file >> numNodes >> numEdges;

        // Resize the adjacency list to the specified number of nodes
        adjacencyList.resize(numNodes);

        int start, end, weight;

        // Read each line and add edges to the graph
        while (file >> start >> end >> weight)
        {
            addEdge(start, end, weight);
        }

        file.close();
    }

    // Function to add an edge to the graph
    void addEdge(int start, int end, int weight)
    {
        adjacencyList[start].emplace_back(end, weight);
    }
    void printAdjacencyList() const
    {
        cout << "Adjacency List:\n";
        for (int i = 0; i < numNodes; ++i)
        {
            cout << "Node " << i << ": ";
            for (const Node &neighbor : adjacencyList[i])
            {
                cout << "(" << neighbor.destination << ", " << neighbor.weight << ") ";
            }
            cout << "\n";
        }
    }

    void hasTwoWayEdge()
    {
        // Iterate over each node in the adjacency list
        for (int node1 = 0; node1 < adjacencyList.size(); ++node1)
        {
            // Iterate over neighbors of node1
            for (const auto &neighbor : adjacencyList[node1])
            {
                int node2 = neighbor.destination;
                // Check if node2 is connected to node1
                if (std::find_if(adjacencyList[node2].begin(), adjacencyList[node2].end(),
                                 [node1](const Node &n)
                                 { return n.destination == node1; }) != adjacencyList[node2].end())
                {
                    // If node2 is connected to node1, return true (at least one two-way edge)
                    cout << "Node " << node1 << " and Node " << node2 << " are connected\n";
                    return;
                }
            }
        }
        // If no two-way edges found, return false
        cout << "No two-way edges found\n";
    }
    static bool compareNodes(const Node &node1, const Node &node2)
    {
        return node1.destination < node2.destination;
    }

    void writeVectorToFile()
    {
        string fileName = "./data/rand_graph_" + to_string(numNodes) + ".edges";
        // Open the file for writing
        ofstream outputFile(fileName);

        // Check if the file is successfully opened
        if (!outputFile.is_open())
        {
            cerr << "Error opening the file: " << fileName << endl;
            return;
        }

        // Write header to the CSV file
        outputFile << numNodes << " " << numEdges << endl;

        for (int i = 0; i < numNodes; ++i)
        {
            sort(adjacencyList[i].begin(), adjacencyList[i].end(), compareNodes);
            for (const Node &neighbor : adjacencyList[i])
            {
                outputFile << i << " " << neighbor.destination << " " << neighbor.weight << endl;
            }
        }
        outputFile.close();

        cout << "CSV file has been created: " << fileName << endl;
    }
};