#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h" 

#define ARCHIVO_CORTES "common/data/corte_cajas.csv"
#define ARCHIVO_CAJAS "common/data/cajas.csv"
#define ARCHIVO_TRANSACCIONES "common/data/transacciones.csv"

#define OBSERVACIONES_LENGTH 256

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

int obtenerHistorialCortesCaja(CorteCaja **cortes) {
    FILE *archivo = fopen(ARCHIVO_CORTES, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cortes_caja.csv");
        return 0; // Error al abrir el archivo
    }

    int cantidadCortes = 0;
    char linea[512]; // Aumentamos el tamaño de la línea para las observaciones

    // Contar la cantidad de cortes en el archivo
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        cantidadCortes++;
    }

    // Volver al inicio del archivo
    fseek(archivo, 0, SEEK_SET);
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera nuevamente

    // Reservar memoria para los cortes
    *cortes = (CorteCaja *)malloc(cantidadCortes * sizeof(CorteCaja));
    if (*cortes == NULL) {
        perror("Error al asignar memoria para los cortes");
        fclose(archivo);
        return 0; // Error al asignar memoria
    }

    // Leer los cortes desde el archivo
    int i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (sscanf(linea, "%d,%d,%10[^,],%8[^,],%f,%f,%f,%f,%f,%255[^\n]",
                   &(*cortes)[i].idCorte, &(*cortes)[i].idCaja, (*cortes)[i].fecha, (*cortes)[i].hora,
                   &(*cortes)[i].ingresosTotales, &(*cortes)[i].egresosTotales, &(*cortes)[i].saldoFinal,
                   &(*cortes)[i].montoDeclarado, &(*cortes)[i].diferencia, (*cortes)[i].observaciones) == 10) {
            i++;
        }
    }

    fclose(archivo);
    return cantidadCortes;
}

void mostrarHistorialCortesCaja() {
    CorteCaja *cortes = NULL;
    int cantidadCortes = obtenerHistorialCortesCaja(&cortes);
    int i;
    if (cantidadCortes > 0) {
        printf("\n%-5s%-6s%-12s%-8s%-10s%-10s%-10s%-10s%-10s%-20s\n",
               "ID", "Caja", "Fecha", "Hora", "Ingresos", "Egresos", "Saldo", "Declarado", "Diferencia", "Observaciones");
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < cantidadCortes; i++) {
            printf("%-5d%-6d%-12s%-8s%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f%-20s\n",
                   cortes[i].idCorte, cortes[i].idCaja, cortes[i].fecha, cortes[i].hora,
                   cortes[i].ingresosTotales, cortes[i].egresosTotales, cortes[i].saldoFinal,
                   cortes[i].montoDeclarado, cortes[i].diferencia, cortes[i].observaciones);
        }
        free(cortes);
    } else {
        printf("No se encontraron cortes de caja.\n");
    }
}
int obtenerSiguienteIdCorteCaja() {
    FILE *archivo = fopen(ARCHIVO_CORTES, "r");
    if (archivo == NULL) {
        return 1;
    }

    int maxId = 0;
    char linea[512];
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

int obtenerCajaAbierta(Caja *cajaAbierta) {
    FILE *archivo = fopen(ARCHIVO_CAJAS, "r");
    if (archivo == NULL) {
        return 0;
    }

    Caja cajaActual;
    int cajaEncontrada = 0;
    char linea[256];

    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (sscanf(linea, "%d,%d,%10[^,],%8[^,],%f,%9[^\n]",
                   &cajaActual.idCaja, &cajaActual.idUsuario, cajaActual.fecha, cajaActual.hora,
                   &cajaActual.montoInicial, cajaActual.estado) == 6) {
            if (strcmp(cajaActual.estado, "Abierta") == 0) {
                *cajaAbierta = cajaActual;
                cajaEncontrada = 1;
                break;
            }
        }
    }

    fclose(archivo);
    return cajaEncontrada;
}

int actualizarEstadoCaja(int idCaja, const char *estado) {
    FILE *archivoOriginal = fopen(ARCHIVO_CAJAS, "r");
    FILE *archivoTemporal = fopen("common/data/cajas_temp.csv", "w");

    if (!archivoOriginal || !archivoTemporal) {
        perror("Error al abrir los archivos");
        if (archivoOriginal) fclose(archivoOriginal);
        if (archivoTemporal) fclose(archivoTemporal);
        return 0;
    }

    char linea[256];
    int encontrado = 0;

    fprintf(archivoTemporal, "ID_Caja,ID_Usuario,Fecha,Hora,Monto_Inicial,Estado\n");

    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        Caja caja;
        if (sscanf(linea, "%d,%d,%10[^,],%8[^,],%f,%9[^\n]",
                   &caja.idCaja, &caja.idUsuario, caja.fecha, caja.hora,
                   &caja.montoInicial, caja.estado) == 6) {
            if (caja.idCaja == idCaja) {
                strcpy(caja.estado, estado);
                encontrado = 1;
            }
            fprintf(archivoTemporal, "%d,%d,%s,%s,%.2f,%s\n",
                    caja.idCaja, caja.idUsuario, caja.fecha, caja.hora,
                    caja.montoInicial, caja.estado);
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/cajas_temp.csv");
        return 0;
    }

    remove(ARCHIVO_CAJAS);
    rename("common/data/cajas_temp.csv", ARCHIVO_CAJAS);

    return 1;
}

float calcularIngresosEgresos(int idCaja, float *egresos) {
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "r");
    if (archivo == NULL) {
        return 0.0;
    }

    float ingresos = 0.0;
    *egresos = 0.0;
    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Transaccion t;
        if (sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                   &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                   &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                   t.observaciones, t.corteCaja, t.inicioJornada) == 12) {
            if (strcmp(t.metodoPago, "Efectivo") == 0) {
                if (strcmp(t.tipoMovimiento, "Ingreso") == 0) {
                    ingresos += t.ingreso;
                } else if (strcmp(t.tipoMovimiento, "Egreso") == 0) {
                    *egresos += t.egreso;
                }
            }
        }
    }

    fclose(archivo);
    return ingresos;
}

void realizarCorteCaja() {
    Caja cajaAbierta;
    if (!obtenerCajaAbierta(&cajaAbierta)) {
        printf("No hay cajas abiertas para realizar un corte.\n");
        return;
    }

    float egresos = 0.0;
    float ingresos = calcularIngresosEgresos(cajaAbierta.idCaja, &egresos);
    float montoDeclarado;

    printf("\n--- Realizar Corte de Caja ---\n");
    printf("ID Caja: %d\n", cajaAbierta.idCaja);
    printf("Monto Inicial: %.2f\n", cajaAbierta.montoInicial);
    printf("Ingresos Totales: %.2f\n", ingresos);
    printf("Egresos Totales: %.2f\n", egresos);
    printf("Ingrese el monto declarado: ");
    scanf("%f", &montoDeclarado);

    float saldoFinal = montoDeclarado;
    float diferencia = saldoFinal - (cajaAbierta.montoInicial + ingresos - egresos);

    CorteCaja corte;
    corte.idCorte = obtenerSiguienteIdCorteCaja();
    corte.idCaja = cajaAbierta.idCaja;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(corte.fecha, sizeof(corte.fecha), "%Y-%m-%d", tiempoLocal);
    strftime(corte.hora, sizeof(corte.hora), "%H:%M:%S", tiempoLocal);

    corte.ingresosTotales = ingresos;
    corte.egresosTotales = egresos;
    corte.saldoFinal = saldoFinal;
    corte.montoDeclarado = cajaAbierta.montoInicial + ingresos - egresos;
    corte.diferencia = diferencia;

    printf("Ingrese observaciones: ");
    scanf(" %[^\n]", corte.observaciones);

    FILE *archivo = fopen(ARCHIVO_CORTES, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de cortes.\n");
        return;
    }

    fprintf(archivo, "%d,%d,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%s\n",
            corte.idCorte, corte.idCaja, corte.fecha, corte.hora,
            corte.ingresosTotales, corte.egresosTotales, corte.saldoFinal,
            corte.montoDeclarado, corte.diferencia, corte.observaciones);

    fclose(archivo);

    actualizarEstadoCaja(cajaAbierta.idCaja, "Cerrada");

    printf("Corte de caja realizado con éxito.\n");
}
