#include "Registro.h"

Registro::Registro() : _registro() {
}

const linear_set<NombreCampo>& Registro::campos() {
    return _registro.claves();
}

void Registro::definir(const NombreCampo& campo, const Valor& valor){
    pair<NombreCampo, Valor> entrada = make_pair(campo, valor);
    _registro.insert(entrada);
}

Valor& Registro::operator[](const NombreCampo& campo) {
    return _registro.at(campo);
}