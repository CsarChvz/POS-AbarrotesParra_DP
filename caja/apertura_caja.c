#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h" 

#define ARCHIVO_CAJA "common/data/cajas.csv"

#define FECHA_LENGTH 11
#define HORA_LENGTH 9

typedef struct {
    int idCaja;
    int idUsuario;
    char fecha[FECHA_LENGTH];
    char hora[HORA_LENGTH];
    float montoInicial;
    char estado[10];
} Caja;

// Función para obtener la fecha y hora actual en formato AAAA-MM-DD HH:MM:SS
void obtenerFechaHoraActual(char *fecha, char *hora) {
    time_t tiempoActual;
    struct tm *tiempoLocal;
    time(&tiempoActual);
    tiempoLocal = localtime(&tiempoActual);

    strftime(fecha, 11, "%Y-%m-%d", tiempoLocal);
    strftime(hora, 9, "%H:%M:%S", tiempoLocal);
}

// Función para obtener la fecha actual en formato AAAA-MM-DD
void obtenerFechaActual(char *fechaActual) {
    time_t tiempoActual;
    struct tm *tiempoLocal;
    time(&tiempoActual);
    tiempoLocal = localtime(&tiempoActual);

    strftime(fechaActual, 11, "%Y-%m-%d", tiempoLocal);
}
// Función para obtener el siguiente ID de caja disponible
int obtenerSiguienteIdCaja() {
    FILE *archivo = fopen(ARCHIVO_CAJA, "r");
    if (archivo == NULL) {
        return 1; // Si no existe el archivo, el primer ID es 1
    }

    char linea[256];
    int maxId = 0;
    int id;

    // Saltar la cabecera
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

// Función para aperturar una nueva caja
int aperturarCaja(int idUsuario, float montoInicial) {
    FILE *archivo = fopen(ARCHIVO_CAJA, "a");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cajas.csv");
        return 0; // Error al abrir el archivo
    }

    int idCaja = obtenerSiguienteIdCaja();
    char fecha[11], hora[9];
    obtenerFechaHoraActual(fecha, hora);

    if (fprintf(archivo, "%d,%d,%s,%s,%.2f,%s\n", idCaja, idUsuario, fecha, hora, montoInicial, "Abierta") < 0) {
        perror("Error al escribir en el archivo cajas.csv");
        fclose(archivo);
        return 0; // Error al escribir en el archivo
    }

    fclose(archivo);
    return 1; // Éxito
}

void menuAperturaCaja() {
    int idUsuario;
    float montoInicial;

    idUsuario = usuario_global.id; // Asignación, no declaración

    printf("Ingrese el monto inicial de la caja: ");
    scanf("%f", &montoInicial);

    if (aperturarCaja(idUsuario, montoInicial)) {
        printf("Caja aperturada exitosamente.\n");
    } else {
        printf("Error al aperturar la caja.\n");
    }
}

// Función para verificar el estado de la caja del día actual
int verificarEstadoCajaDiaActual(int idUsuario, Caja *cajaActual) {
    FILE *archivo = fopen(ARCHIVO_CAJA, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cajas.csv");
        return -1; // Error al abrir el archivo
    }

    char fechaActual[11];
    obtenerFechaActual(fechaActual);

    char linea[256];
    int cajaEncontrada = 0;

    // Saltar la cabecera
    fgets(linea, 256, archivo);

    while (fgets(linea, 256, archivo) != NULL) {
        Caja caja;
        if (sscanf(linea, "%d,%d,%10[^,],%8[^,],%f,%9s",
                   &caja.idCaja, &caja.idUsuario, caja.fecha, caja.hora, &caja.montoInicial, caja.estado) == 6) {
            if (caja.idUsuario == idUsuario && strcmp(caja.fecha, fechaActual) == 0) {
                *cajaActual = caja;
                cajaEncontrada = 1;
                break; // Caja encontrada, salir del bucle
            }
        }
    }

    fclose(archivo);
    return cajaEncontrada;
}

// Función para mostrar el estado de la caja o preguntar por la apertura
void mostrarEstadoCajaODarOpcionApertura() {
    int idUsuario;
    Caja cajaActual;
    idUsuario = usuario_global.id;
    int cajaEncontrada = verificarEstadoCajaDiaActual(idUsuario, &cajaActual);

    if (cajaEncontrada == 1) {
        printf("Caja del día actual:\n");
        printf("ID Caja: %d\n", cajaActual.idCaja);
        printf("Fecha: %s\n", cajaActual.fecha);
        printf("Hora: %s\n", cajaActual.hora);
        printf("Monto Inicial: %.2f\n", cajaActual.montoInicial);
        printf("Estado: %s\n", cajaActual.estado);
    } else if (cajaEncontrada == 0) {
        printf("No se encontró una caja abierta para el día actual.\n");
        char opcion;
        printf("¿Desea aperturar una nueva caja? (s/n): ");
        scanf(" %c", &opcion);

        if (opcion == 's' || opcion == 'S') {
            float montoInicial;
            printf("Ingrese el monto inicial de la caja: ");
            scanf("%f", &montoInicial);

            if (aperturarCaja(idUsuario, montoInicial)) {
                printf("Caja aperturada exitosamente.\n");
            } else {
                printf("Error al aperturar la caja.\n");
            }
        }
    } else {
        printf("Error al verificar el estado de la caja.\n");
    }
}