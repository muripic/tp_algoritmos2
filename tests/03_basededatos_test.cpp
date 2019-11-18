#include "gtest-1.8.1/gtest.h"
#include "../src/BaseDeDatos.h"


TEST(test_basededatos, tabla_unica) {
    linear_set<NombreCampo> campos;
    campos.fast_insert("Nombre");
    campos.fast_insert("Apellido");
    campos.fast_insert("Programa");
    campos.fast_insert("CharID");

    BaseDeDatos bdd;
    ASSERT_EQ(bdd.tablas().size(), 0);

    bdd.agregarTabla("personajes", "CharID", campos);
    ASSERT_EQ(bdd.tablas().size(), 1);

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

    ASSERT_EQ(bdd.tablas().at("personajes").registros().size(), 0);

    bdd.agregarRegistro(char0001, "personajes");
    bdd.agregarRegistro(char0002, "personajes");
    bdd.agregarRegistro(char0003, "personajes");
    bdd.agregarRegistro(char0004, "personajes");
    bdd.agregarRegistro(char0005, "personajes");

    ASSERT_EQ(bdd.tablas().at("personajes").registros().size(), 5);

    bdd.eliminarRegistro("0004", "personajes");
    ASSERT_EQ(bdd.tablas().at("personajes").registros().size(), 4);

    bdd.eliminarTabla("personajes");
    ASSERT_EQ(bdd.tablas().size(), 0);
}

TEST(test_basededatos, from) {
    linear_set<NombreCampo> campos;
    campos.fast_insert("Nombre");
    campos.fast_insert("Apellido");
    campos.fast_insert("Programa");
    campos.fast_insert("CharID");

    BaseDeDatos bdd;
    bdd.agregarTabla("personajes", "CharID", campos);

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

    Consulta q1("from(personajes)");
    ASSERT_EQ(q1.tipo_consulta(), FROM);
    ASSERT_EQ(q1.nombre_tabla(), "personajes");

    ASSERT_EQ(bdd.tablas().size(), 1);

    bdd.agregarRegistro(char0001, "personajes");
    bdd.agregarRegistro(char0002, "personajes");
    bdd.agregarRegistro(char0003, "personajes");
    bdd.agregarRegistro(char0004, "personajes");
    bdd.agregarRegistro(char0005, "personajes");
    ASSERT_EQ(bdd.tablas().at("personajes").registros().size(), 5);

    vector<Registro> resFrom = bdd.realizarConsulta(q1);
    ASSERT_EQ(resFrom.size(), 5);
    ASSERT_EQ(resFrom[2], char0003);
}