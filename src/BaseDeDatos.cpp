#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() : _tablas(){
}

void BaseDeDatos::agregarTabla(const NombreTabla& tabla, const NombreCampo& clave, const linear_set<NombreCampo>& campos){
    Tabla nueva(campos, clave);
    pair<string, Tabla> entrada = make_pair(tabla, nueva);
    _tablas.insert(entrada);
}

void BaseDeDatos::agregarRegistro(Registro& registro, const NombreTabla& tabla){
    _tablas.at(tabla).insertar(registro);
}

const string_map<Tabla>& BaseDeDatos::tablas() const{
    return _tablas;
}

void BaseDeDatos::eliminarTabla(const NombreTabla& tabla){
    _tablas.erase(tabla);
}

void BaseDeDatos::eliminarRegistro(const Valor &valor, const NombreTabla& tabla){
    _tablas.at(tabla).borrar(valor);
}

linear_set<Registro> BaseDeDatos::realizarConsulta(Consulta& consulta){
    if (consulta.tipo_consulta() == FROM){
        return fromAux(consulta.nombre_tabla());
    } else if (consulta.tipo_consulta() == SELECT){
        return selectAux(consulta.subconsulta1(), consulta.campo1(), consulta.valor());
    } else if (consulta.tipo_consulta() == MATCH){
        return matchAux(consulta.subconsulta1(), consulta.campo1(), consulta.campo2());
    } else if (consulta.tipo_consulta() == PROJ){
        return projAux(consulta.subconsulta1(), consulta.conj_campos());
    } else if (consulta.tipo_consulta() == RENAME){
        return renameAux(consulta.subconsulta1(), consulta.campo1(), consulta.campo2());
    } else if (consulta.tipo_consulta() == INTER){
        return interAux(consulta.subconsulta1(), consulta.subconsulta2());
    } else if (consulta.tipo_consulta() == UNION){
        return unionAux(consulta.subconsulta1(), consulta.subconsulta2());
    } else { //consulta.tipo_consulta() == PRODUCT
        return productAux(consulta.subconsulta1(), consulta.subconsulta2());
    }
}

linear_set<Registro> BaseDeDatos::fromAux(const NombreTabla& t){
    linear_set<Registro> res;
    if (_tablas.count(t) == 1){
        res = _tablas.at(t).registros();
    }
    return res;
}

linear_set<Registro> BaseDeDatos::selectAux(const Consulta& q, const NombreCampo& c, const Valor& v){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::selectProdAux(const Consulta& q, const NombreTabla& t1, const NombreTabla& t2, const NombreCampo& c, const Valor& v){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::matchAux(const Consulta& q, const NombreCampo& c1, const NombreCampo& c2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::joinAux(const NombreTabla& t1, const NombreTabla& t2, const NombreCampo& c1, const NombreCampo& c2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::projAux(const Consulta& q, const set<NombreCampo>& cs){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::renameAux(const Consulta& q, const NombreCampo& c1, const NombreCampo& c2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::interAux(const Consulta& q1, const Consulta& q2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::unionAux(const Consulta& q1, const Consulta& q2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}

linear_set<Registro> BaseDeDatos::productAux(const Consulta& q1, const Consulta& q2){
    linear_set<Registro> res;
    //COMPLETAR
    return res;
}