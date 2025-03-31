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