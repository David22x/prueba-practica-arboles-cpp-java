/*
 * ============================================================
 *  Universidad Técnica de Ambato
 *  Estructura de Datos - Árboles Binarios de Búsqueda
 *  Sistema de Gestión de Estudiantes
 *  Autor: [Tu nombre]
 *  Lenguaje: C++
 * ============================================================
 */

#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
#include <limits>
using namespace std;

// ============================================================
//  ESTRUCTURA: Estudiante
//  Contiene los datos de cada estudiante en el árbol
// ============================================================
struct Estudiante
{
    string cedula; // Clave de búsqueda (BST)
    string apellidos;
    string nombres;
    float notaFinal;
    string carrera;
    int nivel;
};

// ============================================================
//  ESTRUCTURA: Nodo del árbol binario
// ============================================================
struct Nodo
{
    Estudiante datos;
    Nodo *izq; // subárbol izquierdo (cédula menor)
    Nodo *der; // subárbol derecho  (cédula mayor)

    // Constructor
    Nodo(const Estudiante &e) : datos(e), izq(nullptr), der(nullptr) {}
};

// ============================================================
//  CLASE: ArbolBST
//  Árbol Binario de Búsqueda ordenado por cédula
// ============================================================
class ArbolBST
{
private:
    Nodo *raiz;

    // ── Auxiliares recursivos ──────────────────────────────

    /* Insertar un nodo en el subárbol con raíz p */
    Nodo *insertar(Nodo *p, const Estudiante &e)
    {
        if (p == nullptr)
            return new Nodo(e);

        if (e.cedula < p->datos.cedula)
            p->izq = insertar(p->izq, e);
        else if (e.cedula > p->datos.cedula)
            p->der = insertar(p->der, e);
        else
            cout << "   Ya existe un estudiante con cédula " << e.cedula << "\n";

        return p;
    }

    /* Buscar nodo con cédula dada; devuelve puntero o nullptr */
    Nodo *buscar(Nodo *p, const string &cedula) const
    {
        if (p == nullptr || p->datos.cedula == cedula)
            return p;
        if (cedula < p->datos.cedula)
            return buscar(p->izq, cedula);
        return buscar(p->der, cedula);
    }

    /* Encontrar el nodo con la cédula mínima del subárbol */
    Nodo *minimo(Nodo *p) const
    {
        while (p->izq != nullptr)
            p = p->izq;
        return p;
    }

    /* Eliminar nodo con cédula dada y devolver nueva raíz del subárbol */
    Nodo *eliminar(Nodo *p, const string &cedula, bool &eliminado)
    {
        if (p == nullptr)
        {
            eliminado = false;
            return nullptr;
        }

        if (cedula < p->datos.cedula)
        {
            p->izq = eliminar(p->izq, cedula, eliminado);
        }
        else if (cedula > p->datos.cedula)
        {
            p->der = eliminar(p->der, cedula, eliminado);
        }
        else
        {
            eliminado = true;
            // Caso 1: hoja
            if (p->izq == nullptr && p->der == nullptr)
            {
                delete p;
                return nullptr;
            }
            // Caso 2: un hijo
            if (p->izq == nullptr)
            {
                Nodo *tmp = p->der;
                delete p;
                return tmp;
            }
            if (p->der == nullptr)
            {
                Nodo *tmp = p->izq;
                delete p;
                return tmp;
            }
            // Caso 3: dos hijos → sucesor inorden
            Nodo *suc = minimo(p->der);
            p->datos = suc->datos;
            bool dummy = true;
            p->der = eliminar(p->der, suc->datos.cedula, dummy);
        }
        return p;
    }

    /* Recorrido Inorden (izq → raíz → der) — ordena por cédula */
    void inorden(Nodo *p) const
    {
        if (p == nullptr)
            return;
        inorden(p->izq);
        imprimirEstudiante(p->datos);
        inorden(p->der);
    }

    /* Recorrido Preorden (raíz → izq → der) */
    void preorden(Nodo *p) const
    {
        if (p == nullptr)
            return;
        imprimirEstudiante(p->datos);
        preorden(p->izq);
        preorden(p->der);
    }

    /* Recorrido Postorden (izq → der → raíz) */
    void postorden(Nodo *p) const
    {
        if (p == nullptr)
            return;
        postorden(p->izq);
        postorden(p->der);
        imprimirEstudiante(p->datos);
    }

