#include <stdio.h>
#include <stdlib.h>
#define MAX_USERS 100
#define USUARIO_LENGTH 50
#define CONTRASENA_LENGTH 50

typedef struct {
    int id;
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];
    int rol;
    int activo;
} Usuario;

int obtenerUsuarios(Usuario usuarios[], int max_usuarios) {
    FILE *archivo = fopen("common/data/usuarios.csv", "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }
    
    char linea[256];
    int count = 0;
    
    // Saltar la línea de cabecera si existe
    fgets(linea, 256, archivo);
    
    // Leer los usuarios desde el archivo CSV
    while (fgets(linea, 256, archivo) != NULL && count < max_usuarios) {
        Usuario u;
        sscanf(linea, "%d,%[^,],%[^,],%d,%d", 
               &u.id, u.usuario, u.contrasena, &u.rol, &u.activo);
        usuarios[count] = u;
        count++;
    }
    
    fclose(archivo);
    return count; // Retorna la cantidad de usuarios leídos
}



int listarUsuarios() {
    Usuario usuarios[MAX_USERS]; // Arreglo para almacenar usuarios
    int cantidadUsuariosLeidos, i;
    
    cantidadUsuariosLeidos = obtenerUsuarios(usuarios, MAX_USERS);
    
    if (cantidadUsuariosLeidos == 0) {
        printf("\nNo hay usuarios registrados o error al leer el archivo.\n");
        return 1;
    }
    
    printf("\n--- Lista de Usuarios ---\n");
    for (i = 0; i < cantidadUsuariosLeidos; i++) {
        printf("ID: %d, Usuario: %-15s, Rol: %d, Activo: %d\n", 
               usuarios[i].id, usuarios[i].usuario, usuarios[i].rol, usuarios[i].activo);
    }
    return 0;
}
