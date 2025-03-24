// seguridad.h
#ifndef SEGURIDAD_H
#define SEGURIDAD_H

#define USUARIO_LENGTH 20


typedef struct {
    int id;
    char usuario[USUARIO_LENGTH];
    int rol;
    int activo;
} UsuarioGlobal;

extern UsuarioGlobal usuario_global;

int checar_credenciales(const char *usuario, const char *contrasena);
int inicio_sesion();
void cerrar_sesion();
int obtener_rol(const char *usuario);
void cambiar_contrasena();

#endif