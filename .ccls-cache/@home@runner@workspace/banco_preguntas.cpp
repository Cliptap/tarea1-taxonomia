// banco_preguntas.cpp
#include "banco_preguntas.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string nivelToString(NivelTaxonomico nivel) {
    switch(nivel) {
        case NivelTaxonomico::Recordar: return "Recordar";
        case NivelTaxonomico::Entender: return "Entender";
        case NivelTaxonomico::Aplicar: return "Aplicar";
        case NivelTaxonomico::Analizar: return "Analizar";
        case NivelTaxonomico::Evaluar: return "Evaluar";
        case NivelTaxonomico::Crear: return "Crear";
        default: return "Desconocido";
    }
}

Pregunta::Pregunta(string e, NivelTaxonomico n, int t, string s, int a)
    : enunciado(e), nivel(n), tiempoEstimado(t), solucionEsperada(s), anioCreacion(a) {}

Pregunta::~Pregunta() {}

NivelTaxonomico Pregunta::getNivel() const {
    return this->nivel;
}

int Pregunta::getTiempo() const {
    return this->tiempoEstimado;
}

int Pregunta::getAnio() const {
    return this->anioCreacion;
}

string Pregunta::getEnunciado() const {
    return this->enunciado;
}

void Pregunta::setEnunciado(string e) {
    this->enunciado = e;
}

void Pregunta::setSolucion(string s) {
    this->solucionEsperada = s;
}

PreguntaMultiple::PreguntaMultiple(string e, NivelTaxonomico n, int t, string s, int a, vector<string> o)
    : Pregunta(e, n, t, s, a), opciones(o) {}

void PreguntaMultiple::mostrar() const {
    cout << "[MULTIPLE] " << this->enunciado << " (" << nivelToString(this->nivel) << ", " << this->tiempoEstimado << " min)" << endl;
    for (size_t i = 0; i < this->opciones.size(); ++i) {
        cout << (char)('A' + i) << ") " << this->opciones[i] << endl;
    }
}

PreguntaVF::PreguntaVF(string e, NivelTaxonomico n, int t, string s, int a, bool r)
    : Pregunta(e, n, t, s, a), respuestaCorrecta(r) {}

void PreguntaVF::mostrar() const {
    cout << "[VF] " << this->enunciado << " (" << nivelToString(this->nivel) << ", " << this->tiempoEstimado << " min)" << endl;
    cout << "¿Verdadero o Falso?" << endl;
    cout << "Respuesta correcta: " << (this->respuestaCorrecta ? "Verdadero" : "Falso") << endl;
}

BancoPreguntas::BancoPreguntas() {}

BancoPreguntas::~BancoPreguntas() {
    for (size_t i = 0; i < this->preguntas.size(); ++i) {
        delete this->preguntas[i];
    }
}

void BancoPreguntas::agregar(Pregunta* p) {
    this->preguntas.push_back(p);
}

void BancoPreguntas::mostrar() const {
    for (size_t i = 0; i < this->preguntas.size(); ++i) {
        this->preguntas[i]->mostrar();
        cout << "--------------------" << endl;
    }
}

void BancoPreguntas::buscarPorNivel(NivelTaxonomico nivel) const {
    for (size_t i = 0; i < this->preguntas.size(); ++i) {
        if (this->preguntas[i]->getNivel() == nivel) {
            this->preguntas[i]->mostrar();
            cout << "--------------------" << endl;
        }
    }
}

void BancoPreguntas::eliminarPorEnunciado(const string& texto) {
    for (auto it = this->preguntas.begin(); it != this->preguntas.end(); ) {
        if ((*it)->getEnunciado() == texto) {
            delete *it;
            it = this->preguntas.erase(it);
        } else {
            ++it;
        }
    }
}

int BancoPreguntas::tiempoTotalEvaluacion() const {
    int total = 0;
    for (size_t i = 0; i < this->preguntas.size(); ++i) {
        total = total + this->preguntas[i]->getTiempo();
    }
    return total;
}

void menu() {
    BancoPreguntas banco;
    int opcion;
    do {
        cout << "\n------ MENU ------\n";
        cout << "1. Agregar pregunta\n";
        cout << "2. Mostrar preguntas\n";
        cout << "3. Buscar por nivel taxonomico\n";
        cout << "4. Eliminar pregunta por enunciado\n";
        cout << "5. Tiempo estimado total\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            int tipo, nivel, tiempo, anio;
            string enunciado, solucion;
            cout << "Tipo (1=Multiple, 2=VF): "; cin >> tipo;
            cout << "Nivel (1=Recordar, ..., 6=Crear): "; cin >> nivel;
            cout << "Tiempo estimado: "; cin >> tiempo;
            cin.ignore();
            cout << "Enunciado: "; getline(cin, enunciado);
            cout << "Solucion esperada: "; getline(cin, solucion);
            cout << "Anio creacion: "; cin >> anio;

            if (tipo == 1) {
                vector<string> opciones;
                for (int i = 0; i < 4; i++) {
                    string op;
                    cout << "Opcion " << (char)('A' + i) << ": ";
                    getline(cin >> ws, op);
                    opciones.push_back(op);
                }
                banco.agregar(new PreguntaMultiple(enunciado, (NivelTaxonomico)nivel, tiempo, solucion, anio, opciones));
            } else {
                bool respuesta;
                cout << "Respuesta (1=Verdadero, 0=Falso): "; cin >> respuesta;
                banco.agregar(new PreguntaVF(enunciado, (NivelTaxonomico)nivel, tiempo, solucion, anio, respuesta));
            }
        }
        else if (opcion == 2) {
            banco.mostrar();
        }
        else if (opcion == 3) {
            int n;
            cout << "Nivel a buscar: "; cin >> n;
            banco.buscarPorNivel((NivelTaxonomico)n);
        }
        else if (opcion == 4) {
            string texto;
            cout << "Enunciado exacto a eliminar: ";
            getline(cin >> ws, texto);
            banco.eliminarPorEnunciado(texto);
        }
        else if (opcion == 5) {
            cout << "Tiempo estimado total: " << banco.tiempoTotalEvaluacion() << " minutos." << endl;
        }

    } while (opcion != 0);
}