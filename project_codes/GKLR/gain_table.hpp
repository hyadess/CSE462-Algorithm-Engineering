// an array of doubly linked lists. each index in the array represents a gain value. the list at that index contains all the nodes with that gain value.
#define GAIN_TABLE_H
#ifdef GAIN_TABLE_H


#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int vertex;
    int gain;
    Node* next;
    Node* prev;
    int partition;
    vector<Node*> neighbors;
    bool is_locked;

    Node(int vertex, int gain)
    {
        this->vertex = vertex;
        this->next = nullptr;
        this->prev = nullptr;
        this->gain = gain;
        this->partition = -1;
        this->is_locked = false;

    }

};

class GainTable {

private:
    int maxGain;
    int minGain;
    // create a data structure that stores and sorts the nodes by its gain value on run. more like a priority queue
    vector<Node*> gainTable;


public:

    GainTable(int maxGain,int minGain) {
        this->maxGain = maxGain;
        this->minGain = minGain;
        // create dummy nodes for each gain value and push it to the gainTable
        // highest to lowest gain value
        for ( int i =maxGain; i>=minGain; i--) {
            Node* dummy = new Node(-1, i);
            gainTable.push_back(dummy);
        }
    }
    
    int searchIndex(int gain) {
        //maxgain is at index 0
        return maxGain - gain;
    }
    
    void insert(Node* node) {
        // O(1) operation
        int index = searchIndex(node->gain);
        Node* dummy = gainTable[index];
        // assume node is the new node to be inserted
        // insert node between dummy and dummy->next
        node->next = dummy->next;
        node->prev = dummy;
        dummy->next = node;
        if (node->next != nullptr) {
            node->next->prev = node;
        }
    }
    
    void remove(Node* node) {
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
    }
    // the optimal node to be removed is the first node in the list with the highest gain value
    
    Node* getFirst() {
        // first node in the list with the highest gain value and is not locked
        for (int i = 0; i < gainTable.size(); i++) {
            // if the gain value is negative for i, then return nullptr

            if(gainTable[i]->gain<0){
                return nullptr;
            }
            Node* dummy = gainTable[i];
            Node* current = dummy->next;
            while (current != nullptr) {
                //make sure the gain value is positive
                if (!current->is_locked ) {
                    return current;
                }
                current = current->next;
            }
        }
        return nullptr;
    }

    // shift a node to 2 gain value lower
    void shiftDown(Node* node) {
        // O(1) operation
        remove(node);
        node->gain -= 2;
        insert(node);
    }

    // shift a node to 2 gain value higher
    void shiftUp(Node* node) {
        // O(1) operation
        remove(node);
        node->gain += 2;
        insert(node);
    }

    void print() {
        cout << "========================================Gain Table==========================================" << endl;
        for (int i = 0; i < gainTable.size(); i++) {
            Node* dummy = gainTable[i];
            cout << "Gain " << dummy->gain << ": ";
            Node* current = dummy->next;
            while (current != nullptr) {
                cout << current->vertex << " ";
                current = current->next;
            }
            cout << endl;
        }
    }

    void clear() {
        for (int i = 0; i < gainTable.size(); i++) {
            Node* dummy = gainTable[i];
           //unlink don't delete
            dummy->next = nullptr;
        }
    }


};



#endif