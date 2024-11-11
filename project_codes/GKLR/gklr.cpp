#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <algorithm>


struct Vertex {
    int id;
    std::unordered_set<int> neighbors;  // List of neighboring vertex IDs
};


struct Partition {
    std::unordered_set<int> vertices;
    int getSize() const { return vertices.size(); }
};


class Graph {
public:
    Graph(int numVertices, int numPartitions, int maxBalance)
        : numPartitions(numPartitions), maxBalance(maxBalance) {
        vertices.resize(numVertices);
        partitions.resize(numPartitions);
    }

    void addEdge(int u, int v) {
        vertices[u].neighbors.insert(v);
        vertices[v].neighbors.insert(u);
    }

    void assignToPartition(int vertexId, int partitionId) {
        partitions[partitionId].vertices.insert(vertexId);
        vertexToPartition[vertexId] = partitionId;
    }

    void runKLRefinement() {
        bool improvement = true;
        int numIterations = 0;
        while (improvement) {
            numIterations++;
            improvement = false;
            for (int i = 0; i < numPartitions; ++i) {
                for (int j = i + 1; j < numPartitions; ++j) {
                    improvement |= refineBetweenPartitions(i, j);
                }
            }
            if (numIterations > 100) {
                break;
            }
        }
    }

    void printPartitions() const {
        for (int i = 0; i < numPartitions; ++i) {
            std::cout << "Partition " << i << ": ";
            for (int vertex : partitions[i].vertices) {
                std::cout << vertex << " ";
            }
            std::cout << "\n";
        }
    }

private:
    int numPartitions;
    int maxBalance;
    std::vector<Vertex> vertices;
    std::vector<Partition> partitions;
    std::unordered_map<int, int> vertexToPartition;

    bool refineBetweenPartitions(int p, int q) {
        std::vector<int> gains;
        std::unordered_set<int> locked;

        // Calculate initial gains and select vertices to swap
        calculateGains(p, q, gains, locked);

        int maxGain = 0;
        int cumulativeGain = 0;
        bool improvement = false;

        for (int i = 0; i < gains.size(); ++i) {
            cumulativeGain += gains[i];
            
            // Check balance constraint before applying the swap
            if (isBalancedAfterSwap(p, q) && cumulativeGain > maxGain) {
                maxGain = cumulativeGain;
                improvement = true;
            }
        }

        if (improvement) {
            applySwaps(p, q, maxGain);
        }

        return improvement;
    }

    void calculateGains(int p, int q, std::vector<int>& gains, std::unordered_set<int>& locked) {
        for (int v : partitions[p].vertices) {
            if (locked.find(v) == locked.end()) {
                int gain = calculateGain(v, q);
                gains.push_back(gain);
            }
        }
        for (int v : partitions[q].vertices) {
            if (locked.find(v) == locked.end()) {
                int gain = calculateGain(v, p);
                gains.push_back(gain);
            }
        }
        std::sort(gains.begin(), gains.end(), std::greater<int>());
    }

    int calculateGain(int vertex, int targetPartition) {
        int currentPartition = vertexToPartition[vertex];
        int gain = 0;
        
        for (int neighbor : vertices[vertex].neighbors) {
            if (vertexToPartition[neighbor] == currentPartition) {
                gain -= 1;  
            } else if (vertexToPartition[neighbor] == targetPartition) {
                gain += 1;  
            }
        }
        
        return gain;
    }

    void applySwaps(int p, int q, int maxGain) {
        int swapsApplied = 0;
        
        for (int vertex : partitions[p].vertices) {
            if (calculateGain(vertex, q) >= maxGain) {
                partitions[p].vertices.erase(vertex);
                partitions[q].vertices.insert(vertex);
                vertexToPartition[vertex] = q;
                swapsApplied++;
            }
        }

        for (int vertex : partitions[q].vertices) {
            if (calculateGain(vertex, p) >= maxGain) {
                partitions[q].vertices.erase(vertex);
                partitions[p].vertices.insert(vertex);
                vertexToPartition[vertex] = p;
                swapsApplied++;
            }
        }
    }

    bool isBalancedAfterSwap(int p, int q) {
        int sizeP = partitions[p].getSize();
        int sizeQ = partitions[q].getSize();
        return std::abs(sizeP - sizeQ) <= maxBalance;
    }
};

int main() {
    int numVertices = 10;
    int numPartitions = 3;
    int maxBalance = 2;

    Graph graph(numVertices, numPartitions, maxBalance);


    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 5);
    graph.addEdge(2, 6);
    graph.addEdge(3, 7);
    graph.addEdge(4, 8);
    graph.addEdge(5, 9);


    graph.assignToPartition(0, 0);
    graph.assignToPartition(1, 0);
    graph.assignToPartition(2, 1);
    graph.assignToPartition(3, 1);
    graph.assignToPartition(4, 2);
    graph.assignToPartition(5, 2);
    graph.assignToPartition(6, 0);
    graph.assignToPartition(7, 1);
    graph.assignToPartition(8, 2);
    graph.assignToPartition(9, 0);


    graph.runKLRefinement();


    graph.printPartitions();

    return 0;
}
