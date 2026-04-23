#ifndef ELIMINATORIAS_H
#define ELIMINATORIAS_H

#include "Partido.h"

#define MAX_PARTIDOS_ETAPA 32

class Eliminatorias {
private:
    char etapa[30];       // "R16", "R8", "QF", "SF", "3er", "Final"
    Partido* partidos;
    int numPartidos;
    Equipo** clasificados;
    int numClasificados;

public:
    Eliminatorias();
    Eliminatorias(const char* etapa, int numPartidos);
    Eliminatorias(const Eliminatorias& otro);
    ~Eliminatorias();

    const char* getEtapa() const;
    Partido* getPartidos() const;
    int getNumPartidos() const;
    Equipo** getClasificados() const;
    int getNumClasificados() const;

    void setEtapa(const char* e);

    void agregarPartido(Equipo* e1, Equipo* e2, const char* fecha);

    void simularEtapa();

    void obtenerClasificados();

    void mostrar() const;

    Eliminatorias& operator=(const Eliminatorias& otro);
};

#endif //ELIMINATORIAS_H
