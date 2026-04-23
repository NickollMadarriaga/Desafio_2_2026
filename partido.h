#ifndef PARTIDO_H
#define PARTIDO_H

#include "Equipo.h"
#include "estadisticas_partido.h"

#define MAX_ARBITROS 3
#define JUGADORES_POR_PARTIDO 11

// Estructura para registrar las estadisticas de un jugador en un partido especifico
struct EstadisticasJugadorPartido {
    int numeroCamiseta;
    int goles;
    int amarillas;
    int rojas;
    int faltas;
    int minutos;
};

class Partido {
private:
    Equipo* equipo1;
    Equipo* equipo2;
    char fecha[11];   // formato DD/MM/AAAA
    char hora[6];     // formato HH:MM
    char sede[100];
    char arbitros[MAX_ARBITROS][30];

    EstadisticasPartido* statsEquipo1;
    EstadisticasPartido* statsEquipo2;

    int* convocados1;
    int* convocados2;

    EstadisticasJugadorPartido* statsJugadores1;
    EstadisticasJugadorPartido* statsJugadores2;

    bool simulado;
    bool fueProrroga;

public:
    Partido();
    Partido(Equipo* e1, Equipo* e2, const char* fecha, const char* hora,
            const char* sede, const char arbitros[][30]);
    Partido(const Partido& otro);
    ~Partido();

    Equipo* getEquipo1() const;
    Equipo* getEquipo2() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getSede() const;
    EstadisticasPartido* getStatsEquipo1() const;
    EstadisticasPartido* getStatsEquipo2() const;
    bool getFueProrroga() const;
    bool isSimulado() const;

    void setFecha(const char* f);
    void setHora(const char* h);
    void setSede(const char* s);
    void setEquipo1(Equipo* e);
    void setEquipo2(Equipo* e);

    // Simular partido usando la formula de lambda del enunciado
    // Retorna: 1 si gana equipo1, 2 si gana equipo2, 0 si empate (solo fase grupos)
    int simularPartido();

    // Simular con prorroga (para eliminatorias)
    int simularPartidoConProrroga();

    void actualizarDatos();
    void mostrar() const;
    void mostrarGoleadores() const;

    Partido& operator=(const Partido& otro);
};

#endif // PARTIDO_H
