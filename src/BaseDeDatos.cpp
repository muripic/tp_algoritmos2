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
        cout << "Select de clave de un producto" << endl;
        NombreTabla nt1 = q.subconsulta1().nombre_tabla();
        NombreTabla nt2 = q.subconsulta2().nombre_tabla();
        if (nt1 != nt2 and _tablas.at(nt1).clave() == c) {
            res = selectProdAux(q.subconsulta1(), nt1, nt2, c, v);
        }
        //"select(product(from(lineas),from(estaciones)),id_linea, '1')"
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
        while (itCamp2 != r2.campos().end()) {
            res[res.size() - 1].definir(*itCamp2, r2[*itCamp2]);
            ++itCamp2;
        }
        ++it;
    }
    return res;
}

Respuesta BaseDeDatos::matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    //Optimización 3: Join con claves
    if (q.tipo_consulta() == PRODUCT and q.subconsulta1().tipo_consulta() == FROM and
        q.subconsulta2().tipo_consulta() == FROM and
        q.subconsulta1().nombre_tabla() != q.subconsulta2().nombre_tabla() and
        _tablas.count(q.subconsulta1().nombre_tabla()) == 1 and
        _tablas.count(q.subconsulta2().nombre_tabla()) == 1 and
        _tablas.at(q.subconsulta1().nombre_tabla()).clave() == c1 and
        _tablas.at(q.subconsulta2().nombre_tabla()).clave() == c2) {
        return joinAux(q.subconsulta1().nombre_tabla(), q.subconsulta2().nombre_tabla(), c1, c2);
    }
    Respuesta res;
    Respuesta rs = realizarConsulta(q);
    for (Registro &r : rs) {
        if (r.campos().count(c1) == 1 and r.campos().count(c2) == 1 and r[c1] == r[c2]) {
            res.push_back(r);
        }
    }
    return res;
}

Respuesta
BaseDeDatos::joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta res;
    Tabla *tabMen = nullptr;
    Tabla *tabMay = nullptr;
    if (_tablas.at(t1).registros().size() <= _tablas.at(t2).registros().size()) {
        tabMen = &_tablas.at(t1);
        tabMay = &_tablas.at(t2);
    } else {
        tabMay = &_tablas.at(t1);
        tabMen = &_tablas.at(t2);
    }
    linear_set<Valor>::const_iterator itClave = tabMen->valoresClave().begin();
    while (itClave != tabMen->valoresClave().end()) {
        Valor v = *itClave;
        if (tabMay->existeRegConClave(v)) {
            Registro rNuevo;
            Registro r1 = tabMen->regPorClave(v);
            Registro r2 = tabMay->regPorClave(v);
            linear_set<NombreCampo>::const_iterator itCamp1 = r1.campos().begin();
            while (itCamp1 != r1.campos().end()) {
                NombreCampo c = *itCamp1;
                rNuevo.definir(c, r1[c]);
                ++itCamp1;
            }
            linear_set<NombreCampo>::const_iterator itCamp2 = r2.campos().begin();
            while (itCamp2 != r2.campos().end()) {
                NombreCampo c = *itCamp2;
                rNuevo.definir(c, r2[c]);
                ++itCamp2;
            }
            res.push_back(rNuevo);
        }
        ++itClave;
    }
    return res;
}

Respuesta BaseDeDatos::projAux(const Consulta &q, const set<NombreCampo> &cs) {
    Respuesta res;
    Respuesta rs = realizarConsulta(q);
    for (Registro& r : rs){
        auto itCampos = r.campos().begin();
        Registro rNuevo;
        while(itCampos != r.campos().end()){
            NombreCampo c = *itCampos;
            if (cs.count(c) == 1){
                rNuevo.definir(c, r[c]);
            }
            ++itCampos;
        }
        if (!rNuevo.campos().empty()){
            res.push_back(rNuevo);
        }
    }
    return res;
}

Respuesta BaseDeDatos::renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2) {
    Respuesta res;
    Respuesta rs = realizarConsulta(q);
    Respuesta::const_iterator itReg = rs.begin();
    while(itReg != rs.end()){
        Registro r = *itReg;
        if (r.campos().count(c2) >= 1 or r.campos().count(c1) < 1){
            res.push_back(r);
        } else {
            Registro rNuevo;
            linear_set<NombreCampo>::const_iterator itCampos = r.campos().begin();
            while(itCampos != r.campos().end()){
                NombreCampo c = *itCampos;
                if (c == c1){
                    rNuevo.definir(c2, r[c]);
                } else {
                    rNuevo.definir(c, r[c]);
                }
                ++itCampos;
            }
            res.push_back(rNuevo);
        }
        ++itReg;
    }
    return res;
}


bool BaseDeDatos::pertenece(const Registro &r, const vector<Registro> &rs) const {
    for (const Registro& e : rs) {
        if (e == r) {
            return true;
        }
    }
    return false;
}


Respuesta BaseDeDatos::interAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    Respuesta rs1 = realizarConsulta(q1);
    Respuesta rs2 = realizarConsulta(q2);
    for (Registro &r : rs1) {
        if (pertenece(r, rs2)) {
            res.push_back(r);
        }
    }
    return res;
}

Respuesta BaseDeDatos::unionAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    Respuesta rs1 = realizarConsulta(q1);
    Respuesta rs2 = realizarConsulta(q2);
    Respuesta::const_iterator it1 = rs1.begin();
    while (it1 != rs1.end()) {
        res.push_back(*it1);
        ++it1;
    }
    Respuesta::const_iterator it2 = rs2.begin();
    while (it2 != rs2.end()) {
        res.push_back(*it2);
        ++it2;
    }
    return res;
}


Respuesta BaseDeDatos::productAux(const Consulta &q1, const Consulta &q2) {
    Respuesta res;
    Respuesta rs1 = realizarConsulta(q1);
    Respuesta rs2 = realizarConsulta(q2);
    auto it1 = rs1.begin();
    while(it1 != rs1.end()){
        auto it2 = rs2.begin();
        while (it2 != rs2.end()){
            Registro rNuevo;
            Registro r1 = *it1;
            Registro r2 = *it2;
            linear_set<NombreCampo>::const_iterator itCamp1 = r1.campos().begin();
            while(itCamp1 != r1.campos().end()){
                NombreCampo c = *itCamp1;
                rNuevo.definir(c, r1[c]);
                ++itCamp1;
            }
            linear_set<NombreCampo>::const_iterator itCamp2 = r2.campos().begin();
            while(itCamp2 != r2.campos().end()){
                NombreCampo c = *itCamp2;
                rNuevo.definir(c, r2[c]);
                ++itCamp2;
            }
            res.push_back(rNuevo);
            ++it2;
        }
        ++it1;    
    }
    return res;
}