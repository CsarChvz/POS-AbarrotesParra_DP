#ifndef GESTION_USUARIOS_H
#define GESTION_USUARIOS_H


typedef struct {
    int id;
    char nombre[50];
    char apellido[50];
    int edad;
} Usuario;

typedef struct {
    int id;
    int usuario_id;
    char descripcion[100];
    float monto;
    char fecha[11]; // formato: YYYY-MM-DD
} Transaccion;

int obtenerUsuarios(Usuario usuarios[], int max_usuarios);

int listarUsuarios();

#endif
