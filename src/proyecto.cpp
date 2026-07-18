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