// g++ main.cpp -o main
// ./main

#include <iostream>
#include <string>
using namespace std;

struct listaAdyacencia { // lista de vecinos
    string nombreCiudad;
    listaAdyacencia* next; // se conectan en punteros entre si: lista enlazada
};

struct Ciudad { // ciudad: vertice del grafo
    string nombre;
    listaAdyacencia* conexiones; // cada nodo tiene su lista de vecinos
    Ciudad* next;
};

struct Grafo { // lista de ciudades
    Ciudad* head;
};

void crearGrafo(Grafo &g) { // formar grafo
    g.head = nullptr;
}

Ciudad* buscarCiudad(Grafo &g, const string &nombre) { //encontrar una ciudad
    Ciudad* temp = g.head;
    while (temp) {
        if (temp->nombre == nombre) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void agregarCiudad(Grafo &g, const string &nombre) { // agregar una ciudad
    Ciudad* newCiudad = new Ciudad{nombre, nullptr, g.head}; // se crea una nueva ciudad sin conexiones, que le siguen en el grafo las demas ciudades
    g.head = newCiudad; // se inserta al inicio del grafo
}

void agregarConexion(Ciudad* Ciudad, const string &dest) {
    listaAdyacencia* newNode = new listaAdyacencia{dest, Ciudad->conexiones}; // guardo al nuevo vecino al inicio de la lista
    Ciudad->conexiones = newNode; // reemplazo con la lista nueva
}

void agregarRuta(Grafo &g, const string &from, const string &to) {
    Ciudad* ciudadFrom = buscarCiudad(g, from);
    Ciudad* ciudadTo = buscarCiudad(g, to);
    
    if (!ciudadFrom || !ciudadTo) {
        cout << "Una de las ciudades no existe" << endl;
        return;
    }
    
    agregarConexion(ciudadFrom, to);
    agregarConexion(ciudadTo, from);
}

bool dfs(Ciudad* current, const string &target, Grafo &g, bool visited[]) { // depth first search
    if (current->nombre == target) return true;

    // marcar ciudad como visitada
    int index = 0;
    Ciudad* temp = g.head;
    while (temp) {
        if (temp->nombre == current->nombre) break;
        temp = temp->next;
        index++;
    }
    visited[index] = true; // guardamos la posicion de current en visited. se marca como visitado

    // recorrer conexiones
    listaAdyacencia* conex = current->conexiones;
    while (conex) {
        Ciudad* nextCiudad = buscarCiudad(g, conex->nombreCiudad);
        int nextIndex = 0;
        temp = g.head;
        while (temp) { // para encontrar la posicion de esta conexion nextCiudad en el grafo
            if (temp->nombre == nextCiudad->nombre) break;
            temp = temp->next;
            nextIndex++;
        }
        if (!visited[nextIndex] && dfs(nextCiudad, target, g, visited)) // si no se ha marcado esta ciudad dentro de visited, volvemos a hacer todo esto con esta conexion, y por tanto, con sus conexiones, intentando llegar al target
            return true; // si ambas arriba son true es porque se encontro la ciudad target
        conex = conex->next; // se intenta con todas las conexiones
    }
    return false; // no se ha encontrado el target = no hay conexion
}

bool isConnected(Grafo &g, const string &from, const string &to) {
    int CiudadCount = 0;
    Ciudad* temp = g.head;
    while (temp) { CiudadCount++; temp = temp->next; } // Contamos el numero de ciudades recorriendo el grafo

    bool* visited = new bool[CiudadCount]; // creamos el array bool que nos ayuda a marcar las visitadas
    for (int i = 0; i < CiudadCount; i++) visited[i] = false; // todas son false por default

    Ciudad* start = buscarCiudad(g, from);
    if (!start) return false;

    bool result = dfs(start, to, g, visited); // verificamos si estan conectadas o no mediante dfs
    delete[] visited;
    return result;
}

int main() {
    Grafo g;
    crearGrafo(g);

    // agregamos las ciudades
    agregarCiudad(g, "A");
    agregarCiudad(g, "B");
    agregarCiudad(g, "C");
    agregarCiudad(g, "D");

    // rutas que las conecten
    agregarRuta(g, "A", "B");
    agregarRuta(g, "B", "C");

    // verificamos conexiones
    cout << "A -> C: " << (isConnected(g, "A", "C") ? "Conectadas" : "No conectadas") << endl;
    cout << "A -> D: " << (isConnected(g, "A", "D") ? "Conectadas" : "No conectadas") << endl;

    return 0;
}
