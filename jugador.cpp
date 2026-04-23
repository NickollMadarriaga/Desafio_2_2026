#include "jugador.h"
#include <iostream>
using namespace std;

Jugador::Jugador() : numeroCamisa(0), estadisticas(nullptr) {
    nombre[0] = '\0';
    apellido[0] = '\0';
    estadisticas = new EstadisticasJugador();
}

Jugador::Jugador(const char* nom, const char* ape, int num, const EstadisticasJugador& stats)
    : numeroCamisa(num) {
    int i = 0;
    for (; i < 49 && nom[i] != '\0'; i++) {
        nombre[i] = nom[i];
    }
    nombre[i] = '\0';
    i = 0;
    for (; i < 49 && ape[i] != '\0'; i++) {
        apellido[i] = ape[i];
    }
    apellido[i] = '\0';

    estadisticas = new EstadisticasJugador(stats);
}

Jugador::Jugador(const Jugador& otro) : numeroCamisa(otro.numeroCamisa) {

    int i = 0;
    for (; i < 49 && otro.nombre[i] != '\0'; i++) {
        nombre[i] = otro.nombre[i];
    }
    nombre[i] = '\0';
    i = 0;
    for (; i < 49 && otro.apellido[i] != '\0'; i++) {
        apellido[i] = otro.apellido[i];
    }
    apellido[i] = '\0';

    estadisticas = new EstadisticasJugador(*otro.estadisticas);
}

Jugador::~Jugador() {
    delete estadisticas;
    estadisticas = nullptr;
}

const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getApellido() const { return apellido; }
int Jugador::getnumeroCamisa() const { return numeroCamisa; }
EstadisticasJugador* Jugador::getEstadisticas() const { return estadisticas; }

void Jugador::setNombre(const char* n) {
    int i = 0;
    for (; i < 49 && n[i] != '\0'; i++) {
        nombre[i] = n[i];
    }
    nombre[i] = '\0';
}

void Jugador::setApellido(const char* a) {
    int i = 0;
    for (; i < 49 && a[i] != '\0'; i++) {
        apellido[i] = a[i];
    }
    apellido[i] = '\0';
}

void Jugador::setnumeroCamisa(int n) { numeroCamisa = n; }

void Jugador::actualizarEstadisticas(int golesP, int minutosP, int faltasP,
                                     int amarillasP, int rojasP) {
    estadisticas->acumularPartido(golesP, minutosP, faltasP, amarillasP, rojasP);
}

bool sonIguales(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return a[i] == b[i];
}

bool Jugador::operator==(const Jugador& otro) const {
    return numeroCamisa == otro.numeroCamisa && sonIguales(nombre, otro.nombre);
}

Jugador& Jugador::operator=(const Jugador& otro) {
    if (this != &otro) {

        int i = 0;
        for (; i < 49 && otro.nombre[i] != '\0'; i++) {
            nombre[i] = otro.nombre[i];
        }
        nombre[i] = '\0';

        i = 0;
        for (; i < 49 && otro.apellido[i] != '\0'; i++) {
            apellido[i] = otro.apellido[i];
        }
        apellido[i] = '\0';

        numeroCamisa = otro.numeroCamisa;

        delete estadisticas;
        estadisticas = new EstadisticasJugador(*otro.estadisticas);
    }
    return *this;
}

void Jugador::mostrar() const {
    cout << "  #" << numeroCamisa << " " << nombre << " " << apellido;
    estadisticas->mostrar();
}
