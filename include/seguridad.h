// seguridad.h
#ifndef SEGURIDAD_H
#define SEGURIDAD_H

#define USUARIO_LENGTH 20

extern char usuario_actual[USUARIO_LENGTH];
extern int usuario_id; // Nueva declaración extern

int checar_credenciales(const char *usuario, const char *contrasena);
int inicio_sesion();
void cerrar_sesion();
int obtener_rol(const char *usuario);
void cambiar_contrasena();

typedef struct {
    int id;
    char usuario[USUARIO_LENGTH];
    int role;
} UsuarioGlobal;

extern UsuarioGlobal usuario_global;

#endif