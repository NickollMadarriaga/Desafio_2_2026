#ifndef JUGADOR_H
#define JUGADOR_H
#include <fstream>

class Jugador
{
private:
    char nombre[50];
    char apellido[50];
    int numeroCamisa;
    EstadisticasJugador* estadisticas;

public:
    Jugador();
    Jugador(const char* nombre, const char* apellido, int numero, const EstadisticasJugador& stats);
    Jugador(const Jugador& otro); // Constructor de copia
    ~Jugador();

    const char* getNombre() const;
    const char* getApellido() const;
    int getNumero() const;
    EstadisticasJugador* getEstadisticas() const;

    void setNombre(const char* n);
    void setApellido(const char* a);
    void setNumero(int n);

    void actualizarEstadisticas(int golesPartido, int minutosPartido,
                                int faltasPartido, int amarillasPartido, int rojasPartido);

    void guardarEnArchivo(ofstream &out) const {
        out.write(nombre, sizeof(nombre));
        out.write(apellido, sizeof(apellido));
        out.write((char*)&numeroCamisa, sizeof(numeroCamisa));

        estadisticas->guardarEnArchivo(out);
    }

    void cargarDesdeArchivo(ifstream &in) {
        in.read(nombre, sizeof(nombre));
        in.read(apellido, sizeof(apellido));
        in.read((char*)&numeroCamisa, sizeof(numeroCamisa));

        if (estadisticas == nullptr) {
            estadisticas = new EstadisticasJugador();
        }

        estadisticas->cargarDesdeArchivo(in);
    }

    bool operator==(const Jugador& otro) const;
    Jugador& operator=(const Jugador& otro);

    void mostrar() const;
};


#endif // JUGADOR_H
