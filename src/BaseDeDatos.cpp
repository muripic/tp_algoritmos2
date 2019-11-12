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
    _tablas.at(tabla).borrar(valor);
}

linear_set<Registro> BaseDeDatos::realizarConsulta(Consulta& consulta){
    if (consulta.tipo_consulta() == FROM){
        //return FROMAux(Nombre_tabla(q), b);
    } else if (consulta.tipo_consulta() == SELECT){
        //return SELECTAux(Subconsulta1(q), Campo1(q), Valor(q), b);
    } else if (consulta.tipo_consulta() == MATCH){
        //return MATCHAux(Subconsulta1(q), Campo1(q), Campo2(q), b);
    } else if (consulta.tipo_consulta() == PROJ){
        //return PROJAux(Subconsulta1(q), Conj_campos(q), b);
    } else if (consulta.tipo_consulta() == RENAME){
        //return RENAMEAux(Subconsulta1(q), Campo1(q), Campo2(q), b);
    } else if (consulta.tipo_consulta() == INTER){
        //return INTERAux(Subconsulta1(q), Subconsulta2(q), b);
    } else if (consulta.tipo_consulta() == UNION){
        //return UNIONAux(Subconsulta1(q), Subconsulta2(q), b);
    } else if (consulta.tipo_consulta() == PRODUCT){
        //return PRODUCTAux(Subconsulta1(q), Subconsulta2(q), b);
    }
}