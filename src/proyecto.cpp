#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ==========================================
// 1. ESTRUCTURAS DE DATOS
// ==========================================

struct Node {
    string id;
    string name;
    string last_name;
    char gender;
    int age;
    string id_boss;
    bool is_dead;
    bool in_jail;
    bool was_boss;
    bool is_boss;

    Node* left;
    Node* right;
};

// Cola personalizada para no usar std::vector ni std::queue (Regla del proyecto)
struct QueueNode {
    Node* treeNode;
    QueueNode* next;
};

struct CustomQueue {
    QueueNode* front;
    QueueNode* rear;
    CustomQueue() : front(nullptr), rear(nullptr) {}

    void push(Node* node) {
        QueueNode* qNode = new QueueNode{node, nullptr};
        if (!rear) {
            front = rear = qNode;
            return;
        }
        rear->next = qNode;
        rear = qNode;
    }

    Node* pop() {
        if (!front) return nullptr;
        QueueNode* temp = front;
        Node* res = temp->treeNode;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return res;
    }

    bool isEmpty() { return front == nullptr; }
};
