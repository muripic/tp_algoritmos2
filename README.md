Cambios:
1) linear_set está implementado sobre linear_map<T, bool> y para hacer erase necesita comparar la clave T.
Por eso es necesario contar con el operador de comparación == en el módulo registro.
2) Se sobrecargó el operador [] en el módulo registro (una versión modificable y otra no) para poder utilizarlo
en el módulo Driver.