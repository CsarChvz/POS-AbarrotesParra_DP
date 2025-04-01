#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/seguridad.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

#define MAX_USERS 100
#define USUARIO_LENGTH_STATIC 50
#define CONTRASENA_LENGTH 50

typedef struct {
    int id;
    char usuario[USUARIO_LENGTH_STATIC];
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
    registrarRegistroAuditoria(usuario_global.id, "LISTAR_USUARIOS", "Lista de usuarios", "Usuario", 0, "Lista de usuarios mostrada", "Informativo", "Éxito");
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

// Función para guardar un nuevo usuario en el archivo usuarios.csv
int guardarUsuario(const char *nombre_usuario, int role) {
    // Validación del nombre de usuario
    if (nombre_usuario == NULL || strlen(nombre_usuario) == 0) {
        printf("Error: El nombre de usuario es inválido.\n");
        return 0; // Error en nombre de usuario
    }

    // Verifica si el nombre de usuario ya existe
    if (usuarioExiste(nombre_usuario)) {
        printf("Error: El nombre de usuario '%s' ya existe.\n", nombre_usuario);
        return 0; // Error: nombre de usuario duplicado
    }

    // Abre el archivo en modo 'append' (agregar al final)
    FILE *archivo = fopen("common/data/usuarios.csv", "a");
    if (archivo == NULL) {
        printf("Error: ");
        perror("Error al abrir el archivo"); // Mejor manejo de errores
        return 0; // Error al abrir el archivo
    }

    int id = obtenerSiguienteId(); // Obtener el siguiente ID (suponiendo que esta función está definida correctamente)

    // Escribir los datos del usuario en el archivo CSV
    if (fprintf(archivo, "%d,%s,%s,%d,%d\n", id, nombre_usuario, "contrasena", role, 1) < 0) {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return 0; // Error al escribir en el archivo
    }

    fclose(archivo);
    registrarRegistroAuditoria(usuario_global.id, "GUARDAR_USUARIO", "Guardar usuario", "Usuario", id, "Usuario guardado", "Informativo", "Éxito");
    return 1; // Éxito
}

// Función para verificar si un nombre de usuario ya existe
int usuarioExiste(const char *nombre_usuario) {
    FILE *archivo = fopen("common/data/usuarios.csv", "r");
    if (archivo == NULL) {
        return 0; // Si no se puede abrir el archivo, asumimos que el usuario no existe
    }

    char linea[256];
    char usuario_actual[20]; // Ajusta el tamaño según tu definición de USUARIO_LENGTH

    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    while (fgets(linea, sizeof(linea), archivo)) {
        if (sscanf(linea, "%*d,%19[^,],%*[^,],%*d,%*d", usuario_actual) == 1) {
            if (strcmp(usuario_actual, nombre_usuario) == 0) {
                fclose(archivo);
                return 1; // El usuario ya existe
            }
        }
    }

    fclose(archivo);
    return 0; // El usuario no existe
}

int buscarUsuario(int id, Usuario *resultado) {
    FILE *archivo = fopen("common/data/usuarios.csv", "r"); // Modo lectura
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 0; // Error al abrir el archivo
    }

    char linea[256];
    int encontrado = 0;

    // Saltar la cabecera
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
        return 0; // Archivo vacío
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Usuario u;
        if (sscanf(linea, "%d,%49[^,],%49[^,],%d,%d",
                   &u.id, u.usuario, u.contrasena, &u.rol, &u.activo) == 5) { // Verifica que haya 5 campos
            if (u.id == id) {
                *resultado = u;
                encontrado = 1;
                break;
            }
        }
    }

    fclose(archivo);
    return encontrado;
}

