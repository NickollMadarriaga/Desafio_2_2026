#include "partido.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

// ---- Helpers locales ----
static float randFloat() { return (float)rand() / RAND_MAX; }

// Distribucion de Poisson: genera un entero con media lambda
static int poisson(float lambda) {
    // Metodo de Knuth
    float L = expf(-lambda);
    int k = 0;
    float p = 1.0f;
    do {
        k++;
        p *= randFloat();
    } while (p > L);
    return k - 1;
}

// ---- Partido ----
Partido::Partido()
    : equipo1(nullptr), equipo2(nullptr),
    statsEquipo1(nullptr), statsEquipo2(nullptr),
    convocados1(nullptr), convocados2(nullptr),
    statsJugadores1(nullptr), statsJugadores2(nullptr),
    simulado(false), fueProrroga(false) {
    fecha[0] = hora[0] = sede[0] = '\0';
    for (int i = 0; i < MAX_ARBITROS; i++) arbitros[i][0] = '\0';
    statsEquipo1 = new EstadisticasPartido();
    statsEquipo2 = new EstadisticasPartido();
}

Partido::Partido(Equipo* e1, Equipo* e2, const char* fec, const char* hor,
                 const char* sed, const char arb[][30])
    : equipo1(e1), equipo2(e2), simulado(false), fueProrroga(false),
    convocados1(nullptr), convocados2(nullptr),
    statsJugadores1(nullptr), statsJugadores2(nullptr) {
    strncpy(fecha, fec, 10); fecha[10] = '\0';
    strncpy(hora,  hor,  5);  hora[5]  = '\0';
    strncpy(sede,  sed, 99);  sede[99]  = '\0';
    for (int i = 0; i < MAX_ARBITROS; i++) {
        strncpy(arbitros[i], arb[i], 29); arbitros[i][29] = '\0';
    }
    statsEquipo1 = new EstadisticasPartido();
    statsEquipo2 = new EstadisticasPartido();
}

Partido::Partido(const Partido& otro)
    : equipo1(otro.equipo1), equipo2(otro.equipo2),
    simulado(otro.simulado), fueProrroga(otro.fueProrroga) {
    strncpy(fecha, otro.fecha, 10); fecha[10] = '\0';
    strncpy(hora,  otro.hora,   5); hora[5]   = '\0';
    strncpy(sede,  otro.sede,  99); sede[99]  = '\0';
    for (int i = 0; i < MAX_ARBITROS; i++) {
        strncpy(arbitros[i], otro.arbitros[i], 29); arbitros[i][29] = '\0';
    }
    statsEquipo1 = new EstadisticasPartido(*otro.statsEquipo1);
    statsEquipo2 = new EstadisticasPartido(*otro.statsEquipo2);

    // Copiar convocados y stats de jugadores
    if (otro.convocados1) {
        convocados1 = new int[JUGADORES_POR_PARTIDO];
        for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) convocados1[i] = otro.convocados1[i];
    } else convocados1 = nullptr;

    if (otro.convocados2) {
        convocados2 = new int[JUGADORES_POR_PARTIDO];
        for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) convocados2[i] = otro.convocados2[i];
    } else convocados2 = nullptr;

    if (otro.statsJugadores1) {
        statsJugadores1 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
        for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) statsJugadores1[i] = otro.statsJugadores1[i];
    } else statsJugadores1 = nullptr;

    if (otro.statsJugadores2) {
        statsJugadores2 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
        for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) statsJugadores2[i] = otro.statsJugadores2[i];
    } else statsJugadores2 = nullptr;
}

Partido::~Partido() {
    delete statsEquipo1;
    delete statsEquipo2;
    delete[] convocados1;
    delete[] convocados2;
    delete[] statsJugadores1;
    delete[] statsJugadores2;
}

Equipo* Partido::getEquipo1() const { return equipo1; }
Equipo* Partido::getEquipo2() const { return equipo2; }
const char* Partido::getFecha() const { return fecha; }
const char* Partido::getHora() const { return hora; }
const char* Partido::getSede() const { return sede; }
EstadisticasPartido* Partido::getStatsEquipo1() const { return statsEquipo1; }
EstadisticasPartido* Partido::getStatsEquipo2() const { return statsEquipo2; }
bool Partido::getFueProrroga() const { return fueProrroga; }
bool Partido::isSimulado() const { return simulado; }

void Partido::setFecha(const char* f) {
    int i = 0;
    while (i < 10 && f[i] != '\0') {
        fecha[i] = f[i];
        i++;
    }
    fecha[i] = '\0';
}

void Partido::setHora(const char* h) {
    int i = 0;
    while (i < 5 && h[i] != '\0') {
        hora[i] = h[i];
        i++;
    }
    hora[i] = '\0';
}

void Partido::setSede(const char* s) {
    int i = 0;
    while (i < 99 && s[i] != '\0') {
        sede[i] = s[i];
        i++;
    }
    sede[i] = '\0';
}

