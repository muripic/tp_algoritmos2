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
    Tabla(linear_set<NombreCampo>& campos, const NombreCampo& clave);
    void insertar(Registro &registro);
    void borrar(const Valor &valor);
    const linear_set<NombreCampo>& campos();
    const NombreCampo& clave();
    const linear_set<Registro>& registros();
    bool existeRegConClave(const Valor& valor);
    const Registro& regPorClave(const Valor& valor);
    const linear_set<Valor>& valoresClave();
    const linear_map<linear_set<Registro>::iterator, Valor>& obtenerColumna(const NombreCampo& campo);

private:
    NombreCampo _clave;
    linear_set<Registro> _registros;
    string_map<linear_set<Registro>::iterator> _diccValorClave;
    string_map<linear_map<linear_set<Registro>::iterator, Valor>> _diccColumnas;
};


#endif //TP3_TABLA_H
