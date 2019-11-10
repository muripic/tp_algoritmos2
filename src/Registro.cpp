#include "Registro.h"

Registro::Registro() : _registro() {
}

linear_set<NombreCampo> Registro::campos() const {
    return _registro.claves();
}

void Registro::definir(const NombreCampo& campo, const Valor& valor){
    pair<NombreCampo, Valor> entrada = make_pair(campo, valor);
    _registro.insert(entrada);
}

Valor& Registro::operator[](const NombreCampo& campo) {
    return _registro.at(campo);
}