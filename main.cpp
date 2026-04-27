#include <iostream>
#include <cstdlib>
#include <ctime>
#include "udeaworldcup.h"
using namespace std;

void imprimirMenu() {
    cout << "UdeAWorldCup - FIFA 2026" << endl;
    cout << " 1. Cargar datos" << endl;
    cout << " 2. Generar grupos (sorteo)" << endl;
    cout << " 3. Simular fase de grupos " << endl;
    cout << "4. Simular eliminatorias" << endl;
    cout << "5. Mostrar estadisticas finales" << endl;
    cout << "6. Medir eficiencia" << endl;
    cout << "0. Salir" << endl;
    cout << "Opcion: ";
}

int main() {
    srand((unsigned int)time(nullptr));

    UdeAWorldCup torneo;
    int opcion = -1;

    while (opcion != 0) {
        imprimirMenu();
        cin >> opcion;

        switch (opcion) {
        case 1:
            torneo.cargarDatos();
            break;
        case 2:
            torneo.generarGrupos();
            break;
        case 3:
            torneo.simularFaseGrupos();
            break;
        case 4:
            torneo.simularEliminatorias();
            break;
        case 5:
            torneo.mostrarEstadisticas();
            break;
        case 6:
            torneo.medirEficiencia();
            break;
        case 0:
            cout << "Hasta luego!" << endl;
            break;
        default:
            cout << "Opcion invalida." << endl;
        }
    }
    return 0;
}