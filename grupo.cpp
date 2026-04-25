#include "grupo.h"
#include <iostream>
using namespace std;

Grupo::Grupo() : letra('?'), partidos(nullptr) {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        equipos[i] = nullptr;
        puntajes[i] = 0;
        golesAFavor[i] = 0;
        golesEnContra[i] = 0;
    }
    partidos = new Partido[PARTIDOS_POR_GRUPO];
}

Grupo::Grupo(char let) : letra(let), partidos(nullptr) {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        equipos[i] = nullptr;
        puntajes[i] = 0;
        golesAFavor[i] = 0;
        golesEnContra[i] = 0;
    }
    partidos = new Partido[PARTIDOS_POR_GRUPO];
}

Grupo::Grupo(const Grupo& otro) : letra(otro.letra) {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        equipos[i] = otro.equipos[i];
        puntajes[i] = otro.puntajes[i];
        golesAFavor[i] = otro.golesAFavor[i];
        golesEnContra[i] = otro.golesEnContra[i];
    }
    partidos = new Partido[PARTIDOS_POR_GRUPO];
    for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) partidos[i] = otro.partidos[i];
}

Grupo::~Grupo() {
    delete[] partidos;
    partidos = nullptr;
}

char Grupo::getLetra() const { return letra; }
Equipo* Grupo::getEquipo(int i) const {
    if (i < 0 || i >= EQUIPOS_POR_GRUPO) return nullptr;
    return equipos[i];
}
Partido* Grupo::getPartidos() const { return partidos; }
int Grupo::getPuntaje(int i) const { return puntajes[i]; }
int Grupo::getDiferenciaGoles(int i) const { return golesAFavor[i] - golesEnContra[i]; }

bool Grupo::agregarEquipo(Equipo* e) {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        if (equipos[i] == nullptr) {
            equipos[i] = e;
            return true;
        }
    }
    return false; // Grupo lleno
}

int Grupo::getCantidadEquipos() const {
    int cnt = 0;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) if (equipos[i] != nullptr) cnt++;
    return cnt;
}

void Grupo::configurarPartidos(const char* fechaInicio) {
    // tdos contra todos
    int pares[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};

    // Asignamos fechas simples: dias 1,4,7,10,13,16 (un partido por par, cada equipo descansa 3 dias)
    int diasOffset[6] = {0, 3, 6, 1, 4, 7};

    int dd, mm, aaaa;
    int i = 0;
    dd = 0;

    while (fechaInicio[i] != '/') {
        dd = dd * 10 + (fechaInicio[i] - '0');
        i++;
    }

    char arb[MAX_ARBITROS][30] = {"codArbitro1", "codArbitro2", "codArbitro3"};

    for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) {
        int dia = dd + diasOffset[i];
        int mes = mm, anio = aaaa;
        while (dia > 30) { dia -= 30; mes++; }

        char fechaBuf[11];
        int pos = 0;

        if (dia < 10) {
            fechaBuf[pos++] = '0';
        }
        fechaBuf[pos++] = (dia / 10) + '0';
        fechaBuf[pos++] = (dia % 10) + '0';

        fechaBuf[pos++] = '/';

        if (mes < 10) {
            fechaBuf[pos++] = '0';
        }
        fechaBuf[pos++] = (mes / 10) + '0';
        fechaBuf[pos++] = (mes % 10) + '0';

        fechaBuf[pos++] = '/';

        int temp = anio;
        int dig[4];

        for (int i = 3; i >= 0; i--) {
            dig[i] = temp % 10;
            temp /= 10;
        }

        for (int i = 0; i < 4; i++) {
            fechaBuf[pos++] = dig[i] + '0';
        }

        fechaBuf[pos] = '\0';


        Equipo* e1 = equipos[pares[i][0]];
        Equipo* e2 = equipos[pares[i][1]];
        partidos[i] = Partido(e1, e2, fechaBuf, "00:00", "nombreSede",
                              (const char(*)[30])arb);
    }
}

