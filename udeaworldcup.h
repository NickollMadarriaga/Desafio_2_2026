#ifndef UDEAWORLDCUP_H
#define UDEAWORLDCUP_H

#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"
#include "Eliminatorias.h"

#define MAX_EQUIPOS 48
#define NUM_GRUPOS 12
#define EQUIPOS_POR_BOMBO 12
#define NUM_BOMBOS 4

class UdeAWorldCup {
private:
    Equipo* equipos;
    Grupo* grupos;
    Partido* partidos;
    Eliminatorias* eliminatorias; // Etapas: R16, R8, QF, SF, 3er, Final
    int totalEquipos;
    int totalPartidos;

    long long contadorIteraciones;
    long long memoriaConsumida;

public:
    UdeAWorldCup();
    ~UdeAWorldCup();
    bool son_Iguales(const char* a, const char* b);
    void cargarDatos();
    void generarGrupos();
    void simularFaseGrupos();
    void simularEliminatorias();
    void mostrarEstadisticas();

    void medirEficiencia();

private:

    bool leerCSV(const char* ruta);
    void ordenarEquiposPorRanking();
    void armarBombos(Equipo* bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO]);
    bool confederacionPermitida(Grupo* g, const char* confederacion);

    // Retorna arreglo de 32 punteros a Equipo
    Equipo** construirTablaClasificacion();

    void configurarR16(Equipo** clasificados32);
    long long calcularMemoria() const;
    void contarIteracion(int n = 1);
};

#endif // UDEAWORLDCUP_H
