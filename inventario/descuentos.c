#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h"

#define ARCHIVO_DESCUENTOS "common/data/descuentos.csv"
#define FECHA_LENGTH 11

typedef struct {
    int idDescuento;
    int idProducto;
    int porcentajeDescuento;
    char fechaInicio[FECHA_LENGTH];
    char fechaFin[FECHA_LENGTH];
} Descuento;

int obtenerSiguienteIdDescuento() {
    FILE *archivo = fopen(ARCHIVO_DESCUENTOS, "r");
    if (archivo == NULL) {
        return 1;
    }

    int maxId = 0;
    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        int id;
        sscanf(linea, "%d", &id);
        if (id > maxId) {
            maxId = id;
        }
    }

    fclose(archivo);
    return maxId + 1;
}

void aplicarDescuentoProducto() {
    Descuento descuento;
    descuento.idDescuento = obtenerSiguienteIdDescuento();

    printf("\n--- Aplicar Descuento a Producto ---\n");
    printf("ID Producto: ");
    scanf("%d", &descuento.idProducto);
    printf("Porcentaje de Descuento: ");
    scanf("%d", &descuento.porcentajeDescuento);
    printf("Fecha de Inicio (YYYY-MM-DD): ");
    scanf("%s", descuento.fechaInicio);
    printf("Fecha de Fin (YYYY-MM-DD): ");
    scanf("%s", descuento.fechaFin);

    FILE *archivo = fopen(ARCHIVO_DESCUENTOS, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de descuentos.\n");
        return;
    }

    fprintf(archivo, "%d,%d,%d,%s,%s\n",
            descuento.idDescuento, descuento.idProducto, descuento.porcentajeDescuento,
            descuento.fechaInicio, descuento.fechaFin);

    fclose(archivo);
    printf("Descuento aplicado con éxito.\n");
}

void configurarFechasDescuento() {
    int idDescuento;
    char fechaInicio[FECHA_LENGTH], fechaFin[FECHA_LENGTH];

    printf("\n--- Configurar Fechas de Descuento ---\n");
    printf("ID Descuento: ");
    scanf("%d", &idDescuento);
    printf("Nueva Fecha de Inicio (YYYY-MM-DD): ");
    scanf("%s", fechaInicio);
    printf("Nueva Fecha de Fin (YYYY-MM-DD): ");
    scanf("%s", fechaFin);

    FILE *archivoOriginal = fopen(ARCHIVO_DESCUENTOS, "r");
    FILE *archivoTemporal = fopen("common/data/descuentos_temp.csv", "w");

    if (!archivoOriginal || !archivoTemporal) {
        perror("Error al abrir los archivos");
        if (archivoOriginal) fclose(archivoOriginal);
        if (archivoTemporal) fclose(archivoTemporal);
        return;
    }

    char linea[256];
    int encontrado = 0;

    fprintf(archivoTemporal, "ID_Descuento,ID_Producto,Porcentaje_Descuento,Fecha_Inicio,Fecha_Fin\n");

    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        Descuento descuento;
        if (sscanf(linea, "%d,%d,%d,%10[^,],%10[^\n]",
                   &descuento.idDescuento, &descuento.idProducto, &descuento.porcentajeDescuento,
                   descuento.fechaInicio, descuento.fechaFin) == 5) {
            if (descuento.idDescuento == idDescuento) {
                strcpy(descuento.fechaInicio, fechaInicio);
                strcpy(descuento.fechaFin, fechaFin);
                encontrado = 1;
            }
            fprintf(archivoTemporal, "%d,%d,%d,%s,%s\n",
                    descuento.idDescuento, descuento.idProducto, descuento.porcentajeDescuento,
                    descuento.fechaInicio, descuento.fechaFin);
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/descuentos_temp.csv");
        printf("Descuento no encontrado.\n");
        return;
    }

    remove(ARCHIVO_DESCUENTOS);
    rename("common/data/descuentos_temp.csv", ARCHIVO_DESCUENTOS);

    printf("Fechas de descuento actualizadas con éxito.\n");
}

void verProductosConDescuento() {
    FILE *archivo = fopen(ARCHIVO_DESCUENTOS, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de descuentos.\n");
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    printf("\n--- Productos con Descuento ---\n");
    printf("%-15s%-15s%-20s%-20s\n", "ID Producto", "Porcentaje", "Fecha Inicio", "Fecha Fin");
    printf("------------------------------------------------------------------\n");

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Descuento descuento;
        if (sscanf(linea, "%d,%d,%d,%10[^,],%10[^\n]",
                   &descuento.idDescuento, &descuento.idProducto, &descuento.porcentajeDescuento,
                   descuento.fechaInicio, descuento.fechaFin) == 5) {
            printf("%-15d%-15d%-20s%-20s\n",
                   descuento.idProducto, descuento.porcentajeDescuento,
                   descuento.fechaInicio, descuento.fechaFin);
        }
    }

    fclose(archivo);
}

