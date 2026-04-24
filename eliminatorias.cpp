#include "eliminatorias.h"
#include <iostream>
#include "partido.h"
using namespace std;

Eliminatorias::Eliminatorias()
    : partidos(nullptr), numPartidos(0), clasificados(nullptr), numClasificados(0) {
    etapa[0] = '\0';
}

Eliminatorias::Eliminatorias(const char* eta, int numP)
    : numPartidos(numP), clasificados(nullptr), numClasificados(0) {
    int i = 0;
    while (i < 29 && eta[i] != '\0') {
        etapa[i] = eta[i];
        i++;
    }
    etapa[i] = '\0';

    partidos = new Partido[numP];

    clasificados = new Equipo*[numP];
    for (int j = 0; j < numP; j++) {
        clasificados[j] = nullptr;
    }
}


Eliminatorias::Eliminatorias(const Eliminatorias& otro)
    : numPartidos(otro.numPartidos), numClasificados(otro.numClasificados) {

    int i = 0;
    while (i < 29 && otro.etapa[i] != '\0') {
        etapa[i] = otro.etapa[i];
        i++;
    }
    etapa[i] = '\0';
    partidos = new Partido[numPartidos];
    for (int j = 0; j < numPartidos; j++) {
        partidos[j] = otro.partidos[j];
    }
    clasificados = new Equipo*[numPartidos];
    for (int k = 0; k < numPartidos; k++) {
        clasificados[k] = otro.clasificados[k];
    }
}


Eliminatorias::~Eliminatorias() {
    delete[] partidos;
    delete[] clasificados;
    partidos = nullptr;
    clasificados = nullptr;
}

const char* Eliminatorias::getEtapa() const { return etapa; }
Partido* Eliminatorias::getPartidos() const { return partidos; }
int Eliminatorias::getNumPartidos() const { return numPartidos; }
Equipo** Eliminatorias::getClasificados() const { return clasificados; }
int Eliminatorias::getNumClasificados() const { return numClasificados; }
void Eliminatorias::setEtapa(const char* e) {
    int i = 0;

    while (i < 29 && e[i] != '\0') {
        etapa[i] = e[i];
        i++;
    }

    etapa[i] = '\0';
}


void Eliminatorias::agregarPartido(Equipo* e1, Equipo* e2, const char* fecha) {
    // Encontrar primer slot libre
    for (int i = 0; i < numPartidos; i++) {
        if (partidos[i].getEquipo1() == nullptr) {
            char arb[MAX_ARBITROS][30] = {"codArbitro1", "codArbitro2", "codArbitro3"};
            partidos[i] = Partido(e1, e2, fecha, "00:00", "nombreSede",
                                  (const char(*)[30])arb);
            return;
        }
    }
}

void Eliminatorias::simularEtapa() {
    for (int i = 0; i < numPartidos; i++) {
        if (partidos[i].getEquipo1() == nullptr) continue;
        partidos[i].simularPartidoConProrroga();
        partidos[i].actualizarDatos();
    }
    obtenerClasificados();
}

void Eliminatorias::obtenerClasificados() {
    numClasificados = 0;
    for (int i = 0; i < numPartidos; i++) {
        if (!partidos[i].isSimulado()) continue;
        int g1 = partidos[i].getStatsEquipo1()->getGoles();
        int g2 = partidos[i].getStatsEquipo2()->getGoles();
        Equipo* ganador = (g1 >= g2) ? partidos[i].getEquipo1() : partidos[i].getEquipo2();
        clasificados[numClasificados++] = ganador;
    }
}

void Eliminatorias::mostrar() const {
    cout << " ETAPA: " << etapa << endl;
    for (int i = 0; i < numPartidos; i++) {
        if (partidos[i].getEquipo1() == nullptr) continue;
        partidos[i].mostrar();
        partidos[i].mostrarGoleadores();
    }
}

Eliminatorias& Eliminatorias::operator=(const Eliminatorias& otro) {
    if (this != &otro) {

        int i = 0;
        while (i < 29 && otro.etapa[i] != '\0') {
            etapa[i] = otro.etapa[i];
            i++;
        }
        etapa[i] = '\0';

        numPartidos = otro.numPartidos;
        numClasificados = otro.numClasificados;

        delete[] partidos;
        delete[] clasificados;

        partidos = new Partido[numPartidos];
        for (int j = 0; j < numPartidos; j++) {
            partidos[j] = otro.partidos[j];
        }

        clasificados = new Equipo*[numPartidos];
        for (int j = 0; j < numPartidos; j++) {
            clasificados[j] = otro.clasificados[j];
        }
    }
    return *this;
}
