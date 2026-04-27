#include "equipo.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

Equipo::Equipo() : ranking(0), jugadores(nullptr), estadisticas(nullptr) {
    nombre[0] = '\0';
    confederacion[0] = '\0';
    jugadores   = new Jugador[MAX_JUGADORES];
    estadisticas = new EstadisticasEquipo();
}

Equipo::Equipo(const char* nom, const char* conf, int rank, const EstadisticasEquipo& stats)
    : ranking(rank) {

    int i = 0;
    while (i < 99 && nom[i] != '\0') {
        nombre[i] = nom[i];
        i++;
    }
    nombre[i] = '\0';

    i = 0;
    while (i < 49 && conf[i] != '\0') {
        confederacion[i] = conf[i];
        i++;
    }
    confederacion[i] = '\0';

    jugadores = new Jugador[MAX_JUGADORES];
    estadisticas = new EstadisticasEquipo(stats);
}


Equipo::Equipo(const Equipo& otro) : ranking(otro.ranking) {

    int i = 0;
    while (i < 99 && otro.nombre[i] != '\0') {
        nombre[i] = otro.nombre[i];
        i++;
    }
    nombre[i] = '\0';

    i = 0;
    while (i < 49 && otro.confederacion[i] != '\0') {
        confederacion[i] = otro.confederacion[i];
        i++;
    }
    confederacion[i] = '\0';

    jugadores = new Jugador[MAX_JUGADORES];
    for (int j = 0; j < MAX_JUGADORES; j++) {
        jugadores[j] = otro.jugadores[j];
    }

    estadisticas = new EstadisticasEquipo(*otro.estadisticas);
}

Equipo::~Equipo() {
    delete[] jugadores;
    delete estadisticas;
    jugadores   = nullptr;
    estadisticas = nullptr;
}

const char* Equipo::getNombre() const { return nombre; }
const char* Equipo::getConfederacion() const { return confederacion; }
int Equipo::getRanking() const { return ranking; }
Jugador* Equipo::getJugadores() const { return jugadores; }
EstadisticasEquipo* Equipo::getEstadisticas() const { return estadisticas; }

Jugador* Equipo::getJugador(int indice) const {
    if (indice < 0 || indice >= MAX_JUGADORES) return nullptr;
    return &jugadores[indice];
}

void Equipo::setNombre(const char* n) {

    int i = 0;

    while (i < 99 && n[i] != '\0') {
        nombre[i] = n[i];
        i++;
    }

    nombre[i] = '\0';
}

void Equipo::setConfederacion(const char* c) {

    int i = 0;

    while (i < 49 && c[i] != '\0') {
        confederacion[i] = c[i];
        i++;
    }

    confederacion[i] = '\0';
}

void Equipo::setRanking(int r) { ranking = r; }

void Equipo::inicializarJugadores() {
    int golesTotal  = estadisticas->getGolesFavor();
    int golesPorJug = golesTotal / MAX_JUGADORES;
    int resto       = golesTotal % MAX_JUGADORES;

    char nomBuf[50], apeBuf[50];

    for (int i = 0; i < MAX_JUGADORES; i++) {
        int camisa = i + 1;
        int k = 0;

        // Copiar nombre
        const char baseNom[] = "nombre";
        int j = 0;
        while (baseNom[j] != '\0') {
            nomBuf[k++] = baseNom[j++];
        }

        // Convertir número a texto
        int temp = camisa;
        char num[10];
        int len = 0;

        if (temp == 0) {
            num[len++] = '0';
        } else {
            while (temp > 0) {
                num[len++] = (temp % 10) + '0';
                temp /= 10;
            }
        }

        for (int m = len - 1; m >= 0; m--) {
            nomBuf[k++] = num[m];
        }

        nomBuf[k] = '\0';

        k = 0;

        const char baseApe[] = "apellido";
        j = 0;
        while (baseApe[j] != '\0') {
            apeBuf[k++] = baseApe[j++];
        }

        temp = camisa;
        len = 0;

        if (temp == 0) {
            num[len++] = '0';
        } else {
            while (temp > 0) {
                num[len++] = (temp % 10) + '0';
                temp /= 10;
            }
        }

        for (int m = len - 1; m >= 0; m--) {
            apeBuf[k++] = num[m];
        }

        apeBuf[k] = '\0';

        int golesJug = golesPorJug + (i < resto ? 1 : 0);
        EstadisticasJugador stats(0, golesJug, 0, 0, 0, 0, 0);
        jugadores[i] = Jugador(nomBuf, apeBuf, camisa, stats);
    }
}

int* Equipo::seleccionarOnce() const {
    int* indices = new int[MAX_JUGADORES];
    for (int i = 0; i < MAX_JUGADORES; i++) indices[i] = i;

    for (int i = MAX_JUGADORES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }

    int* once = new int[11];
    for (int i = 0; i < 11; i++) once[i] = indices[i];
    delete[] indices;
    return once;
}

EstadisticasEquipo* Equipo::getEstadisticasEquipo() const { return estadisticas; }

void Equipo::actualizarEstadisticas(int gf, int gc, int amarillas, int rojas, int faltas,
                                    bool gano, bool empato) {
    estadisticas->setGolesFavor(estadisticas->getGolesFavor() + gf);
    estadisticas->setGolesContra(estadisticas->getGolesContra() + gc);
    estadisticas->setAmarillas(estadisticas->getAmarillas() + amarillas);
    estadisticas->setRojas(estadisticas->getRojas() + rojas);
    estadisticas->setFaltas(estadisticas->getFaltas() + faltas);
    if (gano) estadisticas->setGanados(estadisticas->getGanados() + 1);
    else if (empato) estadisticas->setEmpatados(estadisticas->getEmpatados() + 1);
    else estadisticas->setPerdidos(estadisticas->getPerdidos() + 1);
}

bool Equipo::operator<(const Equipo& otro) const {
    return ranking < otro.ranking;
}

bool Equipo::operator==(const Equipo& otro) const {
    int i = 0;
    while (nombre[i] != '\0' && otro.nombre[i] != '\0') {
        if (nombre[i] != otro.nombre[i]) {
            return false;
        }
        i++;
    }

    // Verificar que ambos hayan terminado al mismo tiempo
    return nombre[i] == '\0' && otro.nombre[i] == '\0';
}

Equipo& Equipo::operator=(const Equipo& otro) {
    if (this != &otro) {

        int i = 0;
        while (i < 99 && otro.nombre[i] != '\0') {
            nombre[i] = otro.nombre[i];
            i++;
        }
        nombre[i] = '\0';

        i = 0;
        while (i < 49 && otro.confederacion[i] != '\0') {
            confederacion[i] = otro.confederacion[i];
            i++;
        }
        confederacion[i] = '\0';

        ranking = otro.ranking;

        for (int j = 0; j < MAX_JUGADORES; j++) {
            jugadores[j] = otro.jugadores[j];
        }

        delete estadisticas;
        estadisticas = new EstadisticasEquipo(*otro.estadisticas);
    }
    return *this;
}


void Equipo::mostrar() const {
    cout << nombre << " (" << confederacion << ") Ranking:" << ranking << endl;
    estadisticas->mostrar();
}

void Equipo::mostrarConJugadores() const {
    mostrar();
    for (int i = 0; i < MAX_JUGADORES; i++) jugadores[i].mostrar();
}

void Equipo::guardarHistorico() const {

    char ruta[220] = "historico/";

    int i = 0, pos = 10;

    while (nombre[i] != '\0' && pos < 210) {
        ruta[pos++] = (nombre[i] == ' ') ? '_' : nombre[i];
        i++;
    }

    ruta[pos++] = '.';
    ruta[pos++] = 'c';
    ruta[pos++] = 's';
    ruta[pos++] = 'v';
    ruta[pos] = '\0';

    FILE* f = fopen(ruta, "w");

    if (f == NULL) {
        fs::create_directory("historico");
        f = fopen(ruta, "w");
        if (f == NULL) return;
    }

    fprintf(f, "numero;nombre;apellido;partidos;goles;minutos;faltas;amarillas;rojas;asistencias\n");

    for (int i = 0; i < MAX_JUGADORES; i++) {

        EstadisticasJugador* ej = jugadores[i].getEstadisticas();
        if (!ej) continue;

        // número
        fprintf(f, "%d;", jugadores[i].getnumeroCamisa());

        // nombre y apellido
        fprintf(f, "%s;", jugadores[i].getNombre());
        fprintf(f, "%s;", jugadores[i].getApellido());

        // estadísticas
        fprintf(f, "%d;%d;%d;%d;%d;%d;%d\n",
                ej->getPartidos(),
                ej->getGoles(),
                ej->getMinutos(),
                ej->getFaltas(),
                ej->getAmarillas(),
                ej->getRojas(),
                ej->getAsistencias()
                );
    }

    fclose(f);
}
void Equipo::cargarHistorico() {

    char ruta[220] = "historico/";

    int i = 0;
    int pos = 10;

    while (nombre[i] != '\0' && pos < 210) {
        if (nombre[i] == ' ')
            ruta[pos++] = '_';
        else
            ruta[pos++] = nombre[i];
        i++;
    }

    ruta[pos++] = '.';
    ruta[pos++] = 'c';
    ruta[pos++] = 's';
    ruta[pos++] = 'v';
    ruta[pos] = '\0';

    FILE* f = fopen(ruta, "r");
    if (f == NULL) return;

    char linea[256];

    fgets(linea, 256, f); // saltar encabezado

    int j = 0;

    while (fgets(linea, 256, f) && j < MAX_JUGADORES) {

        int num, partidos, goles, minutos, faltas, amarillas, rojas, asistencias;
        char nom[50], ape[50];

        int pos = 0;

        num = 0;
        while (linea[pos] != ';') {
            num = num * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        int k = 0;
        while (linea[pos] != ';') {
            nom[k++] = linea[pos++];
        }
        nom[k] = '\0';
        pos++;

        k = 0;
        while (linea[pos] != ';') {
            ape[k++] = linea[pos++];
        }
        ape[k] = '\0';
        pos++;

        partidos = 0;
        while (linea[pos] != ';') {
            partidos = partidos * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        goles = 0;
        while (linea[pos] != ';') {
            goles = goles * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        minutos = 0;
        while (linea[pos] != ';') {
            minutos = minutos * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        faltas = 0;
        while (linea[pos] != ';') {
            faltas = faltas * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        amarillas = 0;
        while (linea[pos] != ';') {
            amarillas = amarillas * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        rojas = 0;
        while (linea[pos] != ';') {
            rojas = rojas * 10 + (linea[pos] - '0');
            pos++;
        }
        pos++;

        asistencias = 0;
        while (linea[pos] != '\0' && linea[pos] != '\n') {
            asistencias = asistencias * 10 + (linea[pos] - '0');
            pos++;
        }


        jugadores[j].setnumeroCamisa(num);
        jugadores[j].setNombre(nom);
        jugadores[j].setApellido(ape);

        EstadisticasJugador stats(partidos, goles, minutos, faltas, amarillas, rojas, asistencias);
        *jugadores[j].getEstadisticas() = stats;

        j++;
    }

    fclose(f);
}

