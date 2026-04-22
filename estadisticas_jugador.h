#ifndef ESTADISTICASJUGADOR_H
#define ESTADISTICASJUGADOR_H

class EstadisticasJugador {
private:
    int partidos;
    int goles;
    int minutos;
    int faltas;
    int amarillas;
    int rojas;
    int asistencias;

public:
    EstadisticasJugador();

    EstadisticasJugador(int partidos, int goles, int minutos, int faltas,
                        int amarillas, int rojas, int asistencias);

    EstadisticasJugador(const EstadisticasJugador& otro);

    int getPartidos() const;
    int getGoles() const;
    int getMinutos() const;
    int getFaltas() const;
    int getAmarillas() const;
    int getRojas() const;
    int getAsistencias() const;

    void setPartidos(int p);
    void setGoles(int g);
    void setMinutos(int m);
    void setFaltas(int f);
    void setAmarillas(int a);
    void setRojas(int r);
    void setAsistencias(int a);

    void acumularPartido(int golesPartido, int minutosPartido, int faltasPartido,
                         int amarillasPartido, int rojasPartido);

    EstadisticasJugador operator+(const EstadisticasJugador& otro) const;
    EstadisticasJugador& operator+=(const EstadisticasJugador& otro);
    bool operator>(const EstadisticasJugador& otro) const; // Comparar por goles

    void mostrar() const;
};

#endif //ESTADISTICASJUGADOR_H
