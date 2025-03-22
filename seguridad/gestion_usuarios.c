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
    FILE *archivo = fopen("data/usuarios.csv", "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }
    
    char linea[256];
    int count = 0;
    
    // Saltar la l�nea de cabecera si existe
    fgets(linea, 256, archivo);
    
    while (fgets(linea, 256, archivo) != NULL && count < max_usuarios) {
        Usuario u;
        sscanf(linea, "%d,%[^,],%[^,],%d", 
               &u.id, u.usuario, u.contrasena, &u.rol, u.activo );
        usuarios[count] = u;
        count++;
    }
    
    fclose(archivo);
    return count;
}

void listarUsuarios() {
    Usuario usuarios[100]; // Asumimos máximo 100 usuarios
    int cantidad;
    
    cantidad = obtener_usuarios(usuarios, 100);
    
    if (cantidad == 0) {
        printf("\nNo hay usuarios registrados o error al leer el archivo.\n");
        return;
    }
    
    printf("\n--- Lista de Usuarios ---\n");
    printf("ID\tNombre\t\tApellido\t\tEdad\n");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < cantidad; i++) {
        printf("%d\t%-15s\t%-15s\t%d\n", 
               usuarios[i].id, usuarios[i].usuario, 
               usuarios[i].rol, usuarios[i].activo);
    }
}