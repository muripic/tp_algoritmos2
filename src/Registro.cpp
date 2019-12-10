#include "Registro.h"

Registro::Registro() : _registro() {
}

const linear_set<NombreCampo>& Registro::campos() const {
    return _registro.claves();
}

void Registro::definir(const NombreCampo& campo, const Valor& valor) {
    _registro.insert(make_pair(campo, valor));
}

Valor& Registro::operator[](const NombreCampo& campo) {
   return _registro[campo];
}

const Valor &Registro::operator[](const NombreCampo &campo) const {
    return _registro.at(campo);
}

bool Registro::operator==(const Registro& registro) const{
    bool res = true;
    for (const NombreCampo& c1 : campos()){
        if (_registro.at(c1) != registro[c1]){
            res = false;
        }
    }
    return res;
}

void Registro::borrarCampo(const NombreCampo& campo){
    _registro.erase(campo);
}
