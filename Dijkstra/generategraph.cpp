#include "graph.h"

using namespace std;

int main()
{
    // Random graph
    Graph graph(10000, 100);
    graph.writeVectorToFile();
    return 0;
}