void Grupo::simularPartidos() {
    for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) {
        partidos[i].simularPartido();
        partidos[i].actualizarDatos();
    }
    actualizarTabla();
}

void Grupo::actualizarTabla() {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        puntajes[i] = 0;
        golesAFavor[i] = 0;
        golesEnContra[i] = 0;
    }

    for (int p = 0; p < PARTIDOS_POR_GRUPO; p++) {
        Partido& partido = partidos[p];
        if (!partido.isSimulado()) continue;

        int g1 = partido.getStatsEquipo1()->getGoles();
        int g2 = partido.getStatsEquipo2()->getGoles();

        // Encontrar indices de los equipos
        int idx1 = -1, idx2 = -1;
        for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
            if (equipos[i] == partido.getEquipo1()) idx1 = i;
            if (equipos[i] == partido.getEquipo2()) idx2 = i;
        }
        if (idx1 < 0 || idx2 < 0) continue;

        golesAFavor[idx1]  += g1; golesEnContra[idx1] += g2;
        golesAFavor[idx2]  += g2; golesEnContra[idx2] += g1;

        if (g1 > g2) { puntajes[idx1] += 3; }
        else if (g1 == g2) { puntajes[idx1]++; puntajes[idx2]++; }
        else { puntajes[idx2] += 3; }
    }
}

int* Grupo::obtenerClasificacion() const {
    int* orden = new int[EQUIPOS_POR_GRUPO];
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) orden[i] = i;

    // Ordenamiento burbuja
    for (int i = 0; i < EQUIPOS_POR_GRUPO - 1; i++) {
        for (int j = 0; j < EQUIPOS_POR_GRUPO - 1 - i; j++) {
            int a = orden[j], b = orden[j + 1];
            bool intercambiar = false;
            if (puntajes[a] < puntajes[b]) {
                intercambiar = true;
            } else if (puntajes[a] == puntajes[b]) {
                int dga = golesAFavor[a] - golesEnContra[a];
                int dgb = golesAFavor[b] - golesEnContra[b];
                if (dga < dgb) intercambiar = true;
                else if (dga == dgb && golesAFavor[a] < golesAFavor[b]) intercambiar = true;
            }
            if (intercambiar) { int tmp = orden[j]; orden[j] = orden[j+1]; orden[j+1] = tmp; }
        }
    }
    return orden;
}

void Grupo::mostrarGrupo() const {
    cout << " GRUPO " << letra << endl;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        if (equipos[i]) {
            cout << "  " << equipos[i]->getNombre()
            << " (" << equipos[i]->getConfederacion() << ")"
            << " Ranking:" << equipos[i]->getRanking() << endl;
        }
    }
}

void Grupo::mostrarTablaClasificacion() const {
    int* orden = obtenerClasificacion();
    cout << "--- Tabla Grupo " << letra << " ---" << endl;
    cout << "Equipo                 PJ PG PE PP GF GC DG PTS" << endl;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        int idx = orden[i];
        if (!equipos[idx]) continue;
        int pj = 3;
        int dg = golesAFavor[idx] - golesEnContra[idx];
        cout.width(22); cout << left << equipos[idx]->getNombre();
        cout << " " << pj
             << " " << golesAFavor[idx]  // PG aproximado
             << " " << golesAFavor[idx]
             << " " << golesEnContra[idx]
             << " " << dg
             << " " << puntajes[idx] << endl;
    }
    delete[] orden;
}

Grupo& Grupo::operator=(const Grupo& otro) {
    if (this != &otro) {
        letra = otro.letra;
        for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
            equipos[i]     = otro.equipos[i];
            puntajes[i]    = otro.puntajes[i];
            golesAFavor[i] = otro.golesAFavor[i];
            golesEnContra[i] = otro.golesEnContra[i];
        }
        delete[] partidos;
        partidos = new Partido[PARTIDOS_POR_GRUPO];
        for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) partidos[i] = otro.partidos[i];
    }
    return *this;
}