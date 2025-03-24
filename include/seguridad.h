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

int inicio_sesion();
void cambiar_contrasena();
void cerrar_sesion();

#endif