int cambiarEstatusUsuario(int idUsuario) {
    FILE *archivoOriginal = fopen("common/data/usuarios.csv", "r");
    FILE *archivoTemporal = fopen("common/data/usuarios_temp.csv", "w");

    if (!archivoOriginal || !archivoTemporal) {
        perror("Error al abrir los archivos");
        if (archivoOriginal) fclose(archivoOriginal);
        if (archivoTemporal) fclose(archivoTemporal);
        return 0;
    }

    char linea[256];
    int encontrado = 0;

    // Copiar la cabecera
    if (fgets(linea, sizeof(linea), archivoOriginal)) {
        fputs(linea, archivoTemporal);
    }

    // Leer y modificar líneas
    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        Usuario u;
        if (sscanf(linea, "%d,%49[^,],%49[^,],%d,%d",
                   &u.id, u.usuario, u.contrasena, &u.rol, &u.activo) == 5) {
            if (u.id == idUsuario) {
                u.activo = !u.activo; // Cambiar estado
                fprintf(archivoTemporal, "%d,%s,%s,%d,%d\n",
                        u.id, u.usuario, u.contrasena, u.rol, u.activo);
                encontrado = 1;
                printf("\n[Estatus actualizado] Usuario: '%s' -> %s\n",
                       u.usuario, u.activo ? "Activo" : "Inactivo");
            } else {
                fputs(linea, archivoTemporal);
            }
        } else {
            printf("Advertencia: Línea corrupta en el archivo.\n");
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/usuarios_temp.csv");
        printf("Error: No se encontró el usuario con ID %d.\n", idUsuario);
        return 0;
    }

    if (remove("common/data/usuarios.csv") != 0) {
        perror("Error al eliminar el archivo original");
        return 0;
    }

    if (rename("common/data/usuarios_temp.csv", "common/data/usuarios.csv") != 0) {
        perror("Error al renombrar el archivo temporal");
        return 0;
    }
    registrarRegistroAuditoria(usuario_global.id, "CAMBIAR_ESTATUS_USUARIO", "Cambiar estatus de usuario", "Usuario", idUsuario, "Estatus de usuario cambiado", "Modificación", "Éxito");
    return 1;
}

void cambiarEstatusUsuarioMenu() {
    int id;
    listarUsuarios();
    printf("----------------------------------------");
    printf("\n--- Cambiar Estatus de Usuario ---\n");
    printf("Ingrese el ID del usuario: ");

    if (scanf("%d", &id) != 1) {
        printf("Error: Entrada inválida.\n");
        return;
    }

    if (cambiarEstatusUsuario(id)) {
        printf("El estatus del usuario con ID %d ha sido actualizado correctamente.\n", id);
    } else {
        printf("No se pudo actualizar el estatus del usuario.\n");
    }
}

int cambiarRolUsuario(int idUsuario) {
    FILE *archivoOriginal = fopen("common/data/usuarios.csv", "r");
    FILE *archivoTemporal = fopen("common/data/usuarios_temp.csv", "w");

    if (!archivoOriginal || !archivoTemporal) {
        perror("Error al abrir los archivos");
        if (archivoOriginal) fclose(archivoOriginal);
        if (archivoTemporal) fclose(archivoTemporal);
        return 0;
    }

    char linea[256];
    int encontrado = 0;

    // Copiar la cabecera
    if (fgets(linea, sizeof(linea), archivoOriginal)) {
        fputs(linea, archivoTemporal);
    }

    // Leer y modificar líneas
    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        Usuario u;
        if (sscanf(linea, "%d,%49[^,],%49[^,],%d,%d",
                   &u.id, u.usuario, u.contrasena, &u.rol, &u.activo) == 5) {
            if (u.id == idUsuario) {
                u.rol = (u.rol == 1) ? 2 : 1; // Alternar entre 1 (vendedor) y 2 (admin)
                fprintf(archivoTemporal, "%d,%s,%s,%d,%d\n",
                        u.id, u.usuario, u.contrasena, u.rol, u.activo);
                encontrado = 1;
                printf("\n[Rol actualizado] Usuario: '%s' -> Nuevo Rol: %s\n",
                       u.usuario, u.rol == 1 ? "Vendedor" : "Admin");
            } else {
                fputs(linea, archivoTemporal);
            }
        } else {
            printf("Advertencia: Línea corrupta en el archivo.\n");
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/usuarios_temp.csv");
        printf("Error: No se encontró el usuario con ID %d.\n", idUsuario);
        return 0;
    }

    if (remove("common/data/usuarios.csv") != 0) {
        perror("Error al eliminar el archivo original");
        return 0;
    }

    if (rename("common/data/usuarios_temp.csv", "common/data/usuarios.csv") != 0) {
        perror("Error al renombrar el archivo temporal");
        return 0;
    }
    registrarRegistroAuditoria(usuario_global.id, "CAMBIAR_ROL_USUARIO", "Cambiar rol de usuario", "Usuario", idUsuario, "Rol de usuario cambiado", "Modificación", "Éxito");
    return 1;
}

void cambiarRolUsuarioMenu() {
    int id;
    listarUsuarios();
    printf("----------------------------------------");
    printf("\n--- Cambiar Rol de Usuario ---\n");
    printf("Ingrese el ID del usuario: ");

    if (scanf("%d", &id) != 1) {
        printf("Error: Entrada inválida.\n");
        return;
    }

    if (cambiarRolUsuario(id)) {
        printf("El rol del usuario con ID %d ha sido actualizado correctamente.\n", id);
    } else {
        printf("No se pudo actualizar el rol del usuario.\n");
    }
}