#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() : _tablas(){
}

void BaseDeDatos::agregarTabla(NombreTabla& tabla, NombreCampo& clave, linear_set<NombreCampo>& campos){
    Tabla nueva(campos, clave);
    pair<string, Tabla> entrada = make_pair(tabla, nueva);
    _tablas.insert(entrada);
}

void BaseDeDatos::agregarRegistro(Registro& registro, NombreTabla& tabla){
    //COMPLETAR
}

const string_map<Tabla>& BaseDeDatos::tablas(){
    //COMPLETAR
}

void BaseDeDatos::eliminarTabla(NombreTabla& tabla){
    //COMPLETAR
}

void BaseDeDatos::eliminarRegistro(Valor &valor, NombreTabla& tabla){
    //COMPLETAR
}

linear_set<Registro> BaseDeDatos::realizarConsulta(Consulta& consulta){
    //COMPLETAR
}