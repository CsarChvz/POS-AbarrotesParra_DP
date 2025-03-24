#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define ARCHIVO_AUDITORIA "common/data/auditoria.csv"
#define MAX_LINEA 512

typedef struct {
    int idAuditoria;
    char fecha[11];
    char hora[9];
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
    if (!archivo) return 1;

    int maxId = 0;
    char linea[MAX_LINEA];
    fgets(linea, MAX_LINEA, archivo); // Saltar cabecera

    while (fgets(linea, MAX_LINEA, archivo)) {
        RegistroAuditoria r;
        if (sscanf(linea, "%d,%10[^,],%8[^,],%d,%19[^,],%99[^,],%19[^,],%d,%199[^,],%19[^,],%9[^\n]",
                   &r.idAuditoria, r.fecha, r.hora, &r.idUsuario, r.accionCodigo,
                   r.accionDescripcion, r.tipoObjeto, &r.idObjeto, r.detalles,
                   r.severidad, r.estado) == 11 && r.idAuditoria > maxId) {
            maxId = r.idAuditoria;
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

    strncpy(registro.accionCodigo, accionCodigo, sizeof(registro.accionCodigo) - 1);
    strncpy(registro.accionDescripcion, accionDescripcion, sizeof(registro.accionDescripcion) - 1);
    strncpy(registro.tipoObjeto, tipoObjeto, sizeof(registro.tipoObjeto) - 1);
    registro.idObjeto = idObjeto;
    strncpy(registro.detalles, detalles, sizeof(registro.detalles) - 1);
    strncpy(registro.severidad, severidad, sizeof(registro.severidad) - 1);
    strncpy(registro.estado, estado, sizeof(registro.estado) - 1);

    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "a");
    if (!archivo) {
        perror("Error al abrir el archivo de auditoría");
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
    scanf("%19s", accionCodigo);
    printf("Descripción de la acción: ");
    scanf(" %99[^\n]", accionDescripcion);
    printf("Tipo de objeto: ");
    scanf("%19s", tipoObjeto);
    printf("ID del objeto: ");
    scanf("%d", &idObjeto);
    printf("Detalles: ");
    scanf(" %199[^\n]", detalles);
    printf("Severidad: ");
    scanf("%19s", severidad);
    printf("Estado: ");
    scanf("%9s", estado);

    registrarRegistroAuditoria(idUsuario, accionCodigo, accionDescripcion, tipoObjeto, idObjeto, detalles, severidad, estado);
}

int obtenerRegistrosAuditoria(RegistroAuditoria **registros) {
    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "r");
    if (!archivo) return 0;

    char linea[MAX_LINEA];
    int count = 0;
    while (fgets(linea, MAX_LINEA, archivo)) count++;

    rewind(archivo);
    *registros = malloc(count * sizeof(RegistroAuditoria));
    if (!*registros) {
        fclose(archivo);
        return 0;
    }
    fgets(linea, MAX_LINEA, archivo); // Saltar cabecera

    int i = 0;
    while (fgets(linea, MAX_LINEA, archivo)) {
        RegistroAuditoria r;
        if (sscanf(linea, "%d,%10[^,],%8[^,],%d,%19[^,],%99[^,],%19[^,],%d,%199[^,],%19[^,],%9[^\n]",
                   &r.idAuditoria, r.fecha, r.hora, &r.idUsuario, r.accionCodigo,
                   r.accionDescripcion, r.tipoObjeto, &r.idObjeto, r.detalles,
                   r.severidad, r.estado) == 11) {
            (*registros)[i++] = r;
        }
    }
    fclose(archivo);
    return i;
}

int listarRegistrosAuditoria(int filtro, char filtroValor[]) {
    RegistroAuditoria *registros = NULL;
    int i;
    int cantidadRegistrosLeidos = obtenerRegistrosAuditoria(&registros);
    if (cantidadRegistrosLeidos == 0) {
        printf("\nNo hay registros de auditoría o error al leer el archivo.\n");
        return 1;
    }

    int pagina = 1, registrosPorPagina = 10, registrosMostrados = 0;
    while (1) {
        printf("\n--- Registros de Auditoría - Página %d ---\n", pagina);
        printf("ID\tFecha\t\tHora\tUsuario\tCódigo\tAcción\t\tObjeto\tID Objeto\tDetalles\t\t\tSeveridad\tEstado\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < cantidadRegistrosLeidos; i++) {
            int mostrarRegistro = 0;
            if (filtro == 0 || (filtro == 1 && registros[i].idUsuario == atoi(filtroValor)) ||
                (filtro == 2 && strstr(registros[i].detalles, filtroValor)) ) {
                mostrarRegistro = 1;
            }

            if (mostrarRegistro && registrosMostrados >= (pagina - 1) * registrosPorPagina && registrosMostrados < pagina * registrosPorPagina) {
                printf("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%d\t\t%s\t\t%s\t\t%s\n",
                       registros[i].idAuditoria, registros[i].fecha, registros[i].hora, registros[i].idUsuario,
                       registros[i].accionCodigo, registros[i].accionDescripcion, registros[i].tipoObjeto,
                       registros[i].idObjeto, registros[i].detalles, registros[i].severidad, registros[i].estado);
            }
            if (mostrarRegistro) registrosMostrados++;}

            char opcion;
            printf("\n--- Opciones ---\n");
            if (pagina > 1) printf("a. Página Anterior\n");
            if (registrosMostrados > pagina * registrosPorPagina) printf("s. Siguiente Página\n");
            printf("q. Salir\n");
            printf("Seleccione una opción: ");
            scanf(" %c", &opcion);
    
            if (opcion == 'a' && pagina > 1) pagina--;
            else if (opcion == 's' && registrosMostrados > pagina * registrosPorPagina) pagina++;
            else if (opcion == 'q') break;
            else printf("Opción inválida.\n");
        }
    
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
            filtro = 0;
            strcpy(filtroValor, "");
        } else {
            printf("Opción inválida.\n");
            return;
        }
    
        listarRegistrosAuditoria(filtro, filtroValor);
    }
    
    void busquedaAvanzada() {
        int filtro;
        char filtroValor[100];
    
        printf("\n--- Búsqueda Avanzada de Registros de Auditoría ---\n");
        printf("1. Filtrar por usuario\n");
        printf("2. Filtrar por palabra clave\n");
        printf("3. Sin filtro\n");
        printf("Seleccione una opción: ");
        scanf("%d", &filtro);
    
        if (filtro == 1) {
            printf("Ingrese el ID del usuario: ");
            scanf("%s", filtroValor);
        } else if (filtro == 2) {
            printf("Ingrese la palabra clave: ");
            scanf(" %99[^\n]", filtroValor);
        } else if (filtro == 3) {
            filtro = 0;
            strcpy(filtroValor, "");
        } else {
            printf("Opción inválida.\n");
            return;
        }
    
        listarRegistrosAuditoria(filtro, filtroValor);
    }
