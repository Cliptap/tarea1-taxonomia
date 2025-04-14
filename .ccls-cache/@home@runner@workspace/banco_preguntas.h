// banco_preguntas.h
#ifndef BANCO_PREGUNTAS_H
#define BANCO_PREGUNTAS_H

#include <iostream>
#include <vector>
#include <string>

enum class NivelTaxonomico {
    Recordar = 1, Entender, Aplicar, Analizar, Evaluar, Crear
};

class Pregunta {
protected:
    std::string enunciado;
    NivelTaxonomico nivel;
    int tiempoEstimado;
    std::string solucionEsperada;
    int anioCreacion;
public:
    Pregunta(std::string e, NivelTaxonomico n, int t, std::string s, int a);
    virtual ~Pregunta();
    virtual void mostrar() const = 0;
    NivelTaxonomico getNivel() const;
    int getTiempo() const;
    int getAnio() const;
    std::string getEnunciado() const;
    void setEnunciado(std::string e);
    void setSolucion(std::string s);
};

class PreguntaMultiple : public Pregunta {
    std::vector<std::string> opciones;
public:
    PreguntaMultiple(std::string e, NivelTaxonomico n, int t, std::string s, int a, std::vector<std::string> o);
    void mostrar() const override;
};

class PreguntaVF : public Pregunta {
    bool respuestaCorrecta;
public:
    PreguntaVF(std::string e, NivelTaxonomico n, int t, std::string s, int a, bool r);
    void mostrar() const override;
};

class BancoPreguntas {
    std::vector<Pregunta*> preguntas;
public:
    BancoPreguntas();
    ~BancoPreguntas();
    void agregar(Pregunta* p);
    void mostrar() const;
    void buscarPorNivel(NivelTaxonomico nivel) const;
    void eliminarPorEnunciado(const std::string& texto);
    int tiempoTotalEvaluacion() const;
};

void menu();

#endif // BANCO_PREGUNTAS_H