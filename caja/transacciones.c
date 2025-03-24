#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h"

#define ARCHIVO_TRANSACCIONES "common/data/transacciones.csv"

typedef struct {
    int idTransaccion;
    char fecha[11]; // YYYY-MM-DD
    char tipoMovimiento[10]; // Ingreso o Egreso
    char categoriaTransaccion[20]; // Venta, Compra, etc.
    float ingreso;
    float egreso;
    float saldo;
    char metodoPago[20];
    int idUsuario;
    char observaciones[100];
    char corteCaja[4]; // "Sí" o "No"
    char inicioJornada[4]; // "Sí" o "No"
} Transaccion;

int obtenerSiguienteIdTransaccion() {
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "r");
    if (archivo == NULL) {
        return 1; // Si el archivo no existe, el primer ID es 1
    }

    int maxId = 0;
    char linea[256];

    // Saltar la cabecera
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo)) {
        Transaccion t;
        if (sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                   &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                   &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                   t.observaciones, t.corteCaja, t.inicioJornada) == 12) {
            if (t.idTransaccion > maxId) {
                maxId = t.idTransaccion;
            }
        }
    }

    fclose(archivo);
    return maxId + 1;
}

void registrarIngreso() {

    Transaccion transaccion;
    transaccion.idTransaccion = obtenerSiguienteIdTransaccion();
    transaccion.idUsuario = usuario_id;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(transaccion.fecha, sizeof(transaccion.fecha), "%Y-%m-%d", tiempoLocal);

    strcpy(transaccion.tipoMovimiento, "Ingreso");

    printf("Categoría de ingreso (Venta, Depósito, etc.): ");
    scanf("%s", transaccion.categoriaTransaccion);

    printf("Monto del ingreso: ");
    scanf("%f", &transaccion.ingreso);
    transaccion.egreso = 0.0;

    printf("Método de pago: ");
    scanf("%s", transaccion.metodoPago);

    printf("Observaciones: ");
    scanf(" %[^\n]", transaccion.observaciones);

    printf("¿Corte de caja? (Sí/No): ");
    scanf("%s", transaccion.corteCaja);

    printf("¿Inicio de jornada? (Sí/No): ");
    scanf("%s", transaccion.inicioJornada);

    // Calcular el saldo
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "r");
    float saldoAnterior = 0.0;
    if (archivo != NULL) {
        char linea[256];
        // Saltar la cabecera
        fgets(linea, sizeof(linea), archivo);
        while (fgets(linea, sizeof(linea), archivo)) {
            Transaccion t;
            if (sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                       &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                       &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                       t.observaciones, t.corteCaja, t.inicioJornada) == 12) {
                saldoAnterior = t.saldo;
            }
        }
        fclose(archivo);
    }

    transaccion.saldo = saldoAnterior + transaccion.ingreso;

    // Escribir la transacción en el archivo
    archivo = fopen(ARCHIVO_TRANSACCIONES, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de transacciones.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%s,%.2f,%.2f,%.2f,%s,%d,%s,%s,%s\n",
            transaccion.idTransaccion, transaccion.fecha, transaccion.tipoMovimiento,
            transaccion.categoriaTransaccion, transaccion.ingreso, transaccion.egreso,
            transaccion.saldo, transaccion.metodoPago, transaccion.idUsuario,
            transaccion.observaciones, transaccion.corteCaja, transaccion.inicioJornada);

    fclose(archivo);

    printf("Ingreso registrado con éxito.\n");
}

void registrarEgreso() {
    Transaccion transaccion;
    transaccion.idTransaccion = obtenerSiguienteIdTransaccion();
    transaccion.idUsuario = usuario_id;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(transaccion.fecha, sizeof(transaccion.fecha), "%Y-%m-%d", tiempoLocal);

    strcpy(transaccion.tipoMovimiento, "Egreso");

    printf("Categoría de egreso (Compra, Pago, etc.): ");
    scanf("%s", transaccion.categoriaTransaccion);

    printf("Monto del egreso: ");
    scanf("%f", &transaccion.egreso);
    transaccion.ingreso = 0.0;

    printf("Método de pago: ");
    scanf("%s", transaccion.metodoPago);

    printf("Observaciones: ");
    scanf(" %[^\n]", transaccion.observaciones);

    printf("¿Corte de caja? (Sí/No): ");
    scanf("%s", transaccion.corteCaja);

    printf("¿Inicio de jornada? (Sí/No): ");
    scanf("%s", transaccion.inicioJornada);

    // Calcular el saldo
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "r");
    float saldoAnterior = 0.0;
    if (archivo != NULL) {
        char linea[256];
        // Saltar la cabecera
        fgets(linea, sizeof(linea), archivo);
        while (fgets(linea, sizeof(linea), archivo)) {
            Transaccion t;
            if (sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                       &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                       &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                       t.observaciones, t.corteCaja, t.inicioJornada) == 12) {
                saldoAnterior = t.saldo;
            }
        }
        fclose(archivo);
    }

    transaccion.saldo = saldoAnterior - transaccion.egreso;

    // Escribir la transacción en el archivo
    archivo = fopen(ARCHIVO_TRANSACCIONES, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de transacciones.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%s,%.2f,%.2f,%.2f,%s,%d,%s,%s,%s\n",
            transaccion.idTransaccion, transaccion.fecha, transaccion.tipoMovimiento,
            transaccion.categoriaTransaccion, transaccion.ingreso, transaccion.egreso,
            transaccion.saldo, transaccion.metodoPago, transaccion.idUsuario,
            transaccion.observaciones, transaccion.corteCaja, transaccion.inicioJornada);
    fclose(archivo);

    printf("Egreso registrado con éxito.\n");
}