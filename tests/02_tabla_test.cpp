#include <fstream>
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

TEST(test_tabla, borrar) {
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

    personajes.insertar(char0001);
    personajes.insertar(char0002);
    personajes.insertar(char0003);

    ASSERT_EQ(personajes.registros().size(), 3);
    ASSERT_TRUE(personajes.existeRegConClave("0003"));
    personajes.borrar("0003");
    ASSERT_FALSE(personajes.existeRegConClave("0003"));
    ASSERT_EQ(personajes.registros().size(), 2);
    personajes.borrar("0002");
    ASSERT_FALSE(personajes.existeRegConClave("0002"));
    ASSERT_EQ(personajes.registros().size(), 1);
}

TEST(test_tabla, otras_funciones) {
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

    personajes.insertar(char0001);
    personajes.insertar(char0002);
    personajes.insertar(char0003);

    ASSERT_EQ(personajes.registros().size(), 3);
    ASSERT_TRUE(personajes.existeRegConClave("0003"));
    ASSERT_EQ(personajes.regPorClave("0003")["Nombre"], "Peter");

    ASSERT_EQ(personajes.valoresClave().size(), 3);
    ASSERT_EQ(personajes.valoresClave().count("0002"), 1);

    ASSERT_EQ(personajes.obtenerColumna("Apellido").size(), 3);
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

    personajes.insertar(char0001);
    personajes.insertar(char0002);
    personajes.insertar(char0003);

    ASSERT_EQ(personajes.registros().size(), 3);
    ASSERT_TRUE(personajes.existeRegConClave("0003"));
    ASSERT_EQ(personajes.regPorClave("0003")["Nombre"], "Peter");

    ASSERT_EQ(personajes.valoresClave().size(), 3);
    ASSERT_EQ(personajes.valoresClave().count("0002"), 1);

    ASSERT_EQ(personajes.obtenerColumna("Apellido").size(), 3);
}

TEST(test_tabla, stress) {

    linear_set<NombreCampo> cs;
    cs.insert("id");
    cs.insert("fecha");
    cs.insert("provincia_origen");
    cs.insert("localidad_origen");
    cs.insert("provincia_destino");
    cs.insert("localidad_destino");
    cs.insert("cant_lineas");
    cs.insert("cant_pasajeros");
    cs.insert("cant_servicios");

    Tabla interurbanos(cs, "id");

    interurbanos.cargarRegistros("interurbano");

    ASSERT_EQ(interurbanos.registros().size(), 16190);
}