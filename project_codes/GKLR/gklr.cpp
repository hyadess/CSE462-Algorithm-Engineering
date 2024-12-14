#define GKLR
#ifdef GKLR
#include <bits/stdc++.h>
using namespace std;
#include "gain_table.hpp"
#define maxn 100005

vector<Node *> nodes;

// given inputs
int partition_count;
int balance_dif;
string input_file;

// variables to keep track of the partitions

int partition_size[maxn];
// the gain table
GainTable *gainTable;

int calculate_cut_size()
{
    int cut_size = 0;
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes[i]->neighbors.size(); j++)
        {
            if (nodes[i]->partition != nodes[i]->neighbors[j]->partition)
            {
                cut_size++;
            }
        }
    }
    return cut_size / 2;
}


void unlock_nodes()
{
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->is_locked = false;
    }
}


void initialize()
{
    // open a file and read the nodes and vertices
    // file is in the format
    // vertex_count edge_count
    // edges with pairs of vertices per line
    // nodes are initialized with gain 0

    ifstream file("small_dense/"+input_file);

    int vertex_count, edge_count;
    file >> vertex_count >> edge_count;

    for (int i = 0; i < vertex_count; i++)
    {
        Node *node = new Node(i, 0);
        nodes.push_back(node);
    }
    for (int i = 0; i < edge_count; i++)
    {
        int u, v;
        file >> u >> v;
        nodes[u]->neighbors.push_back(nodes[v]);
        nodes[v]->neighbors.push_back(nodes[u]);
    }
    file.close();

    // print the nodes with their neighbors

    // cout << "========================================Nodes and Neighbors==========================================" << endl;

    // for (int i = 0; i < vertex_count; i++)
    // {
    //     cout << "Node " << nodes[i]->vertex << " neighbors: ";
    //     for (int j = 0; j < nodes[i]->neighbors.size(); j++)
    //     {
    //         cout << nodes[i]->neighbors[j]->vertex << " ";
    //     }
    //     cout << endl;
    // }

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

void init_partition()
{
    //  assign the nodes to partitions
    int cur = 0;
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->partition = cur;
        partition_size[cur]++;
        cur = (cur + 1) % partition_count;
    }

    // find out cur max and min size of the partitions

    // print_partitions();
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
    // gainTable->print();
}

int shift_and_lock()
{
    // cout << "========================================Swapping and Locking==========================================" << endl;

    // the node with the highest gain value=========================================
    Node *node = gainTable->getFirst();
    if (node == nullptr)
    {
        return -1;
    }
    node->is_locked = true;
    gainTable->remove(node);
    // cout << "Swapping node " << node->vertex << " with gain " << node->gain << endl;

    // choose the partition with the highest number of neighbors============================================

    int partitions[partition_count] = {0};
    int partition = 0; // the new partition for node
    int old_partition = node->partition;
    int max = -1;

    for (int i = 0; i < node->neighbors.size(); i++)
    {
        partitions[node->neighbors[i]->partition]++;
    }

    for (int i = 0; i < node->neighbors.size(); i++)
    {
        if (partitions[node->neighbors[i]->partition] > max &&
            node->neighbors[i]->partition != old_partition &&
            abs((partition_size[node->neighbors[i]->partition] + 1) - (partition_size[old_partition] - 1)) <= balance_dif)
        {
            max = partitions[node->neighbors[i]->partition];
            partition = node->neighbors[i]->partition;
        }
    }
    if (max == -1)
    {
        gainTable->insert(node);
        return -1;
    }


    // update the partition size=========================================
    partition_size[old_partition]--;
    partition_size[partition]++;

    // update the gain of the neighbors=========================================
    for (int i = 0; i < node->neighbors.size(); i++)
    {
        // neighbour with old partition will have gain +2
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
    // gainTable->print();
    // print_partitions();

    return 1; // success
}

bool isCSVEmpty(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return false; // or handle the error as needed
    }

    // Check if the file is empty
    if (file.peek() == std::ifstream::traits_type::eof()) {
        return true; // File is empty
    }

    return false; // File is not empty
}


int main(int argc, char *argv[])
{
    partition_count = 2;
    balance_dif = 3;
    if (argc > 1)
    {
        partition_count = atoi(argv[1]);
        balance_dif = atoi(argv[2]);
        input_file = argv[3];
    }

    initialize();
    // cout << "========================================Initialization==========================================" << endl;
    init_partition();

    int iteration = 10000;
    calculate_gains();
    while (iteration--)
    {
        //cout<<"Iteration "<<100-iteration<<endl;
        // clear the gain table
        int cont = 0;
        while (1)
        {

            if (shift_and_lock() == -1)
            {
                break;
            }
            cont++;
            if (cont > 100000)
            {
                break;
            }
        }
        unlock_nodes();
    }

    // store the result in a file, make sure to append the result

    // write in results/input_file.txt file

    // ofstream file("results/small_dense" + input_file, ios::app);
    // // the cut size
    // file << "For partition count " << partition_count << " ,input file " << input_file << " and balance difference " << balance_dif << " the cut size is " << calculate_cut_size() << endl;

    // file.close();

    //also, write in a csv file
    // if input file is input.txt change it to input.csv


    ofstream csv_file("results/small_dense/" + input_file.substr(0, input_file.size() - 4) + ".csv", ios::app);

    // if the csv file size is 0, add the column names
    if (isCSVEmpty("results/small_dense/" + input_file.substr(0, input_file.size() - 4) + ".csv"))
    {
        csv_file << "partition_count,balance_difference,cut_size" << endl;
    }



    // add columns partition count, balance difference, cut size
    csv_file << partition_count << "," << balance_dif << "," << calculate_cut_size() << endl;





    return 0;
}

#endif