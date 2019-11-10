#include "gtest-1.8.1/gtest.h"
#include "../src/Registro.h"

TEST(test_registro, unico) {
    Registro nuevo;
    ASSERT_EQ(nuevo.campos().size(), 0);
    nuevo.definir("Nombre", "Homer");
    ASSERT_EQ(nuevo.campos().size(), 1);
    ASSERT_EQ(nuevo["Nombre"], "Homer");
    nuevo.definir("Apellido", "Simpson");
    ASSERT_EQ(nuevo.campos().size(), 2);
    nuevo.definir("Nombre", "Lisa");
    ASSERT_EQ(nuevo["Nombre"], "Lisa");
}