#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() : _tablas() {
}

void
BaseDeDatos::agregarTabla(const NombreTabla &tabla, const NombreCampo &clave, const linear_set<NombreCampo> &campos) {
    Tabla nueva(campos, clave);
    _tablas.insert(make_pair(tabla, nueva));
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

Respuesta &BaseDeDatos::realizarConsulta(const Consulta &consulta) {
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

Respuesta &BaseDeDatos::fromAux(const NombreTabla &t) {
    Respuesta *res = new Respuesta;
    if (_tablas.count(t) == 1) {
        for (const Registro &r : _tablas.at(t).registros())
            res->push_back(r);
    }
    return *res;
}

Respuesta &BaseDeDatos::selectAux(const Consulta &q, const NombreCampo &c, const Valor &v) {
    if (q.tipo_consulta() == FROM and _tablas.count(q.nombre_tabla()) == 1) {

        //Optimización 1: Select con clave
        Tabla *t = &(_tablas.at(q.nombre_tabla()));
        if (c == t->clave()) {
            Respuesta *res = new Respuesta;
            res->push_back(t->regPorClave(v));
            return *res;
        } else if (t->campos().count(c) == 1) {

            //Optimización 2: Select sin clave
            Respuesta *res = new Respuesta;
            auto itCol = t->obtenerColumna(c).begin();
            while (itCol != t->obtenerColumna(c).end()) {
                if (itCol->second == v) {
                    auto itR = itCol->first;
                    res->push_back(*itR);
                }
                itCol++;
            }
            return *res;
        }
    } else if (q.tipo_consulta() == SELECT and q.subconsulta1().tipo_consulta() == FROM and
               _tablas.count(q.subconsulta1().nombre_tabla()) == 1 and
               c == _tablas.at(q.subconsulta1().nombre_tabla()).clave()) {

        //Optimización 4: Select con clave de select sin clave
        Respuesta *res = new Respuesta;
        Tabla *t = &(_tablas.at(q.subconsulta1().nombre_tabla()));
        if (t->campos().count(q.campo1()) == 1 and t->regPorClave(v)[q.campo1()] == q.valor()) {
            res->push_back(t->regPorClave(v));
        }
        return *res;
    } else if (q.tipo_consulta() == PRODUCT and q.subconsulta1().tipo_consulta() == FROM and
               q.subconsulta2().tipo_consulta() == FROM) {

        //Optimización 5: Select de clave de un producto
        NombreTabla nt1 = q.subconsulta1().nombre_tabla();
        NombreTabla nt2 = q.subconsulta2().nombre_tabla();
        if (nt1 != nt2 and _tablas.at(nt1).clave() == c) {
            return selectProdAux(q.subconsulta1(), nt1, nt2, c, v);
        }
    }
    //Caso general
    Respuesta *res = new Respuesta;
    Respuesta *rs = &realizarConsulta(q);
    for (const Registro &r : *rs) {
        if (r.campos().count(c) == 1 and r[c] == v) {
            res->push_back(r);
        }
    }
    delete rs;
    return *res;
}

Respuesta &
BaseDeDatos::selectProdAux(const Consulta &q, const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c,
                           const Valor &v) {
    Respuesta *res = new Respuesta;
    Respuesta *rs = &selectAux(q, c, v);
    Registro r1 = (*rs)[0];
    auto it = _tablas.at(t2).registros().begin();
    while (it != _tablas.at(t2).registros().end()) {
        Registro r2 = *it;
        Registro rNuevo;
        auto itCamp1 = r1.campos().begin();
        while (itCamp1 != r1.campos().end()) {
            rNuevo.definir(*itCamp1, r1[*itCamp1]);
            ++itCamp1;
        }
        auto itCamp2 = r2.campos().begin();
        while (itCamp2 != r2.campos().end()) {
            rNuevo.definir(*itCamp2, r2[*itCamp2]);
            ++itCamp2;
        }
        res->push_back(rNuevo);
        ++it;
    }
    delete rs;
    return *res;
}


Respuesta &BaseDeDatos::matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    //Optimización 3: Join con claves
    if (q.tipo_consulta() == PRODUCT and q.subconsulta1().tipo_consulta() == FROM and
        q.subconsulta2().tipo_consulta() == FROM and
        q.subconsulta1().nombre_tabla() != q.subconsulta2().nombre_tabla() and
        _tablas.count(q.subconsulta1().nombre_tabla()) == 1 and
        _tablas.count(q.subconsulta2().nombre_tabla()) == 1 and
        _tablas.at(q.subconsulta1().nombre_tabla()).clave() == c1 and
        _tablas.at(q.subconsulta2().nombre_tabla()).clave() == c2) {
        return joinAux(q.subconsulta1().nombre_tabla(), q.subconsulta2().nombre_tabla(), c1, c2);
    } else if (q.tipo_consulta() == FROM and _tablas.at(q.nombre_tabla()).campos().count(c1) == 1 and
               _tablas.at(q.nombre_tabla()).campos().count(c2) == 1) {

        //NUEVA OPTIMIZACIÓN: Match de from
        Respuesta *res = new Respuesta;
        for (const Registro &r : _tablas.at(q.nombre_tabla()).registros()) {
            if (r[c1] == r[c2]) {
                res->push_back(r);
            }
        }
        return *res;
    } else {
        /*Respuesta *res = &realizarConsulta(q); //SIN COPIAR
        int size = res->size();
        int i = 0;
        while (i < size) {
            if ((*res)[i].campos().count(c1) == 1 and (*res)[i].campos().count(c2) == 1 and
                (*res)[i][c1] == (*res)[i][c2]) {
                i++;
            } else {
                swap((*res)[i], (*res)[size - 1]);
                res->pop_back();
                size--;
            }
        }
        return *res;*/
        Respuesta *res = new Respuesta;   //CON COPIA
          Respuesta *rs = &realizarConsulta(q);
          for (const Registro &r : *rs) {
              if (r.campos().count(c1) == 1 and r.campos().count(c2) == 1 and r[c1] == r[c2]) {
                  res->push_back(r);
              }
          }
          delete rs;
          return *res;
    }
}

Respuesta &
BaseDeDatos::joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta *res = new Respuesta;
    Tabla *tabMen = nullptr;
    Tabla *tabMay = nullptr;
    if (_tablas.at(t1).registros().size() <= _tablas.at(t2).registros().size()) {
        tabMen = &_tablas.at(t1);
        tabMay = &_tablas.at(t2);
    } else {
        tabMay = &_tablas.at(t1);
        tabMen = &_tablas.at(t2);
    }
    auto itClave = tabMen->valoresClave().begin();
    while (itClave != tabMen->valoresClave().end()) {
        Valor v = *itClave;
        if (tabMay->existeRegConClave(v)) {
            Registro rNuevo;
            Registro r1 = tabMen->regPorClave(v);
            Registro r2 = tabMay->regPorClave(v);
            auto itCamp1 = r1.campos().begin();
            while (itCamp1 != r1.campos().end()) {
                NombreCampo c = *itCamp1;
                rNuevo.definir(c, r1[c]);
                ++itCamp1;
            }
            auto itCamp2 = r2.campos().begin();
            while (itCamp2 != r2.campos().end()) {
                NombreCampo c = *itCamp2;
                rNuevo.definir(c, r2[c]);
                ++itCamp2;
            }
            res->push_back(rNuevo);
        }
        ++itClave;
    }
    return *res;
}

Respuesta &BaseDeDatos::projAux(const Consulta &q, const set<NombreCampo> &cs) {
    Respuesta *res = new Respuesta;
    Respuesta *rs = &realizarConsulta(q);
    for (const Registro &r : *rs) {
        auto itCampos = r.campos().begin();
        Registro rNuevo;
        while (itCampos != r.campos().end()) {
            NombreCampo c = *itCampos;
            if (cs.count(c) == 1) {
                rNuevo.definir(c, r[c]);
            }
            ++itCampos;
        }
        if (!rNuevo.campos().empty()) {
            res->push_back(rNuevo);
        }
    }
    delete rs;
    return *res;
}

Respuesta &BaseDeDatos::renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta *res = &realizarConsulta(q);
    for (Registro &r : *res) {
        if (r.campos().count(c2) == 0 and r.campos().count(c1) == 1) {
            r.definir(c2, r[c1]);
            r.borrarCampo(c1);
        }
    }
    return *res;
}


bool BaseDeDatos::pertenece(const Registro &r, const vector<Registro> &rs) const {
    for (const Registro &e : rs) {
        if (e == r) {
            return true;
        }
    }
    return false;
}


Respuesta &BaseDeDatos::interAux(const Consulta &q1, const Consulta &q2) {
    Respuesta *res = new Respuesta;
    Respuesta *rs1 = &realizarConsulta(q1);
    Respuesta *rs2 = &realizarConsulta(q2);
    for (const Registro &r : *rs1) {
        if (pertenece(r, *rs2)) {
            res->push_back(r);
        }
    }
    delete rs1;
    delete rs2;
    return *res;
}

Respuesta &BaseDeDatos::unionAux(const Consulta &q1, const Consulta &q2) {
    Respuesta *rs1 = &realizarConsulta(q1);
    Respuesta *rs2 = &realizarConsulta(q2);
    for (const Registro &r : *rs2) {
        rs1->push_back(r);
    }
    delete rs2;
    return *rs1;
}

Respuesta &BaseDeDatos::productAux(const Consulta &q1, const Consulta &q2) {
    Respuesta *res = new Respuesta;
    Respuesta *rs1 = &realizarConsulta(q1);
    Respuesta *rs2 = &realizarConsulta(q2);
    for (const Registro &r1 : *rs1) {
        for (const Registro &r2 : *rs2) {
            Registro rNuevo;
            auto itCamp1 = r1.campos().begin();
            while (itCamp1 != r1.campos().end()) {
                NombreCampo c = *itCamp1;
                rNuevo.definir(c, r1[c]);
                ++itCamp1;
            }
            auto itCamp2 = r2.campos().begin();
            while (itCamp2 != r2.campos().end()) {
                NombreCampo c = *itCamp2;
                rNuevo.definir(c, r2[c]);
                ++itCamp2;
            }
            res->push_back(rNuevo);
        }
    }
    delete rs1;
    delete rs2;
    return *res;
}