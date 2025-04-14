#ifndef BANCO_PREGUNTAS_H
#define BANCO_PREGUNTAS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

string nivelToString(int nivel);

class Pregunta {
protected:
    string enunciado;
    int nivel;
    int tiempoEstimado;
    string solucionEsperada;
    int anioCreacion;
public:
    Pregunta(string e, int n, int t, string s, int a);
    virtual ~Pregunta();
    virtual void mostrar() const;
    int getNivel() const;
    int getTiempo() const;
    int getAnio() const;
    string getEnunciado() const;
    void setEnunciado(string e);
    void setSolucion(string s);
    void setTiempo(int t);
    void setNivel(int n);
};

class PreguntaMultiple : public Pregunta {
    vector<string> opciones;
public:
    PreguntaMultiple(string e, int n, int t, string s, int a, vector<string> o);
    void mostrar() const override;
    void setOpciones(vector<string> nuevasOpciones);
};

class PreguntaVF : public Pregunta {
    bool respuestaCorrecta;
public:
    PreguntaVF(string e, int n, int t, string s, int a, bool r);
    void mostrar() const override;
    void setRespuesta(bool r);
};

class BancoPreguntas {
    vector<Pregunta*> preguntas;
public:
    BancoPreguntas();
    ~BancoPreguntas();
    void agregar(Pregunta* p);
    void mostrar() const;
    void buscarPorNivel(int nivel) const;
    void eliminarPorEnunciado(const string& texto);
    void actualizarPregunta(const string& texto);
    void generarEvaluacion(int cantidad, int nivel, int anioActual) const;
    int tiempoTotalEvaluacion() const;
};

void menu();

#endif
