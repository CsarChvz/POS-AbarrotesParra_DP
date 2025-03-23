// seguridad.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/seguridad.h"

#define MAX_USERS 3
#define USUARIO_LENGTH 20
#define CONTRASENA_LENGTH 20

// Estructura de Usuario
typedef struct {
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];
    int rol;
    int id; // añadimos id
} Usuario;

// Lista de usuarios - Seteados para debugear
Usuario usuarios[MAX_USERS] = {
    {"admin", "admin123", 2, 1}, // añadimos id
    {"user1", "contrasena1", 1, 2}, // añadimos id
    {"user2", "contrasena2", 1, 3} // añadimos id
};

// Variable global para almacenar el usuario logueado
char usuario_actual[USUARIO_LENGTH] = "";
int usuario_id = 0; // Definición de usuario_id

int checar_credenciales(const char *usuario, const char *contrasena) {
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 && strcmp(usuarios[i].contrasena, contrasena) == 0) {
            usuario_id = usuarios[i].id; // Rellenar usuario_id
            return 1;
        }
    }
    return 0;
}

int obtener_rol(const char *usuario) {
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0) {
            return usuarios[i].rol;
        }
    }
    return -1;
}

int inicio_sesion() {
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];

    while (1) {
        printf("\n--- Inicio de Sesión ---\n");
        printf("Ingrese el nombre de usuario: ");
        scanf("%19s", usuario);

        printf("Ingrese la contraseña: ");
        scanf("%19s", contrasena);

        if (checar_credenciales(usuario, contrasena)) {
            strcpy(usuario_actual, usuario);
            printf("¡Acceso concedido!\n");
            return 1;
        } else {
            printf("¡Acceso denegado! Usuario o contraseña incorrectos.\n");
        }

        printf("¿Desea intentarlo de nuevo? (s/n): ");
        char opcion;
        scanf(" %c", &opcion);
        if (opcion == 'n' || opcion == 'N') {
            return 0;
        }
    }
}

void cambiar_contrasena() {
    int i;
    char nueva_contrasena[CONTRASENA_LENGTH];
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(usuarios[i].usuario, usuario_actual) == 0) {
            printf("\nIngrese la nueva contraseña: ");
            scanf("%s", &nueva_contrasena);
            strcpy(usuarios[i].contrasena, nueva_contrasena);
            printf("Contraseña cambiada exitosamente.\n");
            cerrar_sesion();
            return;
        }
    }
    printf("Error: Usuario no encontrado.\n");
}

void cerrar_sesion() {
    printf("\nCerrando sesión...\n");
    usuario_actual[0] = '\0';
    usuario_id = 0; // Limpiar usuario_id
}