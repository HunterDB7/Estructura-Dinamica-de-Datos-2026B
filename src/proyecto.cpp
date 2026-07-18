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

// ==========================================
// 2. FUNCIONES DE UTILIDAD
// ==========================================

bool parseBool(string s) {
    return (s == "true" || s == "1");
}

char parseGender(string s) {
    if (s == "Female") return 'M'; // Mujer
    return 'H'; // Hombre por defecto para Male u otros
}
// ==========================================
// 3. OPERACIONES DEL ÁRBOL
// ==========================================

// Busca un nodo por su ID
Node* findNode(Node* root, string target_id) {
    if (!root) return nullptr;
    if (root->id == target_id) return root;
    Node* leftSearch = findNode(root->left, target_id);
    if (leftSearch) return leftSearch;
    return findNode(root->right, target_id);
}

// Inserción en el árbol: Busca al jefe, si no lo encuentra o ya tiene 2 hijos, inserta por nivel.
void insertNode(Node*& root, Node* newNode) {
    if (!root) {
        root = newNode;
        return;
    }

    Node* boss = findNode(root, newNode->id_boss);
    if (boss) {
        if (!boss->left) {
            boss->left = newNode;
            return;
        } else if (!boss->right) {
            boss->right = newNode;
            return;
        }
    }
    
    CustomQueue q;
    q.push(root);
    while (!q.isEmpty()) {
        Node* curr = q.pop();
        if (!curr->left) {
            curr->left = newNode;
            break;
        } else {
            q.push(curr->left);
        }
        if (!curr->right) {
            curr->right = newNode;
            break;
        } else {
            q.push(curr->right);
        }
    }
}