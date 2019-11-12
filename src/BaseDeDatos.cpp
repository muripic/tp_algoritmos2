#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() : _tablas(){
}

void BaseDeDatos::agregarTabla(NombreTabla& tabla, NombreCampo& clave, linear_set<NombreCampo>& campos){
    Tabla nueva(campos, clave);
    pair<string, Tabla> entrada = make_pair(tabla, nueva);
    _tablas.insert(entrada);
}

void BaseDeDatos::agregarRegistro(Registro& registro, NombreTabla& tabla){
    _tablas.at(tabla).insertar(registro);
}

const string_map<Tabla>& BaseDeDatos::tablas(){
    return _tablas;
}

void BaseDeDatos::eliminarTabla(NombreTabla& tabla){
    _tablas.erase(tabla);
}

void BaseDeDatos::eliminarRegistro(Valor &valor, NombreTabla& tabla){
    //COMPLETAR
}

linear_set<Registro> BaseDeDatos::realizarConsulta(Consulta& consulta){
    //COMPLETAR
}