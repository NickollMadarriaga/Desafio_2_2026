#include "UdeAWorldCup.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

UdeAWorldCup::UdeAWorldCup()
    : equipos(nullptr), grupos(nullptr), partidos(nullptr), eliminatorias(nullptr),
    totalEquipos(0), totalPartidos(0),
    contadorIteraciones(0), memoriaConsumida(0) {
    equipos      = new Equipo[MAX_EQUIPOS];
    grupos       = new Grupo[NUM_GRUPOS];
    partidos     = new Partido[72]; // 12 grupos x 6 partidos
    eliminatorias = new Eliminatorias[6]; // R16, R8, QF, SF, 3er, Final
}

UdeAWorldCup::~UdeAWorldCup() {
    delete[] equipos;
    delete[] grupos;
    delete[] partidos;
    delete[] eliminatorias;
}
bool UdeAWorldCup::son_Iguales(const char* a, const char* b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}


// Utilidad: extrae el siguiente campo delimitado por ';' desde pos, avanza pos
static int extraerCampo(const char* linea, int pos, char* dest, int maxLen) {
    int i = 0;
    while (linea[pos] != '\0' && linea[pos] != ';' &&
           linea[pos] != '\n' && linea[pos] != '\r') {
        if (i < maxLen - 1) dest[i++] = linea[pos];
        pos++;
    }
    dest[i] = '\0';
    if (linea[pos] == ';') pos++;
    return pos;
}

bool UdeAWorldCup::leerCSV(const char* ruta) {
    FILE* f = fopen(ruta, "r");
    if (!f) {
        cout << "[ERROR] No se pudo abrir: " << ruta << endl;
        return false;
    }

    char linea[512];

    // Saltar las dos primeras lineas titulo y encabezado de columnas
    fgets(linea, sizeof(linea), f);
    fgets(linea, sizeof(linea), f);
    contarIteracion();

    totalEquipos = 0;

    while (fgets(linea, sizeof(linea), f) && totalEquipos < MAX_EQUIPOS) {
        contarIteracion();

        int inicio = 0;
        if ((unsigned char)linea[0] == 0xEF &&
            (unsigned char)linea[1] == 0xBB &&
            (unsigned char)linea[2] == 0xBF) inicio = 3;

        char bufRanking[10], pais[100], dirTec[100], fed[150], conf[50];
        char bufGF[10], bufGC[10], bufGan[10], bufEmp[10], bufPer[10];

        int pos = inicio;
        pos = extraerCampo(linea, pos, bufRanking, 10);
        pos = extraerCampo(linea, pos, pais,       100);
        pos = extraerCampo(linea, pos, dirTec,     100);
        pos = extraerCampo(linea, pos, fed,        150);
        pos = extraerCampo(linea, pos, conf,        50);
        pos = extraerCampo(linea, pos, bufGF,       10);
        pos = extraerCampo(linea, pos, bufGC,       10);
        pos = extraerCampo(linea, pos, bufGan,      10);
        pos = extraerCampo(linea, pos, bufEmp,      10);
        pos = extraerCampo(linea, pos, bufPer,      10);

        if (pais[0] == '\0' || bufRanking[0] == '\0') continue;

        int ranking = atoi(bufRanking);
        int gf  = atoi(bufGF);
        int gc  = atoi(bufGC);
        int gan = atoi(bufGan);
        int emp = atoi(bufEmp);
        int per = atoi(bufPer);

        if (son_Iguales(conf, "Concacaf")) {
            strcpy(conf, "CONCACAF");
            conf[0]='C'; conf[1]='O'; conf[2]='N'; conf[3]='C';
            conf[4]='A'; conf[5]='C'; conf[6]='A'; conf[7]='F'; conf[8]='\0';
        }

        EstadisticasEquipo stats(gf, gc, gan, emp, per, 0, 0, 0);
        equipos[totalEquipos] = Equipo(pais, conf, ranking, stats);
        equipos[totalEquipos].inicializarJugadores();
        totalEquipos++;
    }
    fclose(f);
    cout << "[OK] Cargados " << totalEquipos << " equipos desde CSV." << endl;
    return true;
}

