#ifndef ESTADISTICASEQUIPO_H
#define ESTADISTICASEQUIPO_H

class EstadisticasEquipo {
private:
    int golesFavor;
    int golesContra;
    int ganados;
    int empatados;
    int perdidos;
    int amarillas;
    int rojas;
    int faltas;

public:
    EstadisticasEquipo();
    EstadisticasEquipo(int gf, int gc, int ganados, int empatados, int perdidos,
                       int amarillas, int rojas, int faltas);
    EstadisticasEquipo(const EstadisticasEquipo& otro);

    int getGolesFavor() const;
    int getGolesContra() const;
    int getGanados() const;
    int getEmpatados() const;
    int getPerdidos() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;
    int getDiferenciaGoles() const;
    int getPartidosJugados() const;
    float getPromedioGolesFavor() const;
    float getPromedioGolesContra() const;

    void setGolesFavor(int gf);
    void setGolesContra(int gc);
    void setGanados(int g);
    void setEmpatados(int e);
    void setPerdidos(int p);
    void setAmarillas(int a);
    void setRojas(int r);
    void setFaltas(int f);

    void actualizarConPartido(int gf, int gc, int amarillas, int rojas, int faltas);

    EstadisticasEquipo& operator+=(const EstadisticasEquipo& otro);
    bool operator>(const EstadisticasEquipo& otro) const; // Comparar por goles a favor

    void mostrar() const;
};

#endif //ESTADISTICASEQUIPO_H
