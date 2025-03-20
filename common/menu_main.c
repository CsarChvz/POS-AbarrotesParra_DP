#include <stdio.h>
#include "logo.h"
#include "../include/ventas.h"

// Definimos constantes para roles
#define ROL_VENDEDOR 1
#define ROL_ADMIN 2

// Prototipos de funciones
void menu_inventario();
void menu_proceso_venta();
void menu_gestion_caja();
void menu_reportes_estadisticas();
void menu_administracion_usuarios();
void menu_funcionalidades_adicionales();
void menu_administracion_productos();
void menu_control_stock();
void menu_metodos_pago();

// Definir estructura de opci�n de men�
typedef struct {
    char *nombre;
    void (*funcion)();
    int permiso_minimo; // 1 = Vendedor, 2 = Admin
} OpcionMenu;

// Funci�n para verificar permisos
int tiene_permiso(int role, int permiso_minimo) {
    return role >= permiso_minimo;
}

// Men� principal
void menu_main(int role) {
    OpcionMenu opciones[] = {
        {"Gesti�n de Inventario", menu_inventario, ROL_VENDEDOR},
        {"Proceso de Venta", menu_proceso_venta, ROL_VENDEDOR},
        {"Gesti�n de Caja", menu_gestion_caja, ROL_ADMIN},
        {"Reportes y Estad�sticas", menu_reportes_estadisticas, ROL_ADMIN},
        {"Administraci�n de Usuarios", menu_administracion_usuarios, ROL_VENDEDOR},
        {"Funcionalidades Adicionales", menu_funcionalidades_adicionales, ROL_VENDEDOR}
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
	int i, num_opcion;
    do {
    	imprimir_logo();
        printf("\n======= MEN� PRINCIPAL =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Salir\n", total_opciones + 1);
        printf("==============================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        if (opcion >= 1 && opcion <= total_opciones) {
            int index = opcion - 1;
            if (tiene_permiso(role, opciones[index].permiso_minimo)) {
                opciones[index].funcion();
            } else {
                printf("Acceso denegado. No tiene permisos para esta opci�n.\n");
            }
        } else if (opcion != total_opciones + 1) {
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (opcion != total_opciones + 1);

    printf("Saliendo del sistema...\n");
}
void menu_inventario() {
    int opcion;
    do {
        printf("\n--- Gesti�n de Inventario ---\n");
        printf("1. Administrar Productos\n");
        printf("2. Control de Stock\n");
        printf("3. Descuentos y Promociones\n");
        printf("4. Volver al Men� Principal\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menu_administracion_productos();
                break;
            case 2:
                menu_control_stock();
                break;
            case 3:
                printf("Funci�n de Descuentos y Promociones a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_administracion_productos() {
    int opcion;
    do {
        printf("\n--- Administraci�n de Productos ---\n");
        printf("1. Registrar Nuevo Producto\n");
        printf("2. Actualizar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Volver al Men� de Inventario\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrar_producto(2);
                break;
            case 2:
                printf("Funci�n de Actualizaci�n a�n no implementada.\n");
                break;
            case 3:
                printf("Funci�n de Eliminaci�n a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_control_stock() {
    int opcion;
    do {
        printf("\n--- Control de Stock ---\n");
        printf("1. Registrar Reposici�n de Stock\n");
        printf("2. Configurar Stock M�nimo\n");
        printf("3. Ver Productos con Bajo Stock\n");
        printf("4. Volver al Men� de Inventario\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Funci�n de Reposici�n a�n no implementada.\n");
                break;
            case 2:
                printf("Funci�n de Configuraci�n de Stock a�n no implementada.\n");
                break;
            case 3:
                printf("Funci�n de Visualizaci�n de Bajo Stock a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_proceso_venta() {
    int opcion;
    do {
        printf("\n--- Proceso de Venta ---\n");
        printf("1. Seleccionar Productos\n");
        printf("2. Confirmar Venta\n");
        printf("3. Elegir M�todo de Pago\n");
        printf("4. Imprimir Ticket\n");
        printf("5. Volver al Men� Principal\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Funci�n de Selecci�n a�n no implementada.\n");
                break;
            case 2:
                printf("Funci�n de Confirmaci�n a�n no implementada.\n");
                break;
            case 3:
                menu_metodos_pago();
                break;
            case 4:
                printf("Funci�n de Impresi�n a�n no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 5);
}

void menu_metodos_pago() {
    int opcion;
    do {
        printf("\n--- M�todos de Pago ---\n");
        printf("1. Pago en Efectivo\n");
        printf("2. Pago con Tarjeta\n");
        printf("3. Pago Parcial\n");
        printf("4. Volver al Men� de Proceso de Venta\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Funci�n de pago a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_gestion_caja() {
    printf("\n--- Gesti�n de Caja ---\n");
    printf("Funcionalidad a�n no implementada.\n");
}

void menu_reportes_estadisticas() {
    printf("\n--- Reportes y Estad�sticas ---\n");
    printf("Funcionalidad a�n no implementada.\n");
}

void menu_administracion_usuarios() {
    printf("\n--- Administraci�n de Usuarios y Seguridad ---\n");
    printf("Funcionalidad a�n no implementada.\n");
}

void menu_funcionalidades_adicionales() {
    printf("\n--- Funcionalidades Adicionales ---\n");
    printf("Funcionalidad a�n no implementada.\n");
}