    /* Contar nodos recursivamente */
    int contarNodos(Nodo *p) const
    {
        if (p == nullptr)
            return 0;
        return 1 + contarNodos(p->izq) + contarNodos(p->der);
    }

    /* Calcular altura recursivamente */
    int altura(Nodo *p) const
    {
        if (p == nullptr)
            return 0;
        int hi = altura(p->izq);
        int hd = altura(p->der);
        return 1 + max(hi, hd);
    }

    /* Buscar nodo con mayor nota */
    Nodo *mayorNota(Nodo *p, Nodo *mejor) const
    {
        if (p == nullptr)
            return mejor;
        if (mejor == nullptr || p->datos.notaFinal > mejor->datos.notaFinal)
            mejor = p;
        mejor = mayorNota(p->izq, mejor);
        mejor = mayorNota(p->der, mejor);
        return mejor;
    }

    /* Buscar nodo con menor nota */
    Nodo *menorNota(Nodo *p, Nodo *peor) const
    {
        if (p == nullptr)
            return peor;
        if (peor == nullptr || p->datos.notaFinal < peor->datos.notaFinal)
            peor = p;
        peor = menorNota(p->izq, peor);
        peor = menorNota(p->der, peor);
        return peor;
    }

    /* Mostrar aprobados (nota >= 7.0) en inorden */
    void aprobados(Nodo *p) const
    {
        if (p == nullptr)
            return;
        aprobados(p->izq);
        if (p->datos.notaFinal >= 7.0f)
            imprimirEstudiante(p->datos);
        aprobados(p->der);
    }

    /* Mostrar reprobados (nota < 7.0) en inorden */
    void reprobados(Nodo *p) const
    {
        if (p == nullptr)
            return;
        reprobados(p->izq);
        if (p->datos.notaFinal < 7.0f)
            imprimirEstudiante(p->datos);
        reprobados(p->der);
    }

    /* Liberar toda la memoria del árbol */
    void destruir(Nodo *p)
    {
        if (p == nullptr)
            return;
        destruir(p->izq);
        destruir(p->der);
        delete p;
    }

    // ── Utilidad de impresión ─────────────────────────────
    void imprimirEstudiante(const Estudiante &e) const
    {
        cout << "  |-----------------------------------------\n";
        cout << "  | Cédula   : " << e.cedula << "\n";
        cout << "  | Apellidos: " << e.apellidos << "\n";
        cout << "  | Nombres  : " << e.nombres << "\n";
        cout << "  | Nota     : " << fixed << setprecision(2) << e.notaFinal
             << "  " << (e.notaFinal >= 7.0f ? "APROBADO" : "REPROBADO") << "\n";
        cout << "  | Carrera  : " << e.carrera << "\n";
        cout << "  | Nivel    : " << e.nivel << "\n";
        cout << "  |-----------------------------------------\n";
    }

public:
    // Constructor / Destructor
    ArbolBST() : raiz(nullptr) {}
    ~ArbolBST() { destruir(raiz); }

    // ── Interfaz pública ──────────────────────────────────

    /* 1. Insertar estudiante */
    void insertarEstudiante(const Estudiante &e)
    {
        raiz = insertar(raiz, e);
        cout << "  Estudiante " << e.cedula << " insertado correctamente.\n";
    }

    /* 2. Buscar estudiante por cédula */
    void buscarEstudiante(const string &cedula) const
    {
        Nodo *res = buscar(raiz, cedula);
        if (res)
            imprimirEstudiante(res->datos);
        else
            cout << "  Estudiante con cédula " << cedula << " no encontrado.\n";
    }

    /* 3. Eliminar estudiante */
    void eliminarEstudiante(const string &cedula)
    {
        bool ok = true;
        raiz = eliminar(raiz, cedula, ok);
        if (ok)
            cout << "  Estudiante " << cedula << " eliminado.\n";
        else
            cout << "  No existe estudiante con cédula " << cedula << ".\n";
    }

