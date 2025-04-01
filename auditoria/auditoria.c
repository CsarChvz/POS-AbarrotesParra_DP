#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARCHIVO_AUDITORIA "common/data/auditoria.csv"
#define MAX_LINEA 512
#define REGISTROS_POR_PAGINA 10

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

// Función para obtener el siguiente ID de auditoría disponible
int obtenerSiguienteIdAuditoria() {
    FILE *archivo = fopen(ARCHIVO_AUDITORIA, "r");
    if (!archivo) return 1; // Si no se puede abrir el archivo, inicia con ID 1

    int maxId = 0;
    char linea[MAX_LINEA];
    fgets(linea, MAX_LINEA, archivo); // Saltar la cabecera del archivo

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
    return maxId + 1; // El siguiente ID es el máximo encontrado + 1
}

// Función para registrar un nuevo registro de auditoría en el archivo
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
        char errorMsg[256]; // Buffer para la cadena formateada
        sprintf(errorMsg, "Error al abrir el archivo de auditor%c%ca", 161, 'a');
        perror(errorMsg); // Pasar la cadena formateada a perror
        return;
    }

    fprintf(archivo, "%d,%s,%s,%d,%s,%s,%s,%d,%s,%s,%s\n",
            registro.idAuditoria, registro.fecha, registro.hora, registro.idUsuario,
            registro.accionCodigo, registro.accionDescripcion, registro.tipoObjeto,
            registro.idObjeto, registro.detalles, registro.severidad, registro.estado);

    fclose(archivo);
    printf("Registro de auditor%c%ca guardado con %c%cxito.\n", 161, 'a', 130, 'x');
}

// Función para solicitar al usuario los datos de un nuevo registro y registrarlo
void solicitarYRegistrarAuditoria() {
    int idUsuario, idObjeto;
    char accionCodigo[20], accionDescripcion[100], tipoObjeto[20], detalles[200], severidad[20], estado[10];

    printf("ID del usuario: ");
    scanf("%d", &idUsuario);
    printf("C%cdigo de acci%cn: ", 162, 162);
    scanf("%19s", accionCodigo);
    printf("Descripci%cn de la acci%cn: ", 162, 162);
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

// Función para obtener todos los registros de auditoría del archivo
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
    fgets(linea, MAX_LINEA, archivo); // Saltar la cabecera

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

// Función para listar los registros de auditoría con opciones de paginación y filtrado
int listarRegistrosAuditoria(int filtro, char filtroValor[]) {
    RegistroAuditoria *registros = NULL;
    int i;
    int cantidadRegistrosLeidos = obtenerRegistrosAuditoria(&registros);
    if (cantidadRegistrosLeidos == 0) {
        printf("\nNo hay registros de auditor%c%ca o error al leer el archivo.\n", 161, 'a');
        return 1;
    }

    int pagina = 1, registrosMostrados = 0;
    while (1) {
        printf("\n--- Registros de Auditor%c%ca - P%cgina %d ---\n", 161, 'a', 160, pagina);
        printf("ID\tFecha\t\tHora\tUsuario\tC%cdigo\tAcci%cn\t\tObjeto\tID Objeto\tDetalles\t\t\tSeveridad\tEstado\n", 162, 162);
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < cantidadRegistrosLeidos; i++) {
            int mostrarRegistro = 0;
            if (filtro == 0 || (filtro == 1 && registros[i].idUsuario == atoi(filtroValor)) ||
                (filtro == 2 && strstr(registros[i].detalles, filtroValor))) {
                mostrarRegistro = 1;
            }

            if (mostrarRegistro && registrosMostrados >= (pagina - 1) * REGISTROS_POR_PAGINA &&
                registrosMostrados < pagina * REGISTROS_POR_PAGINA) {
                printf("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%d\t\t%s\t\t%s\t\t%s\n",
                       registros[i].idAuditoria, registros[i].fecha, registros[i].hora, registros[i].idUsuario,
                       registros[i].accionCodigo, registros[i].accionDescripcion, registros[i].tipoObjeto,
                       registros[i].idObjeto, registros[i].detalles, registros[i].severidad, registros[i].estado);
            }
            if (mostrarRegistro) registrosMostrados++;
        }

        char opcion;
        printf("\n--- Opciones ---\n");
        if (pagina > 1) printf("a. P%cgina Anterior\n", 160);
        if (registrosMostrados > pagina * REGISTROS_POR_PAGINA) printf("s. Siguiente P%cgina\n", 160);
        printf("q. Salir\n");
        printf("Seleccione una opci%cn: ", 162);
        scanf(" %c", &opcion);

        if (opcion == 'a' && pagina > 1) pagina--;
        else if (opcion == 's' && registrosMostrados > pagina * REGISTROS_POR_PAGINA) pagina++;
        else if (opcion == 'q') break;
        else printf("Opci%cn inv%clida.\n", 162, 160);
    }

    free(registros);
    return 0;
}

// Función para solicitar al usuario opciones de filtrado y listar los registros
void solicitarYListarAuditoria() {
    int filtro;
    char filtroValor[100];

    printf("\n--- Filtrar Registros de Auditor%c%ca ---\n", 161, 'a');
    printf("1. Filtrar por usuario\n");
    printf("2. Filtrar por fecha\n");
    printf("3. Filtrar por tipo de acci%cn\n", 162);
    printf("4. Sin filtro\n");
    printf("Seleccione una opci%cn: ", 162);
    scanf("%d", &filtro);

    if (filtro == 1) {
        printf("Ingrese el ID del usuario: ");
        scanf("%s", filtroValor);
    } else if (filtro == 2) {
        printf("Ingrese la fecha (AAAA-MM-DD): ");
        scanf("%s", filtroValor);
    } else if (filtro == 3) {
        printf("Ingrese el tipo de acci%cn: ", 162);
        scanf("%s", filtroValor);
    } else if (filtro == 4) {
        filtro = 0;
        strcpy(filtroValor, "");
    } else {
        printf("Opci%cn inv%clida.\n", 162, 160);
        return;
    }

    listarRegistrosAuditoria(filtro, filtroValor);
}

// Función para realizar una búsqueda avanzada con opciones de filtrado
void busquedaAvanzada() {
    int filtro;
    char filtroValor[100];

    printf("\n--- B%csqueda Avanzada de Registros de Auditor%c%ca ---\n", 163, 161, 'a');
    printf("1. Filtrar por usuario\n");
    printf("2. Filtrar por palabra clave\n");
    printf("3. Sin filtro\n");
    printf("Seleccione una opci%cn: ", 162);
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
        printf("Opci%cn inv%clida.\n", 162, 160);
        return;
    }

    listarRegistrosAuditoria(filtro, filtroValor);
}
