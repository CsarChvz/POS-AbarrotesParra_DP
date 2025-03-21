#ifndef SEGURIDAD_H
#define SEGURIDAD_H
#define USUARIO_LENGTH 20

extern char usuario_actual[USUARIO_LENGTH]; // Hacer accesible la variable global

int checar_credenciales(const char *usuario, const char *contrasena);

int inicio_sesion();

void cerrar_sesion();

int obtener_rol(const char *usuario);

void cambiar_contrasena(char *nueva_contrasena);
#endif

