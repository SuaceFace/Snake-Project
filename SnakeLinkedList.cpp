#include <string>
#include "SnakeLinkedList.h"
#include <iostream>
#include <sstream>

using namespace std;

using namespace std;
string SnakeListNode::toString() const {
    stringstream ss;
    ss << "(row = " << _row << ", col = " << _col << ')';

    return ss.str();
}

SnakeLinkedList::~SnakeLinkedList() {
    SnakeListNode *curNode = _head;
    while (curNode) {
        SnakeListNode *nextNode = curNode->nextNode();
        delete curNode;
        curNode = nextNode;
    }
}

int SnakeLinkedList::length() const {
    int result = 0;
    SnakeListNode *curNode = _head;
    while (curNode ) {
        ++result;
        curNode = curNode->nextNode();
    }

    return result;
}

SnakeListNode *SnakeLinkedList::tail() const {
    SnakeListNode *curNode = _head;
    while (curNode && curNode->nextNode()) {
        curNode = curNode->nextNode();
    }
    return curNode;
}

void SnakeLinkedList::pushFront(int row, int col) {
    _head = new SnakeListNode(row, col, _head);
}

void SnakeLinkedList::pushBack(int row, int col) {
    SnakeListNode *tailNode = tail();
    SnakeListNode *newTailNode = new SnakeListNode(row, col, nullptr);
    if (tailNode == nullptr) {
        _head = newTailNode;
    } else {
        tailNode->setNextNode(newTailNode);
    }
}

void SnakeLinkedList::popBack() {
    /*The popBack method removes the last SnakeListNode from the
     SnakeLinkedList.

     Two special cases to consider are:
     - The SnakeLinkedList is empty. In this case popBack should have no effect.
     - The SnakeLinkedList has only one SnakeListNode. In this case popBack will
     make it empty. When removing a SnakeListNode be sure to free up its memory
     using the delete operator.*/

    if (_head == nullptr) {
        return;
    } else {
        if(_head->nextNode() == nullptr) {
            delete _head;
            _head = nullptr;
        }
        else{
			SnakeListNode *tailNode = head(); //last node
            SnakeListNode *curNode = nullptr; //second to last node
			while (tailNode->nextNode() != nullptr) {
				curNode = tailNode;
				tailNode = tailNode->nextNode();
			}
            delete tailNode; //remove last node
            curNode->setNextNode(nullptr); //second to last now has a nullptr
        }
    }
}

void SnakeLinkedList::printSnake() const {
    SnakeListNode *curNode = _head;
    int len = length();
    cout << "length = " << len;
    if (len > 1) {
        cout << ", head = " << head()->toString();
        cout << ", tail = " << tail()->toString();
    }
    if (len == 1) {
        cout << ", head = tail = " << head()->toString();
    }
    cout << endl;

    while (curNode) {
        cout << " " << curNode->toString() << endl;
        curNode = curNode->nextNode();
    }

    cout << endl;
}