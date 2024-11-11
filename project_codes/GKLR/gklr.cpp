#define GKLR
#ifdef GKLR
#include <bits/stdc++.h>
using namespace std;
#include "gain_table.hpp"

vector<Node *> nodes;
int partition_count;
// the gain table
GainTable *gainTable;

void initialize()
{
    // open a file and read the nodes and vertices
    // file is in the format
    // vertex_count edge_count
    // edges with pairs of vertices per line
    // nodes are initialized with gain 0

    ifstream file("input.txt");

    int vertex_count, edge_count;
    file >> partition_count;
    file >> vertex_count >> edge_count;

    for (int i = 0; i < vertex_count; i++)
    {
        Node *node = new Node(i + 1, 0);
        nodes.push_back(node);
    }
    for (int i = 0; i < edge_count; i++)
    {
        int u, v;
        file >> u >> v;
        nodes[u - 1]->neighbors.push_back(nodes[v - 1]);
        nodes[v - 1]->neighbors.push_back(nodes[u - 1]);
    }
    file.close();

    // print the nodes with their neighbors

    cout << "========================================Nodes and Neighbors==========================================" << endl;

    for (int i = 0; i < vertex_count; i++)
    {
        cout << "Node " << nodes[i]->vertex << " neighbors: ";
        for (int j = 0; j < nodes[i]->neighbors.size(); j++)
        {
            cout << nodes[i]->neighbors[j]->vertex << " ";
        }
        cout << endl;
    }

    // initialize the gain table with -vertex_count to vertex_count
    gainTable = new GainTable(vertex_count, -vertex_count);
}


void print_partitions()
{
    // print the nodes with their partitions
    cout << "========================================current Partitions==========================================" << endl;

    for (int i = 0; i < nodes.size(); i++)
    {
        cout << "Node " << nodes[i]->vertex << " partition: " << nodes[i]->partition << endl;
    }
}

void random_partition()
{
    // randomly assign the nodes to partitions
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->partition = rand() % partition_count;
    }
    print_partitions();

    
}

void calculate_gains()
{
    // calculate the gain of each node
    for (int i = 0; i < nodes.size(); i++)
    {
        int external = 0;
        int internal = 0;
        for (int j = 0; j < nodes[i]->neighbors.size(); j++)
        {
            if (nodes[i]->neighbors[j]->partition != nodes[i]->partition)
            {
                external++;
            }
            else
            {
                internal++;
            }
        }
        nodes[i]->gain = external - internal;
    }

    // assign the nodes to the gain table

    

    for (int i = 0; i < nodes.size(); i++)
    {
        gainTable->insert(nodes[i]);
    }
    gainTable->print();
}

void swap_and_lock()
{
    cout << "========================================Swapping and Locking==========================================" << endl;
    
    // the node with the highest gain value=========================================
    Node *node = gainTable->getFirst();
    if (node == nullptr)
    {
        return;
    }
    node->is_locked = true;
    gainTable->remove(node);
    cout << "Swapping node " << node->vertex << " with gain " << node->gain << endl;



    // choose the partition with the highest number of neighbors============================================

    int partitions[partition_count] = {0};
    for (int i = 0; i < node->neighbors.size(); i++)
    {
        partitions[node->neighbors[i]->partition]++;
    }
    int partition = 0; // the new partition for node
    int max = partitions[0];
    for (int i = 1; i < partition_count; i++)
    {
        if (partitions[i] > max)
        {
            max = partitions[i];
            partition = i;
        }
    }
    int old_partition = node->partition;
    
    // update the gain of the neighbors=========================================
    for (int i = 0; i < node->neighbors.size(); i++)
    {
        //neighbour with old partition will have gain +2
        if (node->neighbors[i]->partition == old_partition)
        {
            gainTable->shiftUp(node->neighbors[i]);
        }
        else if (node->neighbors[i]->partition == partition)
        {
            gainTable->shiftDown(node->neighbors[i]);
        }
    }


    // update the partition and recalculate gain of the node=========================================
    node->partition = partition;

    int external = 0;
    int internal = 0;
    for (int j = 0; j < node->neighbors.size(); j++)
    {
        if (node->neighbors[j]->partition != node->partition)
        {
            external++;
        }
        else
        {
            internal++;
        }
    }
    node->gain = external - internal;

    // insert the node back to the gain table=========================================
    gainTable->insert(node);
    gainTable->print();
    print_partitions();
    
}

int main()
{
    initialize();
    random_partition();
    calculate_gains();
    swap_and_lock();
    swap_and_lock();

    return 0;
}

#endif