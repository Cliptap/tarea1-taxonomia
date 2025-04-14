#include "banco_preguntas.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string nivelToString(int nivel) {
    switch(nivel) {
        case 1: return "Recordar";
        case 2: return "Entender";
        case 3: return "Aplicar";
        case 4: return "Analizar";
        case 5: return "Evaluar";
        case 6: return "Crear";
        default: return "Desconocido";
    }
}

// --------------------- PREGUNTA -----------------------
Pregunta::Pregunta(string e, int n, int t, string s, int a) {
    this->enunciado = e;
    this->nivel = n;
    this->tiempoEstimado = t;
    this->solucionEsperada = s;
    this->anioCreacion = a;
}
Pregunta::~Pregunta() {}
void Pregunta::mostrar() const {
    cout << "[GENERICA] " << this->enunciado << " (" << nivelToString(this->nivel) << ", " << this->tiempoEstimado << " min)" << endl;
}
int Pregunta::getNivel() const { return this->nivel; }
int Pregunta::getTiempo() const { return this->tiempoEstimado; }
int Pregunta::getAnio() const { return this->anioCreacion; }
string Pregunta::getEnunciado() const { return this->enunciado; }
void Pregunta::setEnunciado(string e) { this->enunciado = e; }
void Pregunta::setSolucion(string s) { this->solucionEsperada = s; }
void Pregunta::setTiempo(int t) { this->tiempoEstimado = t; }
void Pregunta::setNivel(int n) { this->nivel = n; }

// --------------------- MULTIPLE ---------------------------
PreguntaMultiple::PreguntaMultiple(string e, int n, int t, string s, int a, vector<string> o)
    : Pregunta(e, n, t, s, a), opciones(o) {}

void PreguntaMultiple::mostrar() const {
    cout << "[MULTIPLE] " << this->enunciado << " (" << nivelToString(this->nivel) << ", " << this->tiempoEstimado << " min)" << endl;
    for (size_t i = 0; i < this->opciones.size(); ++i) {
        cout << (char)('A' + i) << ") " << this->opciones[i] << endl;
    }
}
void PreguntaMultiple::setOpciones(vector<string> nuevasOpciones) {
    this->opciones = nuevasOpciones;
}

// --------------------- VERDADERO/FALSO ---------------------
PreguntaVF::PreguntaVF(string e, int n, int t, string s, int a, bool r)
    : Pregunta(e, n, t, s, a), respuestaCorrecta(r) {}

void PreguntaVF::mostrar() const {
    cout << "[VF] " << this->enunciado << " (" << nivelToString(this->nivel) << ", " << this->tiempoEstimado << " min)" << endl;
    cout << "¿Verdadero o Falso?" << endl;
    cout << "Respuesta correcta: " << (this->respuestaCorrecta ? "Verdadero" : "Falso") << endl;
}
void PreguntaVF::setRespuesta(bool r) {
    this->respuestaCorrecta = r;
}

// --------------------- BANCO -------------------------------
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
void BancoPreguntas::buscarPorNivel(int nivel) const {
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
void BancoPreguntas::actualizarPregunta(const string& texto) {
    for (size_t i = 0; i < preguntas.size(); ++i) {
        if (preguntas[i]->getEnunciado() == texto) {
            string nuevoEnunciado, nuevaSolucion;
            int nuevoNivel, nuevoTiempo;
            cout << "Nuevo enunciado: ";
            getline(cin >> ws, nuevoEnunciado);
            cout << "Nuevo nivel (1-6): ";
            cin >> nuevoNivel;
            cout << "Nuevo tiempo estimado: ";
            cin >> nuevoTiempo;
            cin.ignore();
            cout << "Nueva solución esperada: ";
            getline(cin, nuevaSolucion);

            preguntas[i]->setEnunciado(nuevoEnunciado);
            preguntas[i]->setNivel(nuevoNivel);
            preguntas[i]->setTiempo(nuevoTiempo);
            preguntas[i]->setSolucion(nuevaSolucion);

            PreguntaMultiple* pm = dynamic_cast<PreguntaMultiple*>(preguntas[i]);
            if (pm) {
                vector<string> nuevasOpciones;
                for (int j = 0; j < 4; j++) {
                    string op;
                    cout << "Nueva opción " << (char)('A' + j) << ": ";
                    getline(cin >> ws, op);
                    nuevasOpciones.push_back(op);
                }
                pm->setOpciones(nuevasOpciones);
            }

            PreguntaVF* pvf = dynamic_cast<PreguntaVF*>(preguntas[i]);
            if (pvf) {
                bool nuevaRespuesta;
                cout << "Nueva respuesta (1=Verdadero, 0=Falso): ";
                cin >> nuevaRespuesta;
                pvf->setRespuesta(nuevaRespuesta);
            }

            cout << "Pregunta actualizada correctamente.\n";
            return;
        }
    }
    cout << "Pregunta no encontrada.\n";
}

void BancoPreguntas::generarEvaluacion(int cantidad, int nivel, int anioActual) const {
    int seleccionadas = 0, tiempoTotal = 0;
    cout << "\nEVALUACION GENERADA:\n";
    for (size_t i = 0; i < preguntas.size() && seleccionadas < cantidad; ++i) {
        if (preguntas[i]->getNivel() == nivel && preguntas[i]->getAnio() != anioActual - 1) {
            preguntas[i]->mostrar();
            cout << "--------------------\n";
            tiempoTotal += preguntas[i]->getTiempo();
            seleccionadas++;
        }
    }

    if (seleccionadas < cantidad) {
        cout << "Advertencia: solo se pudieron seleccionar " << seleccionadas << " preguntas válidas.\n";
    }

    cout << "Tiempo estimado total: " << tiempoTotal << " minutos.\n";
}
int BancoPreguntas::tiempoTotalEvaluacion() const {
    int total = 0;
    for (size_t i = 0; i < preguntas.size(); ++i) {
        total += preguntas[i]->getTiempo();
    }
    return total;
}

// --------------------- MENU -------------------------------
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int leerEntero(const string& mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "Error: Ingrese un número válido.\n";
        limpiarBuffer();
        cout << mensaje;
    }
    limpiarBuffer();
    return valor;
}