void UdeAWorldCup::cargarDatos() {
    contadorIteraciones = 0;
    cout << "\n Cargando datos" << endl;
    cout << "Ingrese la ruta del archivo CSV: ";
    char rutaCSV[256];
    cin >> rutaCSV;
    if (!leerCSV(rutaCSV)) {
        const char* nombres[] = {
            "USA","Mexico","Canada","Brasil","Argentina","Francia","Alemania","Espana",
            "Portugal","Inglaterra","Holanda","Belgica","Italia","Croacia","Uruguay","Colombia",
            "Marruecos","Senegal","Ghana","Camerun","Nigeria","Costa_de_Marfil","Japon","Corea",
            "Australia","Arabia_Saudita","Iran","Qatar","Ecuador","Peru","Chile","Venezuela",
            "Dinamarca","Suecia","Noruega","Suiza","Austria","Serbia","Polonia","Rumania",
            "Escocia","Gales","Irlanda","Turquia","Ucrania","Eslovenia","Albania","Eslovaquia"
        };
        const char* confs[] = {
            "CONCACAF","CONCACAF","CONCACAF","CONMEBOL","CONMEBOL","UEFA","UEFA","UEFA",
            "UEFA","UEFA","UEFA","UEFA","UEFA","UEFA","CONMEBOL","CONMEBOL",
            "CAF","CAF","CAF","CAF","CAF","CAF","AFC","AFC",
            "AFC","AFC","AFC","AFC","CONMEBOL","CONMEBOL","CONMEBOL","CONMEBOL",
            "UEFA","UEFA","UEFA","UEFA","UEFA","UEFA","UEFA","UEFA",
            "UEFA","UEFA","UEFA","UEFA","UEFA","UEFA","UEFA","UEFA"
        };
        int rankings[] = {
            11,15,49,5,1,2,12,8,6,4,7,3,9,10,18,27,
            13,20,61,43,35,52,17,23,24,56,21,37,45,22,30,46,
            19,26,33,16,28,36,25,47,31,38,55,34,29,41,67,44
        };
        totalEquipos = 48;
        for (int i = 0; i < 48; i++) {
            contarIteracion();
            int gf = 30 + rand() % 100;
            int gc = 20 + rand() % 80;
            int gan = 10 + rand() % 30;
            int emp = 5  + rand() % 15;
            int per = 5  + rand() % 15;
            EstadisticasEquipo stats(gf, gc, gan, emp, per, 0, 0, 0);
            equipos[i] = Equipo(nombres[i], confs[i], rankings[i], stats);
            equipos[i].inicializarJugadores();
        }
        cout << "[OK] Datos de prueba inicializados para 48 equipos." << endl;
    }
    medirEficiencia();
}

void UdeAWorldCup::ordenarEquiposPorRanking() {
    // Ordenamiento burbuja por ranking FIFA
    for (int i = 0; i < totalEquipos - 1; i++) {
        for (int j = 0; j < totalEquipos - 1 - i; j++) {
            contarIteracion();
            if (equipos[j].getRanking() > equipos[j + 1].getRanking()) {
                Equipo tmp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = tmp;
            }
        }
    }
}

bool UdeAWorldCup::confederacionPermitida(Grupo* g, const char* conf) {
    int count = 0;
    bool esUEFA = son_Iguales(conf, "UEFA") == 0;
    for (int i = 0; i < 4; i++) {
        contarIteracion();
        Equipo* e = g->getEquipo(i);
        if (!e) continue;
        if (strcmp(e->getConfederacion(), conf) == 0) {
            count++;
            if (!esUEFA) return false;
            if (count >= 2) return false;
        }
    }
    return true;
}

