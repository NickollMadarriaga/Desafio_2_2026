#include "estadisticas_partido.h"
#include <iostream>
using namespace std;

EstadisticasPartido::EstadisticasPartido()
    : goles(0), amarillas(0), rojas(0), faltas(0), posesion(50.0f) {}

EstadisticasPartido::EstadisticasPartido(int goles, int amarillas, int rojas, int faltas, float posesion)
    : goles(goles), amarillas(amarillas), rojas(rojas), faltas(faltas), posesion(posesion) {}

EstadisticasPartido::EstadisticasPartido(const EstadisticasPartido& otro)
    : goles(otro.goles), amarillas(otro.amarillas), rojas(otro.rojas),
    faltas(otro.faltas), posesion(otro.posesion) {}

int EstadisticasPartido::getGoles() const { return goles; }
int EstadisticasPartido::getAmarillas() const { return amarillas; }
int EstadisticasPartido::getRojas() const { return rojas; }
int EstadisticasPartido::getFaltas() const { return faltas; }
float EstadisticasPartido::getPosesion() const { return posesion; }

void EstadisticasPartido::setGoles(int g) { goles = g; }
void EstadisticasPartido::setAmarillas(int a) { amarillas = a; }
void EstadisticasPartido::setRojas(int r) { rojas = r; }
void EstadisticasPartido::setFaltas(int f) { faltas = f; }
void EstadisticasPartido::setPosesion(float p) { posesion = p; }

bool EstadisticasPartido::operator>(const EstadisticasPartido& otro) const {
    return goles > otro.goles;
}

void EstadisticasPartido::mostrar() const {
    cout << "  Goles:" << goles << " Amarillas:" << amarillas
         << " Rojas:" << rojas << " Faltas:" << faltas
         << " Posesion:" << posesion << "%" << endl;
}