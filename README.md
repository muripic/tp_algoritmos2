Justificación: linear_set está implementado sobre linear_map<T, bool> y para hacer erase necesita comparar la clave T.
Por eso es necesario contar con el operador de comparación == en el módulo registro.
Para copiar los registros al devolver el resultado de las consultas, fue necesario agregar un operador de copia en el
módulo Registro.