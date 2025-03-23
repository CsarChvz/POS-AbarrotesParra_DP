#ifndef GESTION_USUARIOS_H
#define GESTION_USUARIOS_H


typedef struct {
    int id;
    char nombre[50];
    char apellido[50];
    int edad;
} Usuario;


int obtenerUsuarios(Usuario usuarios[], int max_usuarios);

int listarUsuarios();

int guardarUsuario(const char *nombre_usuario, int role);

void cambiarEstatusUsuarioMenu();

void cambiarRolUsuarioMenu();

#endif
