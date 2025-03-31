#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h" 

#define ARCHIVO_CORTES "common/data/corte_cajas.csv"

#define OBSERVACIONES_LENGTH 256

#define FECHA_LENGTH 11
#define HORA_LENGTH 9


typedef struct {
    int idCorte;
    int idCaja;
    char fecha[FECHA_LENGTH];
    char hora[HORA_LENGTH];
    float ingresosTotales;
    float egresosTotales;
    float saldoFinal;
    float montoDeclarado;
    float diferencia;
    char observaciones[OBSERVACIONES_LENGTH];
} CorteCaja;


int obtenerUltimoCorteCaja(CorteCaja *ultimoCorte) {
    FILE *archivo = fopen(ARCHIVO_CORTES, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cortes_caja.csv");
        return 0; // Error al abrir el archivo
    }

    CorteCaja corteActual;
    int corteEncontrado = 0;
    char linea[512]; // Aumentamos el tamaño de la línea para las observaciones

    // Saltar la cabecera
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (sscanf(linea, "%d,%d,%10[^,],%8[^,],%f,%f,%f,%f,%f,%255[^\n]",
                   &corteActual.idCorte, &corteActual.idCaja, corteActual.fecha, corteActual.hora,
                   &corteActual.ingresosTotales, &corteActual.egresosTotales, &corteActual.saldoFinal,
                   &corteActual.montoDeclarado, &corteActual.diferencia, corteActual.observaciones) == 10) {
            *ultimoCorte = corteActual;
            corteEncontrado = 1;
        }
    }

    fclose(archivo);
    return corteEncontrado;
}

void mostrarUltimoCorteCaja() {
    CorteCaja ultimoCorte;
    if (obtenerUltimoCorteCaja(&ultimoCorte)) {
        printf("\n--- Último Corte de Caja ---\n");
        printf("ID\tCaja\tFecha\t\tHora\tIngresos\tEgresos\tSaldo\tDeclarado\tDiferencia\tObservaciones\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        printf("%d\t%d\t%s\t%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%s\n",
               ultimoCorte.idCorte, ultimoCorte.idCaja, ultimoCorte.fecha, ultimoCorte.hora,
               ultimoCorte.ingresosTotales, ultimoCorte.egresosTotales, ultimoCorte.saldoFinal,
               ultimoCorte.montoDeclarado, ultimoCorte.diferencia, ultimoCorte.observaciones);
    } else {
        printf("No se encontraron cortes de caja.\n");
    }
}