void Partido::setEquipo1(Equipo* e)   { equipo1 = e; }
void Partido::setEquipo2(Equipo* e)   { equipo2 = e; }


static float calcularLambda(float gfa, float gcb) {
    const float beta  = 0.4f;
    const float alpha = 0.6f;
    const float mu    = 1.35f;
    // lambda_A = mu * (GFA/mu)^alpha * (GCB/mu)^beta
    return mu * powf(gfa / mu, alpha) * powf(gcb / mu, beta);
}

static void simularJugadores(Equipo* eq, int* convocados,
                             EstadisticasJugadorPartido* statsJ,
                             int golesEquipo, int minutos) {
    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        statsJ[i].numeroCamiseta = eq->getJugador(convocados[i])->getnumeroCamisa();
        statsJ[i].goles     = 0;
        statsJ[i].amarillas = 0;
        statsJ[i].rojas     = 0;
        statsJ[i].faltas    = 0;
        statsJ[i].minutos   = minutos;
    }

    // Distribuir goles entre los 11 jugadores (4% por jugador hasta llegar al total)
    int golesRestantes = golesEquipo;
    for (int i = 0; i < JUGADORES_POR_PARTIDO && golesRestantes > 0; i++) {
        if (randFloat() < 0.04f) {
            statsJ[i].goles++;
            golesRestantes--;
        }
    }
    // Si quedan goles, asignarlos al primer jugador
    if (golesRestantes > 0) statsJ[0].goles += golesRestantes;

    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        if (randFloat() < 0.06f) {
            statsJ[i].amarillas++;
            if (randFloat() < 0.0115f) {
                statsJ[i].amarillas++;
                statsJ[i].rojas++;
            }
        }
    }

    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        if (randFloat() < 0.13f) {
            statsJ[i].faltas++;
            if (randFloat() < 0.0275f) {
                statsJ[i].faltas++;
                if (randFloat() < 0.007f) {
                    statsJ[i].faltas++;
                }
            }
        }
    }
}

int Partido::simularPartido() {
    if (simulado) return 0;

    delete[] convocados1; delete[] convocados2;
    convocados1 = equipo1->seleccionarOnce();
    convocados2 = equipo2->seleccionarOnce();

    // Calcular lambda para cada equipo
    float gfa1 = equipo1->getEstadisticas()->getPromedioGolesFavor();
    float gcb2 = equipo2->getEstadisticas()->getPromedioGolesContra();
    float gfa2 = equipo2->getEstadisticas()->getPromedioGolesFavor();
    float gcb1 = equipo1->getEstadisticas()->getPromedioGolesContra();

    float lambda1 = calcularLambda(gfa1, gcb2);
    float lambda2 = calcularLambda(gfa2, gcb1);
    int goles1 = poisson(lambda1);
    int goles2 = poisson(lambda2);

    int r1 = equipo1->getRanking();
    int r2 = equipo2->getRanking();
    float posesion1 = (float)r2 / (r1 + r2) * 100.0f;
    float posesion2 = 100.0f - posesion1;

    statsEquipo1->setGoles(goles1);
    statsEquipo2->setGoles(goles2);
    statsEquipo1->setPosesion(posesion1);
    statsEquipo2->setPosesion(posesion2);

    delete[] statsJugadores1; delete[] statsJugadores2;
    statsJugadores1 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
    statsJugadores2 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
    simularJugadores(equipo1, convocados1, statsJugadores1, goles1, 90);
    simularJugadores(equipo2, convocados2, statsJugadores2, goles2, 90);

    int am1 = 0, rj1 = 0, fal1 = 0;
    int am2 = 0, rj2 = 0, fal2 = 0;
    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        am1  += statsJugadores1[i].amarillas; rj1 += statsJugadores1[i].rojas; fal1 += statsJugadores1[i].faltas;
        am2  += statsJugadores2[i].amarillas; rj2 += statsJugadores2[i].rojas; fal2 += statsJugadores2[i].faltas;
    }
    statsEquipo1->setAmarillas(am1); statsEquipo1->setRojas(rj1); statsEquipo1->setFaltas(fal1);
    statsEquipo2->setAmarillas(am2); statsEquipo2->setRojas(rj2); statsEquipo2->setFaltas(fal2);

    simulado = true;

    if (goles1 > goles2) return 1;
    if (goles2 > goles1) return 2;
    return 0;
}

int Partido::simularPartidoConProrroga() {
    int resultado = simularPartido();

    if (resultado == 0) {
        fueProrroga = true;
        int r1 = equipo1->getRanking();
        int r2 = equipo2->getRanking();
        // Menor ranking: mas probabilidad de ganar
        float probE1 = (float)r2 / (r1 + r2);
        int ganador = (randFloat() < probE1) ? 1 : 2;

        if (ganador == 1) {
            statsEquipo1->setGoles(statsEquipo1->getGoles() + 1);
        } else {
            statsEquipo2->setGoles(statsEquipo2->getGoles() + 1);
        }

        for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
            statsJugadores1[i].minutos = 120;
            statsJugadores2[i].minutos = 120;
        }
        return ganador;
    }
    return resultado;
}

