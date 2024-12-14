#include <exception>
#include "BagIterator.h"
#include "Bag.h"


using namespace std;

BagIterator::BagIterator(const Bag& c): bag(c) {
    currentIndex = bag.head;
    currentFreq = 1;
}

void BagIterator::first() {
    currentIndex = bag.head;
    currentFreq = 1;
}

void BagIterator::next() {
    if (!valid()) {
        throw exception();
    }

    if (currentFreq < bag.nodes[currentIndex].elem.second) {
    
        currentFreq++;
    }
    else {
        // We have returned all occurrences of this element
        currentIndex = bag.nodes[currentIndex].next;
        currentFreq = 1;

    }
}

bool BagIterator::valid() const {
    return currentIndex != -1;
}

TElem BagIterator::getCurrent() const {
    if (!valid()) {
        throw exception();
    }
    return bag.nodes[currentIndex].elem.first;
}
//
//BagIterator Bag::iterator() const {
//    return BagIterator(*this);
//}
