#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unordered_set>

using namespace std;

struct Partition
{
    vector<int> assignment;
    double fitness;
};


double calculateFitness(const vector<vector<int>> &graph, const Partition &partition, int numPartitions)
{
    int cutEdges = 0;
    vector<int> partitionSizes(numPartitions, 0);
    for (int u = 0; u < graph.size(); ++u)
    {
        for (int v : graph[u])
        {
            if (partition.assignment[u] != partition.assignment[v])
            {
                cutEdges++;
            }
        }
        partitionSizes[partition.assignment[u]]++;
    }

   
    double balancePenalty = 0;
    int avgSize = graph.size() / numPartitions;
    for (int size : partitionSizes)
    {
        balancePenalty += abs(size - avgSize);
    }

    return cutEdges + balancePenalty;
}


Partition randomPartition(int numNodes, int numPartitions)
{
    Partition partition;
    partition.assignment.resize(numNodes);
    for (int i = 0; i < numNodes; ++i)
    {
        partition.assignment[i] = rand() % numPartitions;
    }
    return partition;
}


Partition crossover(const Partition &parent1, const Partition &parent2)
{
    Partition child;
    int numNodes = parent1.assignment.size();
    child.assignment.resize(numNodes);

    for (int i = 0; i < numNodes; ++i)
    {
        child.assignment[i] = (rand() % 2 == 0) ? parent1.assignment[i] : parent2.assignment[i];
    }

    return child;
}


void mutate(Partition &partition, int numPartitions)
{
    int node = rand() % partition.assignment.size();
    partition.assignment[node] = rand() % numPartitions;
}

Partition select(const vector<Partition> &population)
{
    int idx1 = rand() % population.size();
    int idx2 = rand() % population.size();
    return (population[idx1].fitness < population[idx2].fitness) ? population[idx1] : population[idx2];
}


vector<vector<int>> loadGraph(const string& filename) {
    ifstream file(filename);
    int numNodes, numEdges;
    if (!(file >> numNodes >> numEdges)) {
        cerr << "Error reading the file!" << endl;
        exit(1);
    }
    
    vector<vector<int>> graph(numNodes);  // Ensure correct size initialization
    
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        if (!(file >> u >> v)) {
            cerr << "Error reading edge!" << endl;
            exit(1);
        }
        graph[u].push_back(v);  // Ensure u, v are within bounds
        graph[v].push_back(u);
    }

    return graph;
}


Partition geneticAlgorithm(const vector<vector<int>> &graph, int numPartitions, int populationSize, int generations)
{
    vector<Partition> population;

  
    for (int i = 0; i < populationSize; ++i)
    {
        Partition p = randomPartition(graph.size(), numPartitions);
        p.fitness = calculateFitness(graph, p, numPartitions);
        population.push_back(p);
    }

    for (int gen = 0; gen < generations; ++gen)
    {
        vector<Partition> newPopulation;

        for (int i = 0; i < populationSize; ++i)
        {
       
            Partition parent1 = select(population);
            Partition parent2 = select(population);

            Partition child = crossover(parent1, parent2);

            if (rand() % 100 < 5)
            {
                mutate(child, numPartitions);
            }

            child.fitness = calculateFitness(graph, child, numPartitions);

            newPopulation.push_back(child);
        }

        population = newPopulation;
    }

    return *min_element(population.begin(), population.end(), [](const Partition &a, const Partition &b)
                        { return a.fitness < b.fitness; });
}

int main()
{
    srand(time(0));

    string filename = "graph.txt";
    int numPartitions = 3; 
    int populationSize = 100;
    int generations = 1000;

    vector<vector<int>> graph = loadGraph(filename);

    Partition best = geneticAlgorithm(graph, numPartitions, populationSize, generations);

    cout << "Best partition fitness: " << best.fitness << endl;
    for (int i = 0; i < best.assignment.size(); ++i)
    {
        cout << "Node " << i << " -> Partition " << best.assignment[i] << endl;
    }

    return 0;
}
