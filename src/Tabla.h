#ifndef TP3_TABLA_H
#define TP3_TABLA_H

#include <string>
#include <fstream>
#include <sstream>

#include "Tipos.h"
#include "Registro.h"
#include "modulos_basicos/string_map.h"
#include "modulos_basicos/linear_set.h"

using namespace std;

class Tabla {
public:
    Tabla(const linear_set<NombreCampo>& campos, const NombreCampo& clave);
    void insertar(Registro &registro);
    void borrar(const Valor &valor);
    const linear_set<NombreCampo>& campos() const;
    const NombreCampo& clave() const;
    const linear_set<Registro>& registros() const;
    bool existeRegConClave(const Valor& valor) const;
    const Registro& regPorClave(const Valor& valor) const;
    const linear_set<Valor>& valoresClave() const;
    const linear_map<linear_set<Registro>::iterator, Valor>& obtenerColumna(const NombreCampo& campo) const;

    void cargarRegistros(string dataset);

private:
    NombreCampo _clave;
    linear_set<Registro> _registros;
    string_map<linear_set<Registro>::iterator> _diccValorClave;
    string_map<linear_map<linear_set<Registro>::iterator, Valor>> _diccColumnas;

    bool _leerLinea(ifstream& is, vector<string>& valores) const;

};


#endif //TP3_TABLA_H
