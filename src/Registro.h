#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <string>

#include "Tipos.h"
#include "modulos_basicos/string_map.h"

using namespace std;

class Registro {
public:
    Registro();
    void definir(const NombreCampo& campo, const Valor& valor);
    const linear_set<NombreCampo>& campos() const;
    Valor& operator[](const NombreCampo& campo);
    const Valor& operator[](const NombreCampo& campo) const;
    bool operator==(const Registro& registro) const;
    void borrarCampo(const NombreCampo &campo);


private:
    string_map<Valor> _registro;

};

#endif /*__REGISTRO_H__*/