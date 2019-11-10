#ifndef TP3_TABLA_H
#define TP3_TABLA_H

#include <string>

#include "Tipos.h"
#include "Registro.h"
#include "modulos_basicos/string_map.h"
#include "modulos_basicos/linear_set.h"

using namespace std;

class Tabla {
public:
    Tabla(linear_set<NombreCampo>& campos, NombreCampo& clave);
    void insertar(Registro &registro);
    void borrar(Valor &valor);
    linear_set<NombreCampo>& campos() const;
    NombreCampo& clave() const;
    linear_set<Registro>& registros() const;
    bool existeRegConClave(Valor &valor);
    Registro& regPorClave(Valor &valor) const;
    linear_set<Valor>& valoresClave() const;
    linear_map<linear_set<Registro>::iterator, Valor>& obtenerColumna(NombreCampo &campo) const;

private:
    NombreCampo _clave;
    linear_set<Registro> _registros;
    string_map<linear_set<Registro>::iterator> _diccValorClave;
    string_map<linear_map<linear_set<Registro>::iterator, Valor>> _diccColumnas;
};


#endif //TP3_TABLA_H
