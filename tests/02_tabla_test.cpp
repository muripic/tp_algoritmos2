#include "gtest-1.8.1/gtest.h"
#include "../src/Tabla.h"

TEST(test_tabla, vacia) {
    linear_set<NombreCampo> campos;
    campos.fast_insert("Nombre");
    campos.fast_insert("Apellido");
    campos.fast_insert("Programa");
    campos.fast_insert("CharID");

    Tabla personajes(campos, "CharID");
    ASSERT_EQ(personajes.registros().size(), 0);
    ASSERT_EQ(personajes.campos().size(), 4);
    ASSERT_FALSE(personajes.existeRegConClave("0001"));
}

TEST(test_tabla, insertar) {
    linear_set<NombreCampo> campos;
    campos.fast_insert("Nombre");
    campos.fast_insert("Apellido");
    campos.fast_insert("Programa");
    campos.fast_insert("CharID");

    Tabla personajes(campos, "CharID");
    Registro char0001;
    char0001.definir("Nombre", "Homer");
    char0001.definir("Apellido", "Simpson");
    char0001.definir("Programa", "The Simpsons");
    char0001.definir("CharID", "0001");
    Registro char0002;
    char0002.definir("Nombre", "Randy");
    char0002.definir("Apellido", "Marsh");
    char0002.definir("Programa", "South Park");
    char0002.definir("CharID", "0002");
    Registro char0003;
    char0003.definir("Nombre", "Peter");
    char0003.definir("Apellido", "Griffin");
    char0003.definir("Programa", "Family Guy");
    char0003.definir("CharID", "0003");
    Registro char0004;
    char0004.definir("Nombre", "Bart");
    char0004.definir("Apellido", "Simpson");
    char0004.definir("Programa", "The Simpsons");
    char0004.definir("CharID", "0004");
    Registro char0005;
    char0005.definir("Nombre", "Stan");
    char0005.definir("Apellido", "Marsh");
    char0005.definir("Programa", "South Park");
    char0005.definir("CharID", "0005");

    personajes.insertar(char0001);
    personajes.insertar(char0002);
    personajes.insertar(char0003);
    personajes.insertar(char0004);
    personajes.insertar(char0005);

    ASSERT_EQ(personajes.campos().size(), 4);
    ASSERT_EQ(personajes.registros().size(), 5);
    ASSERT_EQ(personajes.valoresClave().size(), 5);
    ASSERT_TRUE(personajes.existeRegConClave("0001"));
    ASSERT_TRUE(personajes.existeRegConClave("0002"));
    ASSERT_TRUE(personajes.existeRegConClave("0003"));
    ASSERT_TRUE(personajes.existeRegConClave("0004"));
    ASSERT_TRUE(personajes.existeRegConClave("0005"));
    ASSERT_EQ((personajes.regPorClave("0001"))["Nombre"], "Homer");
}

/* PARA TENER COMO REFERENCIA LAS FUNCIONES QUE HAY QUE TESTEAR
Tabla(linear_set<NombreCampo>& campos, NombreCampo& clave);
void insertar(Registro &registro);
void borrar(Valor &valor);
const linear_set<NombreCampo>& campos();
const NombreCampo& clave();
const linear_set<Registro>& registros();
bool existeRegConClave(Valor &valor);
const Registro& regPorClave(Valor &valor);
const linear_set<Valor>& valoresClave();
linear_map<linear_set<Registro>::iterator, Valor>& obtenerColumna(NombreCampo &campo) const;*/