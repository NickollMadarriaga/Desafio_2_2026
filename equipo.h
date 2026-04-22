#ifndef EQUIPO_H
#define EQUIPO_H

#include "jugador.h"
#include "estadisticas_equipo.h"

#define MAX_JUGADORES 26

class Equipo {
private:
    char nombre[100];
    char confederacion[50];
    int ranking;
    Jugador* jugadores;
    EstadisticasEquipo* estadisticas;

public:
    Equipo();
    Equipo(const char* nombre, const char* confederacion, int ranking,
           const EstadisticasEquipo& stats);
    Equipo(const Equipo& otro);
    ~Equipo();


    const char* getNombre() const;
    const char* getConfederacion() const;
    int getRanking() const;
    Jugador* getJugadores() const;
    EstadisticasEquipo* getEstadisticas() const;
    Jugador* getJugador(int indice) const;


    void setNombre(const char* n);
    void setConfederacion(const char* c);
    void setRanking(int r);

    void inicializarJugadores();

    int* seleccionarOnce() const;

    EstadisticasEquipo* getEstadisticasEquipo() const;

    void actualizarEstadisticas(int gf, int gc, int amarillas, int rojas, int faltas, bool gano, bool empato);

    void guardarJugadoresEnArchivo() const;
    void cargarJugadoresDesdeArchivo();

    bool operator<(const Equipo& otro) const; // Comparar por ranking
    bool operator==(const Equipo& otro) const;
    Equipo& operator=(const Equipo& otro);

    void mostrar() const;
    void mostrarConJugadores() const;
};

#endif //EQUIPO_H