void menu() {
    BancoPreguntas banco;
    int opcion;
    do {
        cout << "\n====== MENU PRINCIPAL ======\n";
        cout << "1. Agregar pregunta\n";
        cout << "2. Mostrar todas las preguntas\n";
        cout << "3. Buscar preguntas por nivel taxonómico\n";
        cout << "4. Eliminar pregunta por enunciado\n";
        cout << "5. Actualizar una pregunta\n";
        cout << "6. Generar evaluación (según cantidad y nivel)\n";
        cout << "7. Calcular tiempo estimado total de todas las preguntas\n";
        cout << "0. Salir\n";
        
        opcion = leerEntero("Seleccione una opción: ");

        if (opcion == 1) {
            try {
                int tipo = leerEntero("Tipo (1=Multiple, 2=VF): ");
                if (tipo != 1 && tipo != 2) {
                    cout << "Error: Tipo de pregunta inválido.\n";
                    continue;
                }

                int nivel = leerEntero("Nivel (1=Recordar,2=Entender,3=Aplicar,4=Analizar,5=Evaluar,6=Crear): ");
                if (nivel < 1 || nivel > 6) {
                    cout << "Error: Nivel inválido.\n";
                    continue;
                }

                int tiempo = leerEntero("Tiempo estimado (minutos): ");
                if (tiempo <= 0) {
                    cout << "Error: El tiempo debe ser positivo.\n";
                    continue;
                }

                string enunciado, solucion;
                cout << "Enunciado: "; getline(cin, enunciado);
                if (enunciado.empty()) {
                    cout << "Error: El enunciado no puede estar vacío.\n";
                    continue;
                }

                cout << "Solución esperada: "; getline(cin, solucion);
                if (solucion.empty()) {
                    cout << "Error: La solución no puede estar vacía.\n";
                    continue;
                }

                int anio = leerEntero("Año de creación: ");
                if (anio < 1900 || anio > 2100) {
                    cout << "Error: Año inválido.\n";
                    continue;
                }

            if (tipo == 1) {
                vector<string> opciones;
                for (int i = 0; i < 4; i++) {
                    string op;
                    cout << "Opción " << (char)('A' + i) << ": ";
                    getline(cin >> ws, op);
                    opciones.push_back(op);
                }
                banco.agregar(new PreguntaMultiple(enunciado, nivel, tiempo, solucion, anio, opciones));
            } else {
                int respuestaTemp;
                cout << "Respuesta (1=Verdadero, 0=Falso): "; 
                cin >> respuestaTemp;
                cin.ignore();
                banco.agregar(new PreguntaVF(enunciado, nivel, tiempo, solucion, anio, respuestaTemp == 1));
            }
        } else if (opcion == 2) {
            banco.mostrar();
        } else if (opcion == 3) {
            int n = leerEntero("Nivel a buscar (1 a 6): ");
            if (n < 1 || n > 6) {
                cout << "Error: Nivel inválido.\n";
                continue;
            }
            banco.buscarPorNivel(n);
        } else if (opcion == 4) {
            string texto;
            cout << "Enunciado exacto a eliminar: ";
            getline(cin >> ws, texto);
            banco.eliminarPorEnunciado(texto);
        } else if (opcion == 5) {
            string texto;
            cout << "Enunciado exacto de la pregunta a actualizar: ";
            getline(cin >> ws, texto);
            banco.actualizarPregunta(texto);
        } else if (opcion == 6) {
            int anioActual = leerEntero("Año actual de evaluación: ");
            if (anioActual < 1900 || anioActual > 2100) {
                cout << "Error: Año inválido.\n";
                continue;
            }

            int cantidad = leerEntero("Cantidad de preguntas: ");
            if (cantidad <= 0) {
                cout << "Error: La cantidad debe ser positiva.\n";
                continue;
            }

            int nivel = leerEntero("Nivel deseado (1 a 6): ");
            if (nivel < 1 || nivel > 6) {
                cout << "Error: Nivel inválido.\n";
                continue;
            }
            
            banco.generarEvaluacion(cantidad, nivel, anioActual);
        } else if (opcion == 7) {
            cout << "Tiempo estimado total: " << banco.tiempoTotalEvaluacion() << " minutos.\n";
        }

    } while (opcion != 0);
}