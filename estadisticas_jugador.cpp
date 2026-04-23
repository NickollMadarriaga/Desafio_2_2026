#include "estadisticas_jugador.h"
#include <iostream>
using namespace std;

EstadisticasJugador::EstadisticasJugador()
    : partidos(0), goles(0), minutos(0), faltas(0), amarillas(0), rojas(0), asistencias(0) {}

EstadisticasJugador::EstadisticasJugador(int partidos, int goles, int minutos, int faltas,
                                         int amarillas, int rojas, int asistencias)
    : partidos(partidos), goles(goles), minutos(minutos), faltas(faltas),
    amarillas(amarillas), rojas(rojas), asistencias(asistencias) {}

EstadisticasJugador::EstadisticasJugador(const EstadisticasJugador& otro)
    : partidos(otro.partidos), goles(otro.goles), minutos(otro.minutos),
    faltas(otro.faltas), amarillas(otro.amarillas), rojas(otro.rojas),
    asistencias(otro.asistencias) {}

int EstadisticasJugador::getPartidos() const { return partidos; }
int EstadisticasJugador::getGoles() const { return goles; }
int EstadisticasJugador::getMinutos() const { return minutos; }
int EstadisticasJugador::getFaltas() const { return faltas; }
int EstadisticasJugador::getAmarillas() const { return amarillas; }
int EstadisticasJugador::getRojas() const { return rojas; }
int EstadisticasJugador::getAsistencias() const { return asistencias; }

void EstadisticasJugador::setPartidos(int p) { partidos = p; }
void EstadisticasJugador::setGoles(int g) { goles = g; }
void EstadisticasJugador::setMinutos(int m) { minutos = m; }
void EstadisticasJugador::setFaltas(int f) { faltas = f; }
void EstadisticasJugador::setAmarillas(int a) { amarillas = a; }
void EstadisticasJugador::setRojas(int r) { rojas = r; }
void EstadisticasJugador::setAsistencias(int a) { asistencias = a; }

void EstadisticasJugador::acumularPartido(int golesP, int minutosP, int faltasP,
                                          int amarillasP, int rojasP) {
    partidos++;
    goles    += golesP;
    minutos  += minutosP;
    faltas   += faltasP;
    amarillas += amarillasP;
    rojas    += rojasP;
}

EstadisticasJugador EstadisticasJugador::operator+(const EstadisticasJugador& otro) const {
    return EstadisticasJugador(
        partidos  + otro.partidos,
        goles     + otro.goles,
        minutos   + otro.minutos,
        faltas    + otro.faltas,
        amarillas + otro.amarillas,
        rojas     + otro.rojas,
        asistencias + otro.asistencias
        );
}

EstadisticasJugador& EstadisticasJugador::operator+=(const EstadisticasJugador& otro) {
    partidos   += otro.partidos;
    goles      += otro.goles;
    minutos    += otro.minutos;
    faltas     += otro.faltas;
    amarillas  += otro.amarillas;
    rojas      += otro.rojas;
    asistencias += otro.asistencias;
    return *this;
}

bool EstadisticasJugador::operator>(const EstadisticasJugador& otro) const {
    return goles > otro.goles;
}

void EstadisticasJugador::mostrar() const {
    cout << "  PJ:" << partidos << " Goles:" << goles
         << " Min:" << minutos << " Faltas:" << faltas
         << " Amarillas:" << amarillas << " Rojas:" << rojas
         << " Asistencias:" << asistencias << endl;
}
