#ifndef GRUPO_H
#define GRUPO_H

#include "Equipo.h"
#include "Partido.h"

#define EQUIPOS_POR_GRUPO 4
#define PARTIDOS_POR_GRUPO 6

class Grupo {
private:
    char letra;
    Equipo* equipos[EQUIPOS_POR_GRUPO];
    Partido* partidos;                   // 6 partidos por grupo
    int puntajes[EQUIPOS_POR_GRUPO];
    int golesAFavor[EQUIPOS_POR_GRUPO];
    int golesEnContra[EQUIPOS_POR_GRUPO];

public:
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);
    ~Grupo();

    char getLetra() const;
    Equipo* getEquipo(int i) const;
    Partido* getPartidos() const;
    int getPuntaje(int i) const;
    int getDiferenciaGoles(int i) const;


    bool agregarEquipo(Equipo* e);
    int getCantidadEquipos() const;

    void configurarPartidos(const char* fechaInicio);

    void simularPartidos();

    void actualizarTabla();

    int* obtenerClasificacion() const;

    void mostrarGrupo() const;
    void mostrarTablaClasificacion() const;

    Grupo& operator=(const Grupo& otro);
};

#endif //GRUPO_H
