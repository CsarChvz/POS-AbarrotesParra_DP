#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

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
    transaccion.idUsuario = usuario_global.id;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(transaccion.fecha, sizeof(transaccion.fecha), "%Y-%m-%d", tiempoLocal);

    strcpy(transaccion.tipoMovimiento, "Ingreso");

    printf("Categor%ca de ingreso (Venta, Dep%csito, etc.): ", 161, 162);
    scanf("%s", transaccion.categoriaTransaccion);

    printf("Monto del ingreso: ");
    scanf("%f", &transaccion.ingreso);
    transaccion.egreso = 0.0;

    printf("M%ctodo de pago: ", 130);
    scanf("%s", transaccion.metodoPago);

    printf("Observaciones: ");
    scanf(" %[^\n]", transaccion.observaciones);

    printf("¿Corte de caja? (S%c/No): ", 161);
    scanf("%s", transaccion.corteCaja);

    printf("¿Inicio de jornada? (S%c/No): ", 161);
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

    printf("Ingreso registrado con %cxito.\n", 130);
    // Registrar auditoría
    registrarRegistroAuditoria(transaccion.idUsuario, "TRANSACCION_INGRESO", "Registro de ingreso", "Transaccion", transaccion.idTransaccion, "Ingreso registrado", "Informativo", "Exito");
}

void registrarEgreso() {
    Transaccion transaccion;
    transaccion.idTransaccion = obtenerSiguienteIdTransaccion();
    transaccion.idUsuario = usuario_global.id;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(transaccion.fecha, sizeof(transaccion.fecha), "%Y-%m-%d", tiempoLocal);

    strcpy(transaccion.tipoMovimiento, "Egreso");

    printf("Categor%ca de egreso (Compra, Pago, etc.): ", 161);
    scanf("%s", transaccion.categoriaTransaccion);

    printf("Monto del egreso: ");
    scanf("%f", &transaccion.egreso);
    transaccion.ingreso = 0.0;

    printf("M%ctodo de pago: ", 130);
    scanf("%s", transaccion.metodoPago);

    printf("Observaciones: ");
    scanf(" %[^\n]", transaccion.observaciones);

    printf("¿Corte de caja? (S%c/No): ", 161);
    scanf("%s", transaccion.corteCaja);

    printf("¿Inicio de jornada? (S%c/No): ", 161);
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

    printf("Egreso registrado con %cxito.\n", 130);
    // Registrar auditoría
    registrarRegistroAuditoria(transaccion.idUsuario, "TRANSACCION_EGRESO", "Registro de egreso", "Transaccion", transaccion.idTransaccion, "Egreso registrado", "Informativo", "Exito");
}

// Función para obtener transacciones desde el archivo CSV
int obtenerTransacciones(Transaccion **transacciones) {
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }

    char linea[256];
    int count = 0;

    // Contamos la cantidad de transacciones en el archivo
    while (fgets(linea, 256, archivo) != NULL) {
        count++;
    }

    // Volvemos al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Reservamos memoria para las transacciones
    *transacciones = (Transaccion *)malloc(count * sizeof(Transaccion));
    if (*transacciones == NULL) {
        fclose(archivo);
        return 0; // Error al asignar memoria
    }

    // Saltar la línea de cabecera si existe
    fgets(linea, 256, archivo);

    // Leer las transacciones desde el archivo CSV
    int i = 0;
    while (fgets(linea, 256, archivo) != NULL) {
        Transaccion t;
        sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                t.observaciones, t.corteCaja, t.inicioJornada);
        (*transacciones)[i] = t;
        i++;
    }

    fclose(archivo);
    return count; // Retorna la cantidad de transacciones leídas
}

// Función para listar transacciones con paginación
int listarTransacciones() {
    Transaccion *transacciones = NULL;
    int cantidadTransaccionesLeidas, i, pagina = 1, transaccionesPorPagina = 10;

    cantidadTransaccionesLeidas = obtenerTransacciones(&transacciones);

    if (cantidadTransaccionesLeidas == 0) {
        printf("\nNo hay transacciones registradas o error al leer el archivo.\n");
        return 1;
    }

    while (1) {
        printf("\n--- Lista de Transacciones - P%cgina %d ---\n", 160, pagina);

        // Mostrar las transacciones de la página actual
        int inicio = (pagina - 1) * transaccionesPorPagina;
        int fin = inicio + transaccionesPorPagina;
        if (fin > cantidadTransaccionesLeidas) {
            fin = cantidadTransaccionesLeidas;
        }

        printf("ID\tFecha\t\tTipo\tCategor%ca\tIngreso\tEgreso\tSaldo\tM%ctodo\tUsuario\tObservaciones\tCorte\tInicio\n", 161, 130);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = inicio; i < fin; i++) {
            printf("%d\t%s\t%s\t%s\t%.2f\t%.2f\t%.2f\t%s\t%d\t%s\t%s\t%s\n",
                    transacciones[i].idTransaccion, transacciones[i].fecha, transacciones[i].tipoMovimiento,
                    transacciones[i].categoriaTransaccion, transacciones[i].ingreso, transacciones[i].egreso,
                    transacciones[i].saldo, transacciones[i].metodoPago, transacciones[i].idUsuario,
                    transacciones[i].observaciones, transacciones[i].corteCaja, transacciones[i].inicioJornada);
        }

        // Preguntar si desea ver la siguiente página o volver a la anterior
        char opcion;
        printf("\n--- Opciones ---\n");
        if (pagina > 1) {
            printf("a. P%cgina Anterior\n", 160);
        }
        if (fin < cantidadTransaccionesLeidas) {
            printf("s. Siguiente P%cgina\n", 160);
        }
        printf("q. Salir\n");
        printf("Seleccione una opci%cn: ", 162);
        scanf(" %c", &opcion); // Espacio antes de %c para consumir el salto de línea

        if (opcion == 'a' && pagina > 1) {
            pagina--;
        } else if (opcion == 's' && fin < cantidadTransaccionesLeidas) {
            pagina++;
        } else if (opcion == 'q') {
            break;
        } else {
            printf("Opci%cn inv%clida.\n", 162, 160);
        }
    }

    // Liberar memoria
    free(transacciones);
    //registrar auditoria
    registrarRegistroAuditoria(usuario_global.id, "TRANSACCIONES_LISTADO", "Listado de transacciones", "Transaccion", 0, "Listado de transacciones visto", "Informativo", "Ecxito");
    return 0;
}