    /* 4. Recorrido Inorden */
    void recorridoInorden() const
    {
        cout << "\n  === RECORRIDO INORDEN (ascendente por cédula) ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío.\n";
            return;
        }
        inorden(raiz);
    }

    /* 5. Recorrido Preorden */
    void recorridoPreorden() const
    {
        cout << "\n  === RECORRIDO PREORDEN ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío\n";
            return;
        }
        preorden(raiz);
    }

    /* 6. Recorrido Postorden */
    void recorridoPostorden() const
    {
        cout << "\n === RECORRIDO POSTORDEN ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío.\n";
            return;
        }
        postorden(raiz);
    }

    /* 7. Recorrido por niveles BFS */
    void recorridoPorNiveles() const
    {
        cout << "\n === RECORRIDO BFS (por niveles) ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío\n";
            return;
        }

        queue<Nodo *> cola;
        cola.push(raiz);
        int nivel = 1;

        while (!cola.empty())
        {
            int tam = (int)cola.size();
            cout << "\n  ── Nivel " << nivel++ << " ──\n";
            for (int i = 0; i < tam; i++)
            {
                Nodo *actual = cola.front();
                cola.pop();
                imprimirEstudiante(actual->datos);
                if (actual->izq)
                    cola.push(actual->izq);
                if (actual->der)
                    cola.push(actual->der);
            }
        }
    }

    /* 8. Contar nodos */
    void contarNodos() const
    {
        cout << "  Total de estudiantes: " << contarNodos(raiz) << "\n";
    }

    /* 9. Calcular altura */
    void calcularAltura() const
    {
        cout << "  Altura del árbol: " << altura(raiz) << "\n";
    }

    /* 10. Estudiante con mayor nota */
    void buscarNotaMayor() const
    {
        Nodo *res = mayorNota(raiz, nullptr);
        if (res)
        {
            cout << "\n  === ESTUDIANTE CON MAYOR NOTA ===\n";
            imprimirEstudiante(res->datos);
        }
        else
        {
            cout << "  Árbol vacío\n";
        }
    }

    /* 11. Estudiante con menor nota */
    void buscarNotaMenor() const
    {
        Nodo *res = menorNota(raiz, nullptr);
        if (res)
        {
            cout << "\n  === ESTUDIANTE CON MENOR NOTA ===\n";
            imprimirEstudiante(res->datos);
        }
        else
        {
            cout << "  Árbol vacío\n";
        }
    }

    /* 12. Mostrar aprobados */
    void mostrarAprobados() const
    {
        cout << "\n  === ESTUDIANTES APROBADOS (nota ≥ 7.0) ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío\n";
            return;
        }
        aprobados(raiz);
    }

    /* 13. Mostrar reprobados */
    void mostrarReprobados() const
    {
        cout << "\n  === ESTUDIANTES REPROBADOS (nota < 7.0) ===\n";
        if (raiz == nullptr)
        {
            cout << "  Árbol vacío\n";
            return;
        }
        reprobados(raiz);
    }
};

// ============================================================
//  FUNCIONES DE APOYO: Lectura validada de datos
// ============================================================

/* Limpiar buffer de entrada */
void limpiarBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/* Leer cadena no vacía */
string leerCadena(const string &etiqueta)
{
    string val;
    do
    {
        cout << "  " << etiqueta << ": ";
        getline(cin, val);
        if (val.empty())
            cout << "  Campo requerido. Intente de nuevo.\n";
    } while (val.empty());
    return val;
}

/* Leer nota en rango [0, 10] */
float leerNota()
{
    float nota;
    while (true)
    {
        cout << "  Nota final (0 - 10): ";
        if (cin >> nota && nota >= 0.0f && nota <= 10.0f)
        {
            limpiarBuffer();
            return nota;
        }
        cout << "  Nota inválida. Ingrese un valor entre 0 y 10.\n";
        cin.clear();
        limpiarBuffer();
    }
}

/* Leer nivel (entero positivo) */
int leerNivel()
{
    int nivel;
    while (true)
    {
        cout << "  Nivel (1 - 10): ";
        if (cin >> nivel && nivel >= 1 && nivel <= 10)
        {
            limpiarBuffer();
            return nivel;
        }
        cout << "  Nivel inválido.\n";
        cin.clear();
        limpiarBuffer();
    }
}

/* Construir un Estudiante a partir de la entrada del usuario */
Estudiante leerEstudiante()
{
    Estudiante e;
    e.cedula = leerCadena("Cédula");
    e.apellidos = leerCadena("Apellidos");
    e.nombres = leerCadena("Nombres");
    e.notaFinal = leerNota();
    e.carrera = leerCadena("Carrera");
    e.nivel = leerNivel();
    return e;
}

