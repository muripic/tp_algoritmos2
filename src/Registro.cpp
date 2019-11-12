#include "Registro.h"

Registro::Registro() : _registro() {
}

const linear_set<NombreCampo>& Registro::campos() const {
    return _registro.claves();
}

void Registro::definir(const NombreCampo& campo, const Valor& valor){
    pair<NombreCampo, Valor> entrada = make_pair(campo, valor);
    _registro.insert(entrada);
}

const Valor& Registro::operator[](const NombreCampo& campo) const {
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