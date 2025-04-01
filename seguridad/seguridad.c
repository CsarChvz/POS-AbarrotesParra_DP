#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/seguridad.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

#define USUARIO_LENGTH 20
#define CONTRASENA_LENGTH 20
#define MAX_LINEA 100
#define ARCHIVO_USUARIOS "common/data/usuarios.csv"

typedef struct {
    int id;
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];
    int rol;
    int activo;
} Usuario;

UsuarioGlobal usuario_global;

int leer_usuarios(Usuario **usuarios, int *num_usuarios) {
    FILE *archivo = fopen(ARCHIVO_USUARIOS, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de usuarios");
        printf("Ruta del archivo: %s\n", ARCHIVO_USUARIOS);
        return 0;
    }

    char linea[MAX_LINEA];
    int count = 0;

    while (fgets(linea, MAX_LINEA, archivo) != NULL) {
        count++;
    }

    rewind(archivo);

    *usuarios = (Usuario *)malloc((count - 1) * sizeof(Usuario));
    if (*usuarios == NULL) {
        fclose(archivo);
        perror("Error al asignar memoria para usuarios");
        return 0;
    }

    fgets(linea, MAX_LINEA, archivo); // Saltar la cabecera

    int i = 0;
    while (fgets(linea, MAX_LINEA, archivo) != NULL) {
        if (sscanf(linea, "%d,%19[^,],%19[^,],%d,%d",
                    &(*usuarios)[i].id, (*usuarios)[i].usuario, (*usuarios)[i].contrasena,
                    &(*usuarios)[i].rol, &(*usuarios)[i].activo) == 5) {
            i++;
        }
    }

    fclose(archivo);
    *num_usuarios = i;
    return 1;
}

int checar_credenciales(const char *usuario, const char *contrasena) {
    Usuario *usuarios = NULL;
    int num_usuarios = 0;
    int i;
    if (!leer_usuarios(&usuarios, &num_usuarios)) {
        printf("Error al leer usuarios en checar_credenciales.\n");
        return 0;
    }

    for (i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 && strcmp(usuarios[i].contrasena, contrasena) == 0 && usuarios[i].activo == 1) { //solo usuarios activos
            usuario_global.id = usuarios[i].id;
            free(usuarios);
            registrarRegistroAuditoria(usuario_global.id, "INICIO_SESION", "Inicio de sesión", "Usuario", usuario_global.id, "Inicio de sesión exitoso", "Autenticación", "Éxito");
            return 1;
        }
    }
    free(usuarios);
    return 0;
}

int obtener_rol(const char *usuario) {
    Usuario *usuarios = NULL;
    int num_usuarios = 0;
    int i;
    if (!leer_usuarios(&usuarios, &num_usuarios)) {
        return -1;
    }

    for (i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0) {
            int rol = usuarios[i].rol;
            free(usuarios);
            return rol;
        }
    }
    free(usuarios);
    return -1;
}

int inicio_sesion() {
    char usuario[USUARIO_LENGTH];
    char contrasena[CONTRASENA_LENGTH];

    while (1) {
        printf("\n--- Inicio de Sesi%cn ---\n", 162);
        printf("Ingrese el nombre de usuario: ");
        scanf("%19s", usuario);

        printf("Ingrese la contrase%ca: ", 164);
        scanf("%19s", contrasena);

        if (checar_credenciales(usuario, contrasena)) {
            strcpy(usuario_global.usuario, usuario);
            printf("¡Acceso concedido!\n");
            return 1;
        } else {
            printf("¡Acceso denegado! Usuario o contrase%ca incorrectos.\n", 164);
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
    Usuario *usuarios = NULL;
    int num_usuarios = 0;
    char nueva_contrasena[CONTRASENA_LENGTH];
    int i, j;
    if (!leer_usuarios(&usuarios, &num_usuarios)) {
        printf("Error al leer usuarios.\n");
        return;
    }

    for (i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario_global.usuario) == 0) {
            printf("\nIngrese la nueva contrase%ca: ", 164);
            scanf("%19s", nueva_contrasena);
            strcpy(usuarios[i].contrasena, nueva_contrasena);

            FILE *archivo = fopen(ARCHIVO_USUARIOS, "w");
            if (archivo == NULL) {
                perror("Error al abrir el archivo de usuarios para escritura");
                free(usuarios);
                return;
            }

            fprintf(archivo, "id,usuario,contrasena,rol,activo\n");
            for (j = 0; j < num_usuarios; j++) {
                fprintf(archivo, "%d,%s,%s,%d,%d\n", usuarios[j].id, usuarios[j].usuario, usuarios[j].contrasena, usuarios[j].rol, usuarios[j].activo);
            }
            fclose(archivo);

            printf("Contrase%ca cambiada exitosamente.\n", 164);
            free(usuarios);
            registrarRegistroAuditoria(usuario_global.id, "CAMBIAR_CONTRASENA", "Cambiar contraseña", "Usuario", usuario_global.id, "Contrase%ca cambiada", "Modificacion", "Exito");
            return;
        }
    }
    printf("Error: Usuario no encontrado.\n");
    free(usuarios);
}

void cerrar_sesion() {
    printf("\nCerrando sesi%cn...\n", 162);

    registrarRegistroAuditoria(usuario_global.id, "CERRAR_SESION", "Cerrar sesiocn", "Usuario", usuario_global.id, "Sesión cerrada", "Autenticaciocn", "Exito");
    usuario_global.id = 0;
    memset(usuario_global.usuario, 0, sizeof(usuario_global.usuario));
    usuario_global.rol = 0;
    usuario_global.activo = 0;
}