// ============================================================
//  MENÚ PRINCIPAL
// ============================================================
void mostrarMenu()
{
    cout << "\n";
    cout << "  |----------------------------------------------|\n";
    cout << "  |   SISTEMA DE GESTION ESTUDIANTIL  -  UTA     |\n";
    cout << "  |         Árbol Binario de Búsqueda            |\n";
    cout << "  |----------------------------------------------|\n";
    cout << "  |  1.  Insertar estudiante                     |\n";
    cout << "  |  2.  Buscar estudiante por cédula            |\n";
    cout << "  |  3.  Eliminar estudiante                     |\n";
    cout << "  |  4.  Recorrido Inorden                       |\n";
    cout << "  |  5.  Recorrido Preorden                      |\n";
    cout << "  |  6.  Recorrido Postorden                     |\n";
    cout << "  |  7.  Recorrido por niveles (BFS)             |\n";
    cout << "  |  8.  Contar estudiantes                      |\n";
    cout << "  |  9.  Calcular altura del árbol               |\n";
    cout << "  | 10.  Estudiante con mayor nota               |\n";
    cout << "  | 11.  Estudiante con menor nota               |\n";
    cout << "  | 12.  Mostrar aprobados                       |\n";
    cout << "  | 13.  Mostrar reprobados                      |\n";
    cout << "  | 14.  Salir                                   |\n";
    cout << "  |----------------------------------------------|\n";
    cout << "  Opción: ";
}

// ============================================================
//  FUNCIÓN PRINCIPAL
// ============================================================
int main()
{
    ArbolBST arbol;
    int opcion;

    // Datos de prueba para demostración rápida
    arbol.insertarEstudiante({"1804001001", "Pérez López", "Ana María", 8.5f, "Sistemas", 3});
    arbol.insertarEstudiante({"1804002002", "García Torres", "Luis Alberto", 5.0f, "Industrial", 2});
    arbol.insertarEstudiante({"1804003003", "Molina Ruiz", "Karla Sofía", 9.2f, "Civil", 5});
    arbol.insertarEstudiante({"1804004004", "Vásquez Mora", "Diego Andrés", 6.8f, "Sistemas", 1});
    arbol.insertarEstudiante({"1804005005", "Herrera Lema", "Valeria Noel", 3.5f, "Electrónica", 4});

    cout << "\n  Datos de prueba cargados. Bienvenido al sistema.\n";

    do
    {
        mostrarMenu();

        // Leer opción con validación
        while (!(cin >> opcion) || opcion < 1 || opcion > 14)
        {
            cout << "  Opción inválida. Ingrese un número del 1 al 14: ";
            cin.clear();
            limpiarBuffer();
        }
        limpiarBuffer();
        cout << "\n";

        switch (opcion)
        {
        case 1:
        {
            cout << "  ── Insertar estudiante ──\n";
            Estudiante e = leerEstudiante();
            arbol.insertarEstudiante(e);
            break;
        }
        case 2:
        {
            cout << "  ── Buscar estudiante ──\n";
            string ced = leerCadena("Cédula");
            arbol.buscarEstudiante(ced);
            break;
        }
        case 3:
        {
            cout << "  ── Eliminar estudiante ──\n";
            string ced = leerCadena("Cédula");
            arbol.eliminarEstudiante(ced);
            break;
        }
        case 4:
            arbol.recorridoInorden();
            break;
        case 5:
            arbol.recorridoPreorden();
            break;
        case 6:
            arbol.recorridoPostorden();
            break;
        case 7:
            arbol.recorridoPorNiveles();
            break;
        case 8:
            arbol.contarNodos();
            break;
        case 9:
            arbol.calcularAltura();
            break;
        case 10:
            arbol.buscarNotaMayor();
            break;
        case 11:
            arbol.buscarNotaMenor();
            break;
        case 12:
            arbol.mostrarAprobados();
            break;
        case 13:
            arbol.mostrarReprobados();
            break;
        case 14:
            cout << "  Hasta pronto. Saliendo del sistema...\n\n";
            break;
        }

    } while (opcion != 14);

    return 0;
}