void Partido::actualizarDatos() {
    if (!simulado) return;

    int g1 = statsEquipo1->getGoles();
    int g2 = statsEquipo2->getGoles();
    bool gano1  = g1 > g2;
    bool empato = g1 == g2;

    equipo1->actualizarEstadisticas(g1, g2,
                                    statsEquipo1->getAmarillas(), statsEquipo1->getRojas(),
                                    statsEquipo1->getFaltas(), gano1, empato);
    equipo2->actualizarEstadisticas(g2, g1,
                                    statsEquipo2->getAmarillas(), statsEquipo2->getRojas(),
                                    statsEquipo2->getFaltas(), !gano1 && !empato, empato);

    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        equipo1->getJugador(convocados1[i])->actualizarEstadisticas(
            statsJugadores1[i].goles,
            statsJugadores1[i].minutos,
            statsJugadores1[i].faltas,
            statsJugadores1[i].amarillas,
            statsJugadores1[i].rojas);

        equipo2->getJugador(convocados2[i])->actualizarEstadisticas(
            statsJugadores2[i].goles,
            statsJugadores2[i].minutos,
            statsJugadores2[i].faltas,
            statsJugadores2[i].amarillas,
            statsJugadores2[i].rojas);
    }
}

void Partido::mostrar() const {
    cout << "[" << fecha << " " << hora << "] "
         << equipo1->getNombre() << " " << statsEquipo1->getGoles()
         << " - "
         << statsEquipo2->getGoles() << " " << equipo2->getNombre();
    if (fueProrroga) cout << " (Prorroga)";
    cout << "  Sede: " << sede << endl;
    cout<< "Arbitros:" <<arbitros[0]<<","<< arbitros[1]<<arbitros[2]<<endl;
    cout << "  " << equipo1->getNombre() << ": ";
    statsEquipo1->mostrar();
    cout << "  " << equipo2->getNombre() << ": ";
    statsEquipo2->mostrar();
}

void Partido::mostrarGoleadores() const {
    cout << "  Goleadores " << equipo1->getNombre() << ": ";
    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        if (statsJugadores1[i].goles > 0) {
            cout << "#" << statsJugadores1[i].numeroCamiseta
                 << "(" << statsJugadores1[i].goles << ") ";
        }
    }
    cout << endl;
    cout << "  Goleadores " << equipo2->getNombre() << ": ";
    for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) {
        if (statsJugadores2[i].goles > 0) {
            cout << "#" << statsJugadores2[i].numeroCamiseta
                 << "(" << statsJugadores2[i].goles << ") ";
        }
    }
    cout << endl;
}

Partido& Partido::operator=(const Partido& otro) {
    if (this != &otro) {
        equipo1 = otro.equipo1;
        equipo2 = otro.equipo2;
        int i = 0;
        while (i < 10 && otro.fecha[i] != '\0') {
            fecha[i] = otro.fecha[i];
            i++;
        }
        fecha[i] = '\0';

        i = 0;
        while (i < 5 && otro.hora[i] != '\0') {
            hora[i] = otro.hora[i];
            i++;
        }
        hora[i] = '\0';

        i = 0;
        while (i < 99 && otro.sede[i] != '\0') {
            sede[i] = otro.sede[i];
            i++;
        }
        sede[i] = '\0';

        for (int i = 0; i < MAX_ARBITROS; i++) {
            int j = 0;
            while (j < 29 && otro.arbitros[i][j] != '\0') {
                arbitros[i][j] = otro.arbitros[i][j];
                j++;
            }
            arbitros[i][j] = '\0';
        }

        delete statsEquipo1; statsEquipo1 = new EstadisticasPartido(*otro.statsEquipo1);
        delete statsEquipo2; statsEquipo2 = new EstadisticasPartido(*otro.statsEquipo2);
        simulado    = otro.simulado;
        fueProrroga = otro.fueProrroga;

        delete[] convocados1; convocados1 = nullptr;
        delete[] convocados2; convocados2 = nullptr;
        delete[] statsJugadores1; statsJugadores1 = nullptr;
        delete[] statsJugadores2; statsJugadores2 = nullptr;

        if (otro.convocados1) {
            convocados1 = new int[JUGADORES_POR_PARTIDO];
            for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) convocados1[i] = otro.convocados1[i];
        }
        if (otro.convocados2) {
            convocados2 = new int[JUGADORES_POR_PARTIDO];
            for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) convocados2[i] = otro.convocados2[i];
        }
        if (otro.statsJugadores1) {
            statsJugadores1 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
            for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) statsJugadores1[i] = otro.statsJugadores1[i];
        }
        if (otro.statsJugadores2) {
            statsJugadores2 = new EstadisticasJugadorPartido[JUGADORES_POR_PARTIDO];
            for (int i = 0; i < JUGADORES_POR_PARTIDO; i++) statsJugadores2[i] = otro.statsJugadores2[i];
        }
    }
    return *this;
}