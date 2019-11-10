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
    return _diccColumnas.claves();
}

NombreCampo& Tabla::clave() const{
    return _clave;
}

linear_set<Registro>& Tabla::registros() const{
    return _registros;
}

bool Tabla::existeRegConClave(Valor &valor){
    if (_diccValorClave.count(valor) == 1){
        return true;
    } else{
        return false;
    }

}

Registro& Tabla::regPorClave(Valor &valor) const{
    return _diccValorClave.at(valor);
}

linear_set<Valor>& Tabla::valoresClave() const{
    return _diccValorClave.claves();
}

linear_map<linear_set<Registro>::iterator, Valor>& Tabla::obtenerColumna(NombreCampo &campo) const{
    // COMPLETAR
}