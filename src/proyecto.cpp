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


// ==========================================
// 4. LÓGICA DE SUCESIÓN 
// ==========================================

// Muestra la línea de sucesión (solo para los vivos)
void showSuccessionLine(Node* root) {
    if (!root) return;
    if (!root->is_dead) {
        cout << "- " << root->name << " " << root->last_name 
             << " (ID: " << root->id << ")" << (root->is_boss ? " [JEFE ACTUAL]" : "")
             << (root->in_jail ? " [EN PRISION]" : "") << "\n";
    }
    showSuccessionLine(root->left);
    showSuccessionLine(root->right);
}

// Encuentra el próximo jefe válido usando las complejas reglas que se traducen a Pre-Orden
Node* findNextBoss(Node* root, bool allow_jail) {
    if (!root) return nullptr;
    
    // Si está vivo, y no es el jefe actual (a menos que estemos buscando reemplazo general) y cumple la condición de libertad.
    if (!root->is_dead && !root->is_boss) {
        if (allow_jail || !root->in_jail) {
            return root;
        }
    }

    Node* leftSearch = findNextBoss(root->left, allow_jail);
    if (leftSearch) return leftSearch;

    return findNextBoss(root->right, allow_jail);
}

void triggerSuccession(Node* root, Node*& current_boss) {
    if (!current_boss) return;

    cout << "\n[!] ALERTA: El jefe actual (" << current_boss->name << ") ha muerto, envejecido o ido a prision.\n";
    current_boss->is_boss = false;
    current_boss->was_boss = true;

    // Buscar primer sucesor vivo y libre
    Node* new_boss = findNextBoss(root, false);
    
    // Si no hay nadie libre, buscar a los que están en la cárcel
    if (!new_boss) {
        cout << "[!] No hay sucesores libres. Buscando en prisiones...\n";
        new_boss = findNextBoss(root, true);
    }

    if (new_boss) {
        new_boss->is_boss = true;
        current_boss = new_boss;
        cout << "[+] NUEVO JEFE ASIGNADO: " << current_boss->name << " " << current_boss->last_name << "\n";
    } else {
        cout << "[-] LA FAMILIA HA CAIDO. No hay sucesores vivos.\n";
        current_boss = nullptr;
    }
}


// ==========================================
// 5. MANEJO DE DATOS
// ==========================================

void readCSV(string filename, Node*& root, Node*& current_boss) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se encontro el archivo " << filename << ". Verifica la carpeta bin.\n";
        return;
    }

    string line;
    getline(file, line); 

    while (getline(file, line)) {
        if(line.empty()) continue;

        string fields[10];
        int idx = 0;
        string current = "";
        
        // Parseo manual 
        for (char c : line) {
            if (c == ',' && idx < 10) {
                fields[idx++] = current;
                current = "";
            } else {
                current += c;
            }
        }
        if (idx < 10) fields[idx] = current;

        Node* n = new Node{
            fields[0], fields[1], fields[2], parseGender(fields[3]), stoi(fields[4]),
            fields[5], parseBool(fields[6]), parseBool(fields[7]), parseBool(fields[8]), parseBool(fields[9]),
            nullptr, nullptr
        };

        if (n->is_boss && !current_boss) {
            current_boss = n;
        }

        insertNode(root, n);
    }
    file.close();
    cout << "Datos cargados correctamente.\n";
}

void modifyNodeData(Node* root, Node*& current_boss) {
    string search_id;
    cout << "Ingresa el ID del nodo a modificar: ";
    cin >> search_id;

    Node* target = findNode(root, search_id);
    if (!target) {
        cout << "Nodo no encontrado.\n";
        return;
    }

    cout << "\nEditando a " << target->name << " " << target->last_name << "\n";
    cout << "Nuevo Nombre: "; cin >> target->name;
    cout << "Nuevo Apellido: "; cin >> target->last_name;
    cout << "Nuevo Genero (H/M): "; cin >> target->gender;
    cout << "Nueva Edad: "; cin >> target->age;
    cout << "Esta muerto? (1=Si, 0=No): "; cin >> target->is_dead;
    cout << "Esta en prision? (1=Si, 0=No): "; cin >> target->in_jail;

  // Si el editado es el jefe, verificar las reglas 3 y 4 de destitución automática
    if (target == current_boss) {
       
        if (target->is_dead || target->in_jail || target->age > 70) {
            triggerSuccession(root, current_boss);
        }
    }
}

// ==========================================
// 6. MAIN
// ==========================================

int main() {
    Node* root = nullptr;
    Node* current_boss = nullptr;

    readCSV("BOSS.csv", root, current_boss);

    int option = 0;
    while (option != 4) {
        cout << "\n=== PANEL DE LA FAMILIA ===\n";
        if (current_boss) cout << "Jefe Actual: " << current_boss->name << " " << current_boss->last_name << "\n";
        else cout << "Jefe Actual: NINGUNO\n";
        
        cout << "1. Mostrar linea de sucesion (Solo vivos)\n";
        cout << "2. Cambiar datos de un miembro\n";
        cout << "3. Forzar verificacion de sucesion (Por si hay errores)\n";
        cout << "4. Salir\n";
        cout << "Selecciona: ";
        cin >> option;