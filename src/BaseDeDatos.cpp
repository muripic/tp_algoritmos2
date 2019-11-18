#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() : _tablas() {
}

void
BaseDeDatos::agregarTabla(const NombreTabla &tabla, const NombreCampo &clave, const linear_set<NombreCampo> &campos) {
    Tabla nueva(campos, clave);
    pair<string, Tabla> entrada = make_pair(tabla, nueva);
    _tablas.insert(entrada);
}

void BaseDeDatos::agregarRegistro(Registro &registro, const NombreTabla &tabla) {
    _tablas.at(tabla).insertar(registro);
}

const string_map<Tabla> &BaseDeDatos::tablas() const {
    return _tablas;
}

void BaseDeDatos::eliminarTabla(const NombreTabla &tabla) {
    _tablas.erase(tabla);
}

void BaseDeDatos::eliminarRegistro(const Valor &valor, const NombreTabla &tabla) {
    _tablas.at(tabla).borrar(valor);
}

Respuesta BaseDeDatos::realizarConsulta(const Consulta &consulta) {
    if (consulta.tipo_consulta() == FROM) {
        return fromAux(consulta.nombre_tabla());
    } else if (consulta.tipo_consulta() == SELECT) {
        return selectAux(consulta.subconsulta1(), consulta.campo1(), consulta.valor());
    } else if (consulta.tipo_consulta() == MATCH) {
        return matchAux(consulta.subconsulta1(), consulta.campo1(), consulta.campo2());
    } else if (consulta.tipo_consulta() == PROJ) {
        return projAux(consulta.subconsulta1(), consulta.conj_campos());
    } else if (consulta.tipo_consulta() == RENAME) {
        return renameAux(consulta.subconsulta1(), consulta.campo1(), consulta.campo2());
    } else if (consulta.tipo_consulta() == INTER) {
        return interAux(consulta.subconsulta1(), consulta.subconsulta2());
    } else if (consulta.tipo_consulta() == UNION) {
        return unionAux(consulta.subconsulta1(), consulta.subconsulta2());
    } else { //consulta.tipo_consulta() == PRODUCT
        return productAux(consulta.subconsulta1(), consulta.subconsulta2());
    }
}

Respuesta BaseDeDatos::fromAux(const NombreTabla &t) {
    Respuesta res;
    if (_tablas.count(t) == 1) {
        for (const Registro &r : _tablas.at(t).registros())
            res.push_back(r);
    }
    return res;
}

Respuesta BaseDeDatos::selectAux(const Consulta &q, const NombreCampo &c, const Valor &v) {
    Respuesta res;
    if (q.tipo_consulta() == FROM and _tablas.count(q.nombre_tabla())) {
        //Optimización 1: Select con clave
        Tabla *t = &(_tablas.at(q.nombre_tabla()));
        if (c == t->clave()) {
            res.push_back(t->regPorClave(v));
        } else if (t->campos().count(c) == 1) {
            //Optimización 2: Select sin clave
            auto itCol = t->obtenerColumna(c).begin();
            while (itCol != t->obtenerColumna(c).end()) {
                if (itCol->second == v) {
                    auto itR = itCol->first;
                    res.push_back(*itR);
                }
                ++itCol;
            }
        }
    } else if (q.tipo_consulta() == SELECT and q.subconsulta1().tipo_consulta() == FROM and
               _tablas.count(q.subconsulta1().nombre_tabla()) == 1 and
               c == _tablas.at(q.subconsulta1().nombre_tabla()).clave()) {
        //Optimización 4: Select con clave de select sin clave
        Tabla *t = &(_tablas.at(q.subconsulta1().nombre_tabla()));
        if (t->campos().count(q.campo1()) == 1 and t->regPorClave(v)[q.campo1()] == q.valor()) {
            res.push_back(t->regPorClave(v));
        }
    } else if (q.tipo_consulta() == PRODUCT and q.subconsulta1().tipo_consulta() == FROM and
               q.subconsulta2().tipo_consulta() == FROM) {
        //Optimización 5: Select de clave de un producto
        NombreTabla nt1 = q.subconsulta1().nombre_tabla();
        NombreTabla nt2 = q.subconsulta2().nombre_tabla();
        if (nt1 != nt2 and _tablas.at(nt1).clave() == c) {
            res = selectProdAux(q.subconsulta1().subconsulta1(), nt1, nt2, c, v);
        }
    } else {
        //Caso general
        Respuesta rs = realizarConsulta(q);
        auto itR = rs.begin();
        while (itR != rs.end()) {
            if (itR->campos().count(c) == 1 and (*itR)[c] == v) {
                res.push_back(*itR);
            }
            ++itR;
        }
    }
    return res;
}

Respuesta
BaseDeDatos::selectProdAux(const Consulta &q, const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c,
                           const Valor &v) {
    Respuesta res;
    Registro r1 = *(selectAux(q, c, v).begin());
    linear_set<Registro>::const_iterator it = _tablas.at(t2).registros().begin();
    while (it != _tablas.at(t2).registros().end()) {
        Registro r2 = *it;
        Registro rNuevo; //OJO: Ver si funciona así
        res.push_back(rNuevo);
        linear_set<NombreCampo>::const_iterator itCamp1 = r1.campos().begin();
        while (itCamp1 != r1.campos().end()) {
            res[res.size() - 1].definir(*itCamp1, r1[*itCamp1]);
            ++itCamp1;
        }
        linear_set<NombreCampo>::const_iterator itCamp2 = r2.campos().begin();
        while (itCamp1 != r2.campos().end()) {
            res[res.size() - 1].definir(*itCamp2, r2[*itCamp2]);
            ++itCamp2;
        }
        ++it;
    }
    return res;
}

Respuesta BaseDeDatos::matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta
BaseDeDatos::joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta BaseDeDatos::projAux(const Consulta &q, const set<NombreCampo> &cs) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta BaseDeDatos::renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta BaseDeDatos::interAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta BaseDeDatos::unionAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    //COMPLETAR
    return res;
}

Respuesta BaseDeDatos::productAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    //COMPLETAR
    return res;
}