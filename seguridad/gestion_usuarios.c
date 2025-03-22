#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
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


int obtenerSiguienteId() {
    FILE *archivo = fopen("common/data/usuarios.csv", "r");
    if (archivo == NULL) {
        return 1; // Si no existe el archivo, el primer ID es 1
    }
    
    char linea[256];
    int maxId = 0;
    int id;
    
    // Saltar cabecera
    fgets(linea, 256, archivo);
    
    while (fgets(linea, 256, archivo) != NULL) {
        sscanf(linea, "%d", &id);
        if (id > maxId) {
            maxId = id;
        }
    }
    
    fclose(archivo);
    return maxId + 1;
}

// Funcion para guardar un nuevo usuario en el archivo usuarios.csv
int guardarUsuario(const char *nombre_usuario, int role) {
    // Validación del nombre de usuario
    if (nombre_usuario == NULL || strlen(nombre_usuario) == 0) {
        printf("Error: El nombre de usuario es inválido.\n");
        return 0; // Error en nombre de usuario
    }

    // Abre el archivo en modo 'append' (agregar al final)
    FILE *archivo = fopen("common/data/usuarios.csv", "a");
    if (archivo == NULL) {
        printf("Error: ");
        perror("Error al abrir el archivo");  // Mejor manejo de errores
        return 0; // Error al abrir el archivo
    }

    int id = obtenerSiguienteId();  // Obtener el siguiente ID (suponiendo que esta función está definida correctamente)

    // Escribir los datos del usuario en el archivo CSV
    if (fprintf(archivo, "%d,%s,%s,%d,%d\n", id, nombre_usuario, "contrasena", role, 1) < 0) {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return 0; // Error al escribir en el archivo
    }

    fclose(archivo);
    return 1; // Éxito
}