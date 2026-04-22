#ifndef ESTADISTICASPARTIDO_H
#define ESTADISTICASPARTIDO_H

class EstadisticasPartido {
private:
    int goles;
    int amarillas;
    int rojas;
    int faltas;
    float posesion;

public:
    EstadisticasPartido();
    EstadisticasPartido(int goles, int amarillas, int rojas, int faltas, float posesion);
    EstadisticasPartido(const EstadisticasPartido& otro);


    int getGoles() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;
    float getPosesion() const;

    void setGoles(int g);
    void setAmarillas(int a);
    void setRojas(int r);
    void setFaltas(int f);
    void setPosesion(float p);

    bool operator>(const EstadisticasPartido& otro) const; // Comparar por goles

    void mostrar() const;
};

#endif //ESTADISTICASPARTIDO_H
