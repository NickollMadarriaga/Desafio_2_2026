#include "estadisticas_equipo.h"
#include <iostream>
using namespace std;

EstadisticasEquipo::EstadisticasEquipo()
    : golesFavor(0), golesContra(0), ganados(0), empatados(0),
    perdidos(0), amarillas(0), rojas(0), faltas(0) {}

EstadisticasEquipo::EstadisticasEquipo(int gf, int gc, int gan, int emp, int per,
                                       int am, int rj, int fal)
    : golesFavor(gf), golesContra(gc), ganados(gan), empatados(emp),
    perdidos(per), amarillas(am), rojas(rj), faltas(fal) {}

EstadisticasEquipo::EstadisticasEquipo(const EstadisticasEquipo& otro)
    : golesFavor(otro.golesFavor), golesContra(otro.golesContra),
    ganados(otro.ganados), empatados(otro.empatados), perdidos(otro.perdidos),
    amarillas(otro.amarillas), rojas(otro.rojas), faltas(otro.faltas) {}


int EstadisticasEquipo::getGolesFavor() const { return golesFavor; }
int EstadisticasEquipo::getGolesContra() const { return golesContra; }
int EstadisticasEquipo::getGanados() const { return ganados; }
int EstadisticasEquipo::getEmpatados() const { return empatados; }
int EstadisticasEquipo::getPerdidos() const { return perdidos; }
int EstadisticasEquipo::getAmarillas() const { return amarillas; }
int EstadisticasEquipo::getRojas() const { return rojas; }
int EstadisticasEquipo::getFaltas() const { return faltas; }

int EstadisticasEquipo::getDiferenciaGoles() const {
    return golesFavor - golesContra;
}

int EstadisticasEquipo::getPartidosJugados() const {
    return ganados + empatados + perdidos;
}

float EstadisticasEquipo::getPromedioGolesFavor() const {
    int pj = getPartidosJugados();
    if (pj == 0) return 1.0f; // Evitar division por cero, valor neutro
    return (float)golesFavor / pj;
}

float EstadisticasEquipo::getPromedioGolesContra() const {
    int pj = getPartidosJugados();
    if (pj == 0) return 1.0f;
    return (float)golesContra / pj;
}

void EstadisticasEquipo::setGolesFavor(int gf) { golesFavor = gf; }
void EstadisticasEquipo::setGolesContra(int gc) { golesContra = gc; }
void EstadisticasEquipo::setGanados(int g) { ganados = g; }
void EstadisticasEquipo::setEmpatados(int e) { empatados = e; }
void EstadisticasEquipo::setPerdidos(int p) { perdidos = p; }
void EstadisticasEquipo::setAmarillas(int a) { amarillas = a; }
void EstadisticasEquipo::setRojas(int r) { rojas = r; }
void EstadisticasEquipo::setFaltas(int f) { faltas = f; }

void EstadisticasEquipo::actualizarConPartido(int gf, int gc, int am, int rj, int fal) {
    golesFavor  += gf;
    golesContra += gc;
    amarillas   += am;
    rojas       += rj;
    faltas      += fal;
    if (gf > gc) ganados++;
    else if (gf == gc) empatados++;
    else perdidos++;
}

EstadisticasEquipo& EstadisticasEquipo::operator+=(const EstadisticasEquipo& otro) {
    golesFavor  += otro.golesFavor;
    golesContra += otro.golesContra;
    ganados     += otro.ganados;
    empatados   += otro.empatados;
    perdidos    += otro.perdidos;
    amarillas   += otro.amarillas;
    rojas       += otro.rojas;
    faltas      += otro.faltas;
    return *this;
}

bool EstadisticasEquipo::operator>(const EstadisticasEquipo& otro) const {
    return golesFavor > otro.golesFavor;
}

void EstadisticasEquipo::mostrar() const {
    cout << "  PJ:" << getPartidosJugados()
    << " PG:" << ganados << " PE:" << empatados << " PP:" << perdidos
    << " GF:" << golesFavor << " GC:" << golesContra
    << " DG:" << getDiferenciaGoles()
    << " Am:" << amarillas << " Rj:" << rojas << endl;
}