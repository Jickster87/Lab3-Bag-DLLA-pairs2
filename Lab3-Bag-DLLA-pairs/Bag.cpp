#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
using namespace std;


Bag::Bag() {
    sizeBag = 0;
    head = -1;
    maxCap = MAX_CAPACITY;
    firstEmpty = 0;

    nodes = new dllaNode[maxCap]; //create new array

    //init array
    for (int i = 0; i < maxCap; i++) {
        nodes[i].elem = NULL_TPAIR;
        if (i < maxCap-1)
        {
            nodes[i].next = i + 1;
        }
        else {
            nodes[i].next = -1; //last .next in array
        }
        if (i > 1)
        {
            nodes[i].prev = i - 1;
        }
        else {
            nodes[i].prev = -1; //first .prev in array [0]
        }
    }

}

void Bag::resize() {
    int oldCap = maxCap; //save state
    maxCap *= 2; //double capacity
    
    dllaNode * newNodes = new dllaNode[maxCap]; //allocate new array space

    for (int i = 0; i < oldCap; i++)
    {
        newNodes[i] = nodes[i];
    }
    //init the rest of the new array
    for (int i = oldCap; i < maxCap; i++) {
        newNodes[i].elem = NULL_TPAIR;
        if (i < maxCap - 1)
        {
            newNodes[i].next = i + 1;
        }
        else {
            newNodes[i].next = -1; //last .next in array
        }
        if (i > oldCap) {
            newNodes[i].prev = i - 1;  // Link to the previous node
        }
    }

    firstEmpty = oldCap;
    delete[] nodes;
    nodes = newNodes;

}

void Bag::add(TElem elem) {
    // Check if we need to resize
    if (firstEmpty == -1) {
        resize();
    }

    // Case 1: Element exists, increment frequency
    int current = head;
    while (current != -1) {
        if (nodes[current].elem.first == elem) {
            nodes[current].elem.second++; // Increment frequency
            sizeBag++;
            return;
        }
        current = nodes[current].next;
    }

    // Case 2: Element does not exist, add new node
    int newPos = firstEmpty;                 // Take the first empty spot
    firstEmpty = nodes[firstEmpty].next;     // Update the free list to point to the next free node

    nodes[newPos].elem = std::make_pair(elem, 1); // Set the element and frequency
    nodes[newPos].next = head;               // New node points to the current head
    nodes[newPos].prev = -1;                 // No previous for the new head

    if (head != -1) {
        nodes[head].prev = newPos;           // Update the previous of the old head
    }
    head = newPos;                           // Update head to the new node

    sizeBag++;                               // Increase the bag size
}

bool Bag::remove(TElem elem) {
    // Case 1: Empty bag
    if (head == -1) {
        return false;
    }

    // Traverse to find the element
    int current = head;
    while (current != -1 && nodes[current].elem.first != elem) {
        current = nodes[current].next;
    }

    // Case 2: Element not found
    if (current == -1) {
        return false;
    }

    // Case 3: Element found
    if (nodes[current].elem.second > 1) {
        // Decrease frequency if more than 1 occurrence
        nodes[current].elem.second--;
        sizeBag--;
        return true;
    }

    // Remove the node entirely if frequency is 1
    sizeBag--;

    if (current == head) {
        // Removing the head node
        head = nodes[current].next; // Update head to the next node
        if (head != -1) {
            nodes[head].prev = -1;  // Update the new head's prev
        }
    }
    else {
        // Removing a node in the middle or end
        int prev = nodes[current].prev;
        int next = nodes[current].next;

        if (prev != -1) {
            nodes[prev].next = next;
        }
        if (next != -1) {
            nodes[next].prev = prev;
        }
    }

    // Return the node to the free list
    nodes[current].elem = NULL_TPAIR;         // Reset the element
    nodes[current].next = firstEmpty;         // Link it to the current free list
    nodes[current].prev = -1;                 // No previous in free list
    firstEmpty = current;                     // Update firstEmpty to this node

    return true;
}



bool Bag::search(TElem elem) const {
    int nomad = head;
    while (nomad != -1 && nodes[nomad].elem.first != elem) {
        nomad = nodes[nomad].next;
    }
    if (nomad != -1) {
        return true;
    }
    else {
        return false;
    }
}

int Bag::nrOccurrences(TElem elem) const {
    int current = head;
    while (current != -1) {
        if (nodes[current].elem.first == elem) {
            return nodes[current].elem.second; // directly return frequency
        }
        current = nodes[current].next;
    }
    return 0; // element not found
}


int Bag::size() const {
    return sizeBag;
}


bool Bag::isEmpty() const {
    return sizeBag == 0;
}

BagIterator Bag::iterator() const {
    return BagIterator(*this);
}


Bag::~Bag() {
    delete[] nodes;
}
