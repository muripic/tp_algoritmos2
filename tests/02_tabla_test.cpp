#include "gtest-1.8.1/gtest.h"
#include "../src/Tabla.h"

TEST(test_tabla, vacia) {
    linear_set<NombreCampo> campos;
    campos.fast_insert("Nombre");
    campos.fast_insert("Apellido");
    campos.fast_insert("Programa");
    campos.fast_insert("CharID");

    Tabla personajes(campos, "CharID");
    ASSERT_EQ(personajes.campos().size(), 4);
    ASSERT_FALSE(personajes.existeRegConClave("0001"));
}

/*Tabla(linear_set<NombreCampo>& campos, NombreCampo& clave);
void insertar(Registro &registro);
void borrar(Valor &valor);
const linear_set<NombreCampo>& campos();
const NombreCampo& clave();
const linear_set<Registro>& registros();
bool existeRegConClave(Valor &valor);
const Registro& regPorClave(Valor &valor);
const linear_set<Valor>& valoresClave();
linear_map<linear_set<Registro>::iterator, Valor>& obtenerColumna(NombreCampo &campo) const;*/