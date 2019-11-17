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
    void agregarTabla(const NombreTabla &tabla, const NombreCampo &clave, const linear_set<NombreCampo> &campos);
    void agregarRegistro(Registro& registro, const NombreTabla &tabla);
    const string_map<Tabla> &tablas() const;
    void eliminarTabla(const NombreTabla &tabla);
    void eliminarRegistro(const Valor &valor, const NombreTabla &tabla);
    linear_set<Registro> realizarConsulta(const Consulta &consulta);

private:
    string_map<Tabla> _tablas;

    //FUNCIONES AUXILIARES

    linear_set<Registro> fromAux(const NombreTabla &t);
    linear_set<Registro> selectAux(const Consulta &q, const NombreCampo &c, const Valor &v);
    linear_set<Registro> selectProdAux(const Consulta &q, const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c, const Valor &v);
    linear_set<Registro> matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    linear_set<Registro> joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2);
    linear_set<Registro> renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    linear_set<Registro> projAux(const Consulta &q, const set<NombreCampo> &cs);
    linear_set<Registro> interAux(const Consulta &q1, const Consulta &q2);
    linear_set<Registro> unionAux(const Consulta &q1, const Consulta &q2);
    linear_set<Registro> productAux(const Consulta &q1, const Consulta &q2);

};

#endif //TP3_BASEDEDATOS_H
