#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para manejar strings con strcmp
#include "../include/seguridad.h"

#define MAX_USERS 3
#define USUARIO_LENGTH 20
#define CONTRASENA_LENGTH 20

// Estructura de Usuario
typedef struct {
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];
	int rol;
} Usuario;

// Lista de usuarios - Seteados para debugear
Usuario usuarios[MAX_USERS] = {
    {"admin", "admin123", 2},
    {"user1", "contrasena1", 1},
    {"user2", "contrasena2", 1}
};

// Variable global para almacenar el usuario logueado
char usuario_actual[USUARIO_LENGTH] = "";


int checar_credenciales(const char *usuario, const char *contrasena) {
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 && strcmp(usuarios[i].contrasena, contrasena) == 0) {
            return 1; 
        }
    }
    return 0;
}

// Se comprueba el rol que tiene el usuario
int obtener_rol(const char *usuario) {
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0) {
            return usuarios[i].rol; // Retorna el rol del usuario
        }
    }
    return -1; // Usuario no encontrado
}



// Función para iniciar sesión
int inicio_sesion() {
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];

    while (1) {
        printf("\n--- Inicio de Sesi\242n ---\n"); // Sesión (ó = ASCII 242)
        printf("Ingrese el nombre de usuario: ");
        scanf("%19s", usuario); 

        printf("Ingrese la contrase\244a: "); // Contraseña (ñ = ASCII 244)
        scanf("%19s", contrasena);

        // Verificar credenciales
        if (checar_credenciales(usuario, contrasena)) {
            strcpy(usuario_actual, usuario); // Guardar usuario logueado
            printf("\241Acceso concedido!\n"); // ¡ (¡ = ASCII 241
            return 1;
        } else {
            printf("\241Acceso denegado! Usuario o contrase\244a incorrectos.\n");
        }

        printf("\277Desea intentarlo de nuevo? (s/n): "); // ¿ (¿ = ASCII 277)
        char opcion;
        scanf(" %c", &opcion);
        if (opcion == 'n' || opcion == 'N') {
            return 0;
        }
    }
}



// Función para cerrar sesión
void cerrar_sesion() {
    printf("\nCerrando sesi\242n...\n");
    usuario_actual[0] = '\0'; // Borra el usuario actual
}

