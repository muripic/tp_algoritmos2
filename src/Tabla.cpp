#include "Tabla.h"

Tabla::Tabla(linear_set<NombreCampo>& campos, NombreCampo& clave) : _clave(clave), _registros(), _diccValorClave(), _diccColumnas() {
    linear_set<NombreCampo>::iterator it = campos.begin();
    while (it != campos.end()){
        linear_map<linear_set<Registro>::iterator, Valor> nuevo;
        pair<NombreCampo, linear_map<linear_set<Registro>::iterator, Valor>> entrada;
        entrada.first = *it;
        entrada.second = nuevo;
        _diccColumnas.insert(entrada);
        ++it;
    }
}

void Tabla::insertar(Registro &registro){
    //COMPLETAR
}

void Tabla::borrar(Valor &valor){
    //COMPLETAR
}

linear_set<NombreCampo>& Tabla::campos() const{
    //COMPLETAR
}

NombreCampo& Tabla::clave() const{
    //COMPLETAR
}

linear_set<Registro>& Tabla::registros() const{
    //COMPLETAR
}

bool Tabla::existeRegConClave(Valor &valor){
    //COMPLETAR
}

Registro& Tabla::regPorClave(Valor &valor) const{
    //COMPLETAR
}

linear_set<Valor>& Tabla::valoresClave() const{
    //COMPLETAR
}

linear_map<linear_set<Registro>::iterator, Valor>& Tabla::obtenerColumna(NombreCampo &campo) const{
    //COMPLETAR
}