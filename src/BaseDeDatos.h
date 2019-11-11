#ifndef TP3_BASEDEDATOS_H
#define TP3_BASEDEDATOS_H

#include <string>

#include "Tipos.h"
#include "Registro.h"
#include "Tabla.h"
#include "Consulta.h"
#include "modulos_basicos/string_map.h"
#include "modulos_basicos/linear_set.h"

using namespace std;

class BaseDeDatos {
public:
    BaseDeDatos();
    void agregarTabla(NombreTabla& tabla, NombreCampo& clave, linear_set<NombreCampo>& campos);
    void agregarRegistro(Registro& registro, NombreTabla& tabla);
    const string_map<Tabla>& tablas();
    void eliminarTabla(NombreTabla& tabla);
    void eliminarRegistro(Valor &valor, NombreTabla& tabla);
    linear_set<Registro> realizarConsulta(Consulta& consulta);

private:
    //ESTRUCTURA

    //AUXILIARES

};

#endif //TP3_BASEDEDATOS_H
