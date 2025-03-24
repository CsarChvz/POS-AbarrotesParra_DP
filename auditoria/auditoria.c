#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARCHIVO_AUDITORIA "common/data/auditoria.csv"

typedef struct {
    int idAuditoria;
    char fecha[11]; // AAAA-MM-DD
    char hora[9];  // HH:MM:SS
    int idUsuario;
    char accionCodigo[20];
    char accionDescripcion[100];
    char tipoObjeto[20];
    int idObjeto;
    char detalles[200];
    char severidad[20];
    char estado[10];
} RegistroAuditoria;

int obtenerSiguienteIdAuditoria() {
    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "r");
    if (archivo == NULL) {
        return 1; // Si el archivo no existe, el primer ID es 1
    }

    int maxId = 0;
    char linea[512]; // Ajusta el tamaño según la longitud máxima de la línea

    // Saltar la cabecera
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo)) {
        RegistroAuditoria r;
        if (sscanf(linea, "%d,%10[^,],%8[^,],%d,%19[^,],%99[^,],%19[^,],%d,%199[^,],%19[^,],%9[^\n]",
                   &r.idAuditoria, r.fecha, r.hora, &r.idUsuario, r.accionCodigo,
                   r.accionDescripcion, r.tipoObjeto, &r.idObjeto, r.detalles,
                   r.severidad, r.estado) == 11) {
            if (r.idAuditoria > maxId) {
                maxId = r.idAuditoria;
            }
        }
    }

    fclose(archivo);
    return maxId + 1;
}

void registrarRegistroAuditoria(int idUsuario, const char *accionCodigo, const char *accionDescripcion,
                                const char *tipoObjeto, int idObjeto, const char *detalles,
                                const char *severidad, const char *estado) {
    RegistroAuditoria registro;
    registro.idAuditoria = obtenerSiguienteIdAuditoria();
    registro.idUsuario = idUsuario;

    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    strftime(registro.fecha, sizeof(registro.fecha), "%Y-%m-%d", tiempoLocal);
    strftime(registro.hora, sizeof(registro.hora), "%H:%M:%S", tiempoLocal);

    strcpy(registro.accionCodigo, accionCodigo);
    strcpy(registro.accionDescripcion, accionDescripcion);
    strcpy(registro.tipoObjeto, tipoObjeto);
    registro.idObjeto = idObjeto;
    strcpy(registro.detalles, detalles);
    strcpy(registro.severidad, severidad);
    strcpy(registro.estado, estado);

    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de auditoría.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%d,%s,%s,%s,%d,%s,%s,%s\n",
            registro.idAuditoria, registro.fecha, registro.hora, registro.idUsuario,
            registro.accionCodigo, registro.accionDescripcion, registro.tipoObjeto,
            registro.idObjeto, registro.detalles, registro.severidad, registro.estado);

    fclose(archivo);

    printf("Registro de auditoría guardado con éxito.\n");
}

void solicitarYRegistrarAuditoria() {
    int idUsuario, idObjeto;
    char accionCodigo[20], accionDescripcion[100], tipoObjeto[20], detalles[200], severidad[20], estado[10];

    printf("ID del usuario: ");
    scanf("%d", &idUsuario);

    printf("Código de acción: ");
    scanf("%s", accionCodigo);

    printf("Descripción de la acción: ");
    scanf(" %[^\n]", accionDescripcion); // Leer hasta el salto de línea

    printf("Tipo de objeto: ");
    scanf("%s", tipoObjeto);

    printf("ID del objeto: ");
    scanf("%d", &idObjeto);

    printf("Detalles: ");
    scanf(" %[^\n]", detalles); // Leer hasta el salto de línea

    printf("Severidad: ");
    scanf("%s", severidad);

    printf("Estado: ");
    scanf("%s", estado);

    registrarRegistroAuditoria(idUsuario, accionCodigo, accionDescripcion, tipoObjeto, idObjeto, detalles, severidad, estado);
}

// Función para obtener registros de auditoría desde el archivo CSV
int obtenerRegistrosAuditoria(RegistroAuditoria **registros) {
    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }

    char linea[512];
    int count = 0;

    // Contamos la cantidad de registros en el archivo
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        count++;
    }

    // Volvemos al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Reservamos memoria para los registros
    *registros = (RegistroAuditoria *)malloc(count * sizeof(RegistroAuditoria));
    if (*registros == NULL) {
        fclose(archivo);
        return 0; // Error al asignar memoria
    }

    // Saltar la línea de cabecera si existe
    fgets(linea, sizeof(linea), archivo);

    // Leer los registros desde el archivo CSV
    int i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        RegistroAuditoria r;
        sscanf(linea, "%d,%10[^,],%8[^,],%d,%19[^,],%99[^,],%19[^,],%d,%199[^,],%19[^,],%9[^\n]",
               &r.idAuditoria, r.fecha, r.hora, &r.idUsuario, r.accionCodigo,
               r.accionDescripcion, r.tipoObjeto, &r.idObjeto, r.detalles,
               r.severidad, r.estado);
        (*registros)[i] = r;
        i++;
    }

    fclose(archivo);
    return count; // Retorna la cantidad de registros leídos
}

// Función para listar registros de auditoría con paginación y filtros
int listarRegistrosAuditoria(int filtro, char filtroValor[]) {
    RegistroAuditoria *registros = NULL;
    int cantidadRegistrosLeidos, i, pagina = 1, registrosPorPagina = 10;
    int registrosMostrados = 0; // Cantidad de registros mostrados con el filtro aplicado

    cantidadRegistrosLeidos = obtenerRegistrosAuditoria(&registros);

    if (cantidadRegistrosLeidos == 0) {
        printf("\nNo hay registros de auditoría o error al leer el archivo.\n");
        return 1;
    }

    while (1) {
        printf("\n--- Registros de Auditoría - Página %d ---\n", pagina);

        printf("ID\tFecha\t\tHora\tUsuario\tCódigo\tAcción\t\tObjeto\tID Objeto\tDetalles\t\t\tSeveridad\tEstado\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < cantidadRegistrosLeidos; i++) {
            // Aplicar filtros
            int mostrarRegistro = 0;
            if (filtro == 0) { // Sin filtro
                mostrarRegistro = 1;
            } else if (filtro == 1 && registros[i].idUsuario == atoi(filtroValor)) { // Filtrar por usuario
                mostrarRegistro = 1;
            } else if (filtro == 2 && strcmp(registros[i].fecha, filtroValor) == 0) { // Filtrar por fecha
                mostrarRegistro = 1;
            } else if (filtro == 3 && strcmp(registros[i].accionCodigo, filtroValor) == 0) { // Filtrar por tipo de acción
                mostrarRegistro = 1;
            }

            if (mostrarRegistro) {
                if (registrosMostrados >= (pagina - 1) * registrosPorPagina && registrosMostrados < pagina * registrosPorPagina) {
                    printf("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%d\t\t%s\t\t%s\t\t%s\n",
                           registros[i].idAuditoria, registros[i].fecha, registros[i].hora, registros[i].idUsuario,
                           registros[i].accionCodigo, registros[i].accionDescripcion, registros[i].tipoObjeto,
                           registros[i].idObjeto, registros[i].detalles, registros[i].severidad, registros[i].estado);
                }
                registrosMostrados++;
            }
        }

        // Preguntar si desea ver la siguiente página o volver a la anterior
        char opcion;
        printf("\n--- Opciones ---\n");
        if (pagina > 1) {
            printf("a. Página Anterior\n");
        }
        if (registrosMostrados > pagina * registrosPorPagina) {
            printf("s. Siguiente Página\n");
        }
        printf("q. Salir\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion); // Espacio antes de %c para consumir el salto de línea

        if (opcion == 'a' && pagina > 1) {
            pagina--;
        } else if (opcion == 's' && registrosMostrados > pagina * registrosPorPagina) {
            pagina++;
        } else if (opcion == 'q') {
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Liberar memoria
    free(registros);

    return 0;
}

void solicitarYListarAuditoria() {
    int filtro;
    char filtroValor[100];

    printf("\n--- Filtrar Registros de Auditoría ---\n");
    printf("1. Filtrar por usuario\n");
    printf("2. Filtrar por fecha\n");
    printf("3. Filtrar por tipo de acción\n");
    printf("4. Sin filtro\n");
    printf("Seleccione una opción: ");
    scanf("%d", &filtro);

    if (filtro == 1) {
        printf("Ingrese el ID del usuario: ");
        scanf("%s", filtroValor);
    } else if (filtro == 2) {
        printf("Ingrese la fecha (AAAA-MM-DD): ");
        scanf("%s", filtroValor);
    } else if (filtro == 3) {
        printf("Ingrese el tipo de acción: ");
        scanf("%s", filtroValor);
    } else if (filtro == 4) {
        filtro = 0; // Sin filtro
        strcpy(filtroValor, ""); // Valor de filtro vacío
    } else {
        printf("Opción inválida.\n");
        return;
    }

    listarRegistrosAuditoria(filtro, filtroValor);
}