#include "udeaworldcup.h"
#include <iostream>
using namespace std;

UdeAWorldCup::UdeAWorldCup()
    : equipos(nullptr), grupos(nullptr), partidos(nullptr), eliminatorias(nullptr),
    totalEquipos(0), totalPartidos(0),
    contadorIteraciones(0), memoriaConsumida(0) {
    equipos      = new Equipo[MAX_EQUIPOS];
    grupos       = new Grupo[NUM_GRUPOS];
    partidos     = new Partido[72]; // 12 grupos x 6 partidos
    eliminatorias = new Eliminatorias[6];
}

UdeAWorldCup::~UdeAWorldCup() {
    delete[] equipos;
    delete[] grupos;
    delete[] partidos;
    delete[] eliminatorias;
}