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

