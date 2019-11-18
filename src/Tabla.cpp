#include "Tabla.h"

Tabla::Tabla(const linear_set<NombreCampo> &campos, const NombreCampo &clave) : _clave(clave), _registros(),
                                                                          _diccValorClave(), _diccColumnas() {
    linear_set<NombreCampo>::const_iterator it = campos.begin();
    while (it != campos.end()) {
        linear_map<linear_set<Registro>::iterator, Valor> nuevo;
        _diccColumnas.insert(make_pair(*it, nuevo));
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
    _diccValorClave.insert(make_pair(vc, itNuevoReg));
    linear_set<NombreCampo>::const_iterator itCampos = registro.campos().begin();
    while (itCampos != registro.campos().end()) {
        _diccColumnas.at(*itCampos).fast_insert(make_pair(itNuevoReg, registro[*itCampos]));
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

const linear_set<NombreCampo> &Tabla::campos() const {
    return _diccColumnas.claves();
}

const NombreCampo &Tabla::clave() const {
    return _clave;
}

const linear_set<Registro> &Tabla::registros() const {
    return _registros;
}

bool Tabla::existeRegConClave(const Valor &valor) const {
    return _diccValorClave.count(valor) == 1;

}

const Registro &Tabla::regPorClave(const Valor &valor) const {
    return *_diccValorClave.at(valor);
}

const linear_set<Valor> &Tabla::valoresClave() const {
    return _diccValorClave.claves();
}

const linear_map<linear_set<Registro>::iterator, Valor> &Tabla::obtenerColumna(const NombreCampo &campo) const {
    return _diccColumnas.at(campo);
}

void Tabla::cargarRegistros(string dataset) {
    ifstream is("datos/" + dataset + ".txt");

    vector<string> campos;

    _leerLinea(is, campos);
    int linea = 1;
    while (!is.eof()) {
        vector<string> valores;
        if (!_leerLinea(is, valores)) {
            break;
        }
        if (campos.size() != valores.size()) {
            cerr << "Error leyendo dataset: " << dataset << endl;
            cerr << "El registro en la linea " << linea
                 << " tiene " << valores.size() << " campos"
                 << " pero la tabla declara " << campos.size() << " campos."
                 << endl;
            exit(1);
        }
        Registro r;
        for (size_t i = 0; i < campos.size(); i++) {
            r[campos[i]] = valores[i];
        }
        insertar(r);
    }
}

bool Tabla::_leerLinea(ifstream& is, vector<string>& valores) const {
    string linea;
    if (!getline(is, linea)) {
        return false;
    }

    istringstream isl(linea);
    string valor;
    while (getline(isl, valor, ',')) {
        valores.push_back(valor);
    }
    return true;
}