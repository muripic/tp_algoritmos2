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

typedef vector<Registro> Respuesta;

class BaseDeDatos {
public:
    BaseDeDatos();
    void agregarTabla(const NombreTabla &tabla, const NombreCampo &clave, const linear_set<NombreCampo> &campos);
    void agregarRegistro(Registro& registro, const NombreTabla &tabla);
    const string_map<Tabla> &tablas() const;
    void eliminarTabla(const NombreTabla &tabla);
    void eliminarRegistro(const Valor &valor, const NombreTabla &tabla);
    Respuesta& realizarConsulta(const Consulta &consulta);

private:
    string_map<Tabla> _tablas;

    //FUNCIONES AUXILIARES

    Respuesta& fromAux(const NombreTabla &t);
    Respuesta& selectAux(const Consulta &q, const NombreCampo &c, const Valor &v);
    Respuesta& selectProdAux(const Consulta &q, const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c, const Valor &v);
    Respuesta& matchAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    Respuesta& joinAux(const NombreTabla &t1, const NombreTabla &t2, const NombreCampo &c1, const NombreCampo &c2);
    Respuesta& renameAux(const Consulta &q, const NombreCampo &c1, const NombreCampo &c2);
    Respuesta& projAux(const Consulta &q, const set<NombreCampo> &cs);
    Respuesta& interAux(const Consulta &q1, const Consulta &q2);
    Respuesta& unionAux(const Consulta &q1, const Consulta &q2);
    Respuesta& productAux(const Consulta &q1, const Consulta &q2);
    bool pertenece(const Registro& r, const vector<Registro>& rs) const;

};

#endif //TP3_BASEDEDATOS_H
