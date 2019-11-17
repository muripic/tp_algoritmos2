#include "Tabla.h"

Tabla::Tabla(linear_set<NombreCampo> &campos, const NombreCampo &clave) : _clave(clave), _registros(),
                                                                          _diccValorClave(), _diccColumnas() {
    linear_set<NombreCampo>::iterator it = campos.begin();
    while (it != campos.end()) {
        linear_map<linear_set<Registro>::iterator, Valor> nuevo;
        pair<NombreCampo, linear_map<linear_set<Registro>::iterator, Valor>> entrada = make_pair(*it, nuevo);
        _diccColumnas.insert(entrada);
        ++it;
    }
}

void Tabla::insertar(Registro &registro) {
    Valor vc = registro[_clave];
    if (_diccValorClave.count(vc) == 1) {
        linear_set<Registro>::iterator itReg = _diccValorClave.at(vc);
        linear_set<NombreCampo>::const_iterator itCampos = registro.campos().begin();
        while (itCampos != registro.campos().end()) {
            _diccColumnas.at(*itCampos).erase(itReg);
            ++itCampos;
        }
        _diccValorClave.erase(vc);
        _registros.erase(*itReg);
    }
    linear_set<Registro>::iterator itNuevoReg = _registros.fast_insert(registro);
    pair<Valor, linear_set<Registro>::iterator> entradavc = make_pair(vc, itNuevoReg);
    _diccValorClave.insert(entradavc);
    linear_set<NombreCampo>::const_iterator itCampos = registro.campos().begin();
    while (itCampos != registro.campos().end()) {
        pair<linear_set<Registro>::iterator, Valor> entradacol = make_pair(itNuevoReg, registro[*itCampos]);
        _diccColumnas.at(*itCampos).insert(entradacol);
        ++itCampos;
    }
}

void Tabla::borrar(const Valor &valor) {
    linear_set<Registro>::iterator itReg = _diccValorClave.at(valor);
    linear_set<NombreCampo>::const_iterator itCampos = campos().begin();
    while (itCampos != campos().end()) {
        (_diccColumnas.at(*itCampos)).erase(itReg);
        ++itCampos;
    }
    _diccValorClave.erase(valor);
    _registros.erase(*itReg); /*EliminarSiguiente(itReg). PROBLEMA: en linear map no está sobrecargado el metodo erase
 * para que se le pueda pasar un iterador (como sí lo está en map de la STL).
 * Igual, cambia la complejidad de la operación pero no del algoritmo*/
}

const linear_set<NombreCampo> &Tabla::campos() {
    return _diccColumnas.claves();
}

const NombreCampo &Tabla::clave() {
    return _clave;
}

const linear_set<Registro> &Tabla::registros() {
    return _registros;
}

bool Tabla::existeRegConClave(const Valor &valor) {
    return _diccValorClave.count(valor) == 1;

}

const Registro &Tabla::regPorClave(const Valor &valor) {
    return *_diccValorClave.at(valor);
}

const linear_set<Valor> &Tabla::valoresClave() {
    return _diccValorClave.claves();
}

const linear_map<linear_set<Registro>::iterator, Valor> &Tabla::obtenerColumna(const NombreCampo &campo) {
    return _diccColumnas.at(campo);
}