void UdeAWorldCup::generarGrupos() {
    contadorIteraciones = 0;
    cout << "\n Generando grupos" << endl;
    ordenarEquiposPorRanking();

    // Colocar USA en el bombo 1 primer lugar
    int idxUSA = -1;
    for (int i = 0; i < totalEquipos; i++) {
        contarIteracion();
        if (son_Iguales(equipos[i].getNombre(), "USA") == 0 ||
            son_Iguales(equipos[i].getNombre(), "Estados_Unidos") == 0 ||
            son_Iguales(equipos[i].getNombre(), "United_States") == 0) {
            idxUSA = i; break;
        }
    }
    // Mover USA al indice 0 del bombo 1 si no esta ya
    if (idxUSA > 0) {
        Equipo tmp = equipos[0];
        equipos[0] = equipos[idxUSA];
        equipos[idxUSA] = tmp;
    }

    for (int b = 0; b < NUM_BOMBOS; b++) {
        int inicio = b * EQUIPOS_POR_BOMBO;
        int desde = (b == 0) ? inicio + 1 : inicio;
        int hasta = inicio + EQUIPOS_POR_BOMBO - 1;
        for (int i = hasta; i > desde; i--) {
            contarIteracion();
            int j = desde + rand() % (i - desde + 1);
            Equipo tmp = equipos[i]; equipos[i] = equipos[j]; equipos[j] = tmp;
        }
    }

    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i] = Grupo('A' + i);
    }

    for (int b = 0; b < NUM_BOMBOS; b++) {
        int inicio = b * EQUIPOS_POR_BOMBO;
        bool usado[EQUIPOS_POR_BOMBO] = {false};

        for (int g = 0; g < NUM_GRUPOS; g++) {
            contarIteracion();
            bool asignado = false;
            for (int intento = 0; intento < EQUIPOS_POR_BOMBO * 3 && !asignado; intento++) {
                contarIteracion();
                int idx = inicio + rand() % EQUIPOS_POR_BOMBO;
                if (b == 0 && g == 0) idx = 0;
                if (usado[idx - inicio]) continue;
                if (confederacionPermitida(&grupos[g], equipos[idx].getConfederacion())) {
                    grupos[g].agregarEquipo(&equipos[idx]);
                    usado[idx - inicio] = true;
                    asignado = true;
                }
            }
            if (!asignado) {
                for (int k = 0; k < EQUIPOS_POR_BOMBO; k++) {
                    contarIteracion();
                    if (!usado[k]) {
                        grupos[g].agregarEquipo(&equipos[inicio + k]);
                        usado[k] = true;
                        break;
                    }
                }
            }
        }
    }

    cout << "\n GRUPOS CONFORMADOS " << endl;
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].mostrarGrupo();
    }
    medirEficiencia();
}


void UdeAWorldCup::simularFaseGrupos() {
    contadorIteraciones = 0;
    cout << "\n Simulando fase de grupos " << endl;

    for (int i = 0; i < NUM_GRUPOS; i++) {
        contarIteracion();
        grupos[i].configurarPartidos("20/06/2026");
        grupos[i].simularPartidos();
        cout << "\n"; grupos[i].mostrarTablaClasificacion();
        Partido* ps = grupos[i].getPartidos();
        for (int p = 0; p < PARTIDOS_POR_GRUPO; p++) {
            contarIteracion();
            ps[p].mostrar();
            ps[p].mostrarGoleadores();
        }
    }
    medirEficiencia();
}

Equipo** UdeAWorldCup::construirTablaClasificacion() {
    // 12 primeros + 12 segundos + 8 mejores terceros = 32
    Equipo** clasificados = new Equipo*[32];
    for (int i = 0; i < 32; i++) clasificados[i] = nullptr;

    Equipo* terceros[NUM_GRUPOS];
    int puntosTerceros[NUM_GRUPOS];
    int difGolesTerceros[NUM_GRUPOS];
    int gfTerceros[NUM_GRUPOS];

    int idx = 0;
    for (int g = 0; g < NUM_GRUPOS; g++) {
        contarIteracion();
        int* orden = grupos[g].obtenerClasificacion();
        clasificados[idx++] = grupos[g].getEquipo(orden[0]);
        clasificados[idx++] = grupos[g].getEquipo(orden[1]);
        terceros[g]         = grupos[g].getEquipo(orden[2]);
        puntosTerceros[g]   = grupos[g].getPuntaje(orden[2]);
        difGolesTerceros[g] = grupos[g].getDiferenciaGoles(orden[2]);
        gfTerceros[g]       = grupos[g].getEquipo(orden[2])
                            ->getEstadisticas()->getGolesFavor();
        delete[] orden;
    }

    // Burbuja sobre los 12 terceros
    for (int i = 0; i < NUM_GRUPOS - 1; i++) {
        for (int j = 0; j < NUM_GRUPOS - 1 - i; j++) {
            contarIteracion();
            bool swap = false;
            if (puntosTerceros[j] < puntosTerceros[j+1]) swap = true;
            else if (puntosTerceros[j] == puntosTerceros[j+1]) {
                if (difGolesTerceros[j] < difGolesTerceros[j+1]) swap = true;
                else if (difGolesTerceros[j] == difGolesTerceros[j+1] &&
                         gfTerceros[j] < gfTerceros[j+1]) swap = true;
            }
            if (swap) {
                Equipo* te = terceros[j]; terceros[j] = terceros[j+1]; terceros[j+1] = te;
                int tp = puntosTerceros[j]; puntosTerceros[j] = puntosTerceros[j+1]; puntosTerceros[j+1] = tp;
                int td = difGolesTerceros[j]; difGolesTerceros[j] = difGolesTerceros[j+1]; difGolesTerceros[j+1] = td;
                int tg = gfTerceros[j]; gfTerceros[j] = gfTerceros[j+1]; gfTerceros[j+1] = tg;
            }
        }
    }
    // Agregar los 8 mejores terceros
    for (int i = 0; i < 8; i++) {
        clasificados[idx++] = terceros[i];
    }

    cout << "\n TABLAS DE CLASIFICACION " << endl;
    for (int g = 0; g < NUM_GRUPOS; g++) {
        grupos[g].mostrarTablaClasificacion();
    }
    return clasificados;
}

void UdeAWorldCup::configurarR16(Equipo** clas) {

    eliminatorias[0] = Eliminatorias("R16", 16);

    // Bloque 1: cabezas 0-7 vs terceros 24-31  (8 partidos)
    for (int i = 0; i < 8; i++) {
        contarIteracion();
        eliminatorias[0].agregarPartido(clas[i], clas[24 + i], "10/07/2026");
    }

    // Bloque 2: cabezas 8-11 vs 4 peores segundos 20-23  (4 partidos)
    for (int i = 0; i < 4; i++) {
        contarIteracion();
        eliminatorias[0].agregarPartido(clas[8 + i], clas[20 + i], "10/07/2026");
    }

    // Bloque 3: segundos 12-19 entre si en pares  (4 partidos)
    for (int i = 0; i < 8; i += 2) {
        contarIteracion();
        eliminatorias[0].agregarPartido(clas[12 + i], clas[13 + i], "10/07/2026");
    }

    cout << "\n PARTIDOS R16 CONFIGURADOS (sin simular) " << endl;
    Partido* ps = eliminatorias[0].getPartidos();
    for (int i = 0; i < 16; i++) {
        contarIteracion();
        if (ps[i].getEquipo1()) {
            cout << "  " << ps[i].getEquipo1()->getNombre()
            << " vs " << ps[i].getEquipo2()->getNombre() << endl;
        }
    }
}

void UdeAWorldCup::simularEliminatorias() {
    contadorIteraciones = 0;
    cout << "\n Construyendo tabla y configurando R16 " << endl;
    Equipo** clas32 = construirTablaClasificacion();
    configurarR16(clas32);
    delete[] clas32;

    const char* etapas[] = {"R16", "R8", "QF", "SF", "3er", "Final"};
    int numPartidos[]    = {16,     8,    4,    2,    1,     1};

    for (int e = 0; e < 6; e++) {
        contarIteracion();
        if (e > 0) {
            eliminatorias[e] = Eliminatorias(etapas[e], numPartidos[e]);

            if (e == 4) {
                Partido* psSF = eliminatorias[3].getPartidos();
                int nSF = eliminatorias[3].getNumPartidos();
                Equipo* perd[2]; int np = 0;
                for (int i = 0; i < nSF && np < 2; i++) {
                    contarIteracion();
                    if (!psSF[i].getEquipo1() || !psSF[i].isSimulado()) continue;
                    int g1 = psSF[i].getStatsEquipo1()->getGoles();
                    int g2 = psSF[i].getStatsEquipo2()->getGoles();
                    perd[np++] = (g1 >= g2) ? psSF[i].getEquipo2() : psSF[i].getEquipo1();
                }
                if (np == 2) eliminatorias[4].agregarPartido(perd[0], perd[1], "10/07/2026");

            } else if (e == 5) {
                Equipo** clasSF = eliminatorias[3].getClasificados();
                int nSF = eliminatorias[3].getNumClasificados();
                if (nSF >= 2) {
                    eliminatorias[5].agregarPartido(clasSF[0], clasSF[1], "10/07/2026");
                }

            } else {
                Equipo** clasAnt = eliminatorias[e - 1].getClasificados();
                int n = eliminatorias[e - 1].getNumClasificados();
                cout << "[DEBUG] " << etapas[e] << ": " << n << " clasificados de etapa anterior." << endl;
                for (int i = 0; i + 1 < n; i += 2) {
                    contarIteracion();
                    eliminatorias[e].agregarPartido(clasAnt[i], clasAnt[i + 1], "10/07/2026");
                }
            }
        }
        eliminatorias[e].simularEtapa();
        eliminatorias[e].mostrar();
    }
    medirEficiencia();
}

void UdeAWorldCup::mostrarEstadisticas() {
    cout << "\n ESTADISTICAS FINALES DEL TORNEO " << endl;

    Equipo* campeon = nullptr;
    if (eliminatorias[5].getNumClasificados() > 0) {
        campeon = eliminatorias[5].getClasificados()[0];
    }
    if (!campeon) {
        cout << "(No hay simulacion completada aun)" << endl;
        return;
    }

    cout << "\n1. CAMPEON DEL MUNDO: " << campeon->getNombre() << endl;

    cout << "\n2. TOP 4 DEL MUNDIAL:" << endl;
    cout << "   1ro: " << campeon->getNombre() << endl;

    if (eliminatorias[5].getNumPartidos() > 0) {
        Partido* pFinal = &eliminatorias[5].getPartidos()[0];
        if (pFinal && pFinal->isSimulado()) {
            int g1 = pFinal->getStatsEquipo1()->getGoles();
            int g2 = pFinal->getStatsEquipo2()->getGoles();
            Equipo* sub = (g1 >= g2) ? pFinal->getEquipo2() : pFinal->getEquipo1();
            cout << "   2do: " << sub->getNombre() << endl;
        }
    }

    if (eliminatorias[4].getNumPartidos() > 0) {
        Partido* p3 = &eliminatorias[4].getPartidos()[0];
        if (p3 && p3->isSimulado()) {
            int g1 = p3->getStatsEquipo1()->getGoles();
            int g2 = p3->getStatsEquipo2()->getGoles();
            Equipo* ter  = (g1 >= g2) ? p3->getEquipo1() : p3->getEquipo2();
            Equipo* cuar = (g1 >= g2) ? p3->getEquipo2() : p3->getEquipo1();
            cout << "   3ro: " << ter->getNombre()  << endl;
            cout << "   4to: " << cuar->getNombre() << endl;
        }
    }
    cout << "\n3. MAXIMO GOLEADOR DEL CAMPEON (" << campeon->getNombre() << "):" << endl;
    int maxGoles = 0;
    Jugador* maxGoleador = nullptr;
    for (int j = 0; j < MAX_JUGADORES; j++) {
        Jugador* jug = campeon->getJugador(j);
        if (!jug) continue;
        EstadisticasJugador* ej = jug->getEstadisticas();
        if (ej && ej->getGoles() > maxGoles) {
            maxGoles    = ej->getGoles();
            maxGoleador = jug;
        }
    }
    if (maxGoleador) {
        cout << "   " << maxGoleador->getNombre() << " " << maxGoleador->getApellido()
        << " #" << maxGoleador->getnumeroCamisa() << " con " << maxGoles << " goles" << endl;
    } else {
        cout << "   (sin goles registrados)" << endl;
    }

    cout << "\n4. TOP 3 GOLEADORES DEL TORNEO:" << endl;
    Jugador* top3[3] = {nullptr, nullptr, nullptr};
    int top3Goles[3] = {-1, -1, -1};

    for (int i = 0; i < totalEquipos; i++) {
        contarIteracion();
        for (int j = 0; j < MAX_JUGADORES; j++) {
            contarIteracion();
            Jugador* jug = equipos[i].getJugador(j);
            int g = jug->getEstadisticas()->getGoles();

            for (int k = 0; k < 3; k++) {
                if (g > top3Goles[k]) {

                    for (int m = 2; m > k; m--) {
                        top3[m] = top3[m-1]; top3Goles[m] = top3Goles[m-1];
                    }
                    top3[k] = jug; top3Goles[k] = g;
                    break;
                }
            }
        }
    }
    for (int k = 0; k < 3; k++) {
        if (top3[k]) {
            cout << "   " << (k+1) << ". " << top3[k]->getNombre()
            << " " << top3[k]->getApellido()
            << " - " << top3Goles[k] << " goles" << endl;
        }
    }

    cout << "\n5. EQUIPO CON MAS GOLES HISTORICOS:" << endl;
    int maxGF = -1; Equipo* mejorEquipo = nullptr;
    for (int i = 0; i < totalEquipos; i++) {
        contarIteracion();
        int gf = equipos[i].getEstadisticas()->getGolesFavor();
        if (gf > maxGF) { maxGF = gf; mejorEquipo = &equipos[i]; }
    }
    if (mejorEquipo) {
        cout << "   " << mejorEquipo->getNombre() << " con " << maxGF << " goles" << endl;
    }

    const char* etapasMostrar[] = {"R16", "R8", "SF"};
    int etapaIdx[] = {0, 1, 3};
    cout << "\n6. CONFEDERACION DOMINANTE POR ETAPA:" << endl;
    for (int e = 0; e < 3; e++) {
        contarIteracion();
        Equipo** cls = eliminatorias[etapaIdx[e]].getClasificados();
        int n = eliminatorias[etapaIdx[e]].getNumClasificados();
        const char* confs[] = {"UEFA","CONMEBOL","CONCACAF","CAF","AFC","OFC"};
        int conteos[6] = {0};
        for (int k = 0; k < n; k++) {
            contarIteracion();
            if (!cls[k]) continue;
            for (int c = 0; c < 6; c++) {
                if (strcmp(cls[k]->getConfederacion(), confs[c]) == 0) {
                    conteos[c]++; break;
                }
            }
        }
        int maxC = 0; int idxC = 0;
        for (int c = 0; c < 6; c++) { if (conteos[c] > maxC) { maxC = conteos[c]; idxC = c; } }
        cout << "   " << etapasMostrar[e] << ": " << confs[idxC]
             << " (" << maxC << " equipos)" << endl;
    }
    for (int i = 0; i < totalEquipos; i++) {
        equipos[i].guardarHistorico();
    }

    cout << "[OK] Archivos históricos generados\n";
}

long long UdeAWorldCup::calcularMemoria() const {
    long long mem = 0;
    // Equipos
    mem += sizeof(Equipo) * MAX_EQUIPOS;
    mem += sizeof(Jugador) * MAX_JUGADORES * totalEquipos;
    mem += sizeof(EstadisticasJugador) * MAX_JUGADORES * totalEquipos;
    mem += sizeof(EstadisticasEquipo) * totalEquipos;
    // Grupos
    mem += sizeof(Grupo) * NUM_GRUPOS;
    mem += sizeof(Partido) * PARTIDOS_POR_GRUPO * NUM_GRUPOS;
    // Eliminatorias
    mem += sizeof(Eliminatorias) * 6;
    mem += sizeof(Partido) * (32 + 16 + 8 + 4 + 2 + 1);
    return mem;
}


void UdeAWorldCup::contarIteracion(int n) {
    contadorIteraciones += n;
}

void UdeAWorldCup::medirEficiencia() {
    memoriaConsumida = calcularMemoria();
    cout << "\n[EFICIENCIA]"
         << " Iteraciones=" << contadorIteraciones
         << " | Memoria~=" << memoriaConsumida << " bytes"
         << " (" << memoriaConsumida / 1024 << " KB)" << endl;
}