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
    vector<Registro> realizarConsulta(const Consulta &consulta);

private:
    string_map<Tabla> _tablas;

    //FUNCIONES AUXILIARES

    vector<Registro> fromAux(const NombreTabla &t);
    vector<Registro> selectAux(const Consulta &q, const NombreCampo &c, const Valor &v);
    vector<Registro> selectProdAux(const Consulta &q, const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c, const Valor &v);
    vector<Registro> matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    vector<Registro> joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2);
    vector<Registro> renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    vector<Registro> projAux(const Consulta &q, const set<NombreCampo> &cs);
    vector<Registro> interAux(const Consulta &q1, const Consulta &q2);
    vector<Registro> unionAux(const Consulta &q1, const Consulta &q2);
    vector<Registro> productAux(const Consulta &q1, const Consulta &q2);

};

#endif //TP3_BASEDEDATOS_H
