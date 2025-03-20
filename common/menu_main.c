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

// Definir estructura de opción de menú
typedef struct {
    char *nombre;
    void (*funcion)();
    int permiso_minimo; // 1 = Vendedor, 2 = Admin
} OpcionMenu;

// Función para verificar permisos
int tiene_permiso(int role, int permiso_minimo) {
    return role >= permiso_minimo;
}

// Menú principal
void menu_main(int role) {
    OpcionMenu opciones[] = {
        {"Gestión de Inventario", menu_inventario, ROL_VENDEDOR},
        {"Proceso de Venta", menu_proceso_venta, ROL_VENDEDOR},
        {"Gestión de Caja", menu_gestion_caja, ROL_ADMIN},
        {"Reportes y Estadísticas", menu_reportes_estadisticas, ROL_ADMIN},
        {"Administración de Usuarios", menu_administracion_usuarios, ROL_VENDEDOR},
        {"Funcionalidades Adicionales", menu_funcionalidades_adicionales, ROL_VENDEDOR}
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
	int i, num_opcion;
    do {
    	imprimir_logo();
        printf("\n======= MENÚ PRINCIPAL =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Salir\n", total_opciones + 1);
        printf("==============================\n");
        printf("Seleccione una opción: ");

        scanf("%d", &opcion);

        if (opcion >= 1 && opcion <= total_opciones) {
            int index = opcion - 1;
            if (tiene_permiso(role, opciones[index].permiso_minimo)) {
                opciones[index].funcion();
            } else {
                printf("Acceso denegado. No tiene permisos para esta opción.\n");
            }
        } else if (opcion != total_opciones + 1) {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != total_opciones + 1);

    printf("Saliendo del sistema...\n");
}
void menu_inventario() {
    int opcion;
    do {
        printf("\n--- Gestión de Inventario ---\n");
        printf("1. Administrar Productos\n");
        printf("2. Control de Stock\n");
        printf("3. Descuentos y Promociones\n");
        printf("4. Volver al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menu_administracion_productos();
                break;
            case 2:
                menu_control_stock();
                break;
            case 3:
                printf("Función de Descuentos y Promociones aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_administracion_productos() {
    int opcion;
    do {
        printf("\n--- Administración de Productos ---\n");
        printf("1. Registrar Nuevo Producto\n");
        printf("2. Actualizar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Volver al Menú de Inventario\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrar_producto(2);
                break;
            case 2:
                printf("Función de Actualización aún no implementada.\n");
                break;
            case 3:
                printf("Función de Eliminación aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_control_stock() {
    int opcion;
    do {
        printf("\n--- Control de Stock ---\n");
        printf("1. Registrar Reposición de Stock\n");
        printf("2. Configurar Stock Mínimo\n");
        printf("3. Ver Productos con Bajo Stock\n");
        printf("4. Volver al Menú de Inventario\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Función de Reposición aún no implementada.\n");
                break;
            case 2:
                printf("Función de Configuración de Stock aún no implementada.\n");
                break;
            case 3:
                printf("Función de Visualización de Bajo Stock aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_proceso_venta() {
    int opcion;
    do {
        printf("\n--- Proceso de Venta ---\n");
        printf("1. Seleccionar Productos\n");
        printf("2. Confirmar Venta\n");
        printf("3. Elegir Método de Pago\n");
        printf("4. Imprimir Ticket\n");
        printf("5. Volver al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Función de Selección aún no implementada.\n");
                break;
            case 2:
                printf("Función de Confirmación aún no implementada.\n");
                break;
            case 3:
                menu_metodos_pago();
                break;
            case 4:
                printf("Función de Impresión aún no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 5);
}

void menu_metodos_pago() {
    int opcion;
    do {
        printf("\n--- Métodos de Pago ---\n");
        printf("1. Pago en Efectivo\n");
        printf("2. Pago con Tarjeta\n");
        printf("3. Pago Parcial\n");
        printf("4. Volver al Menú de Proceso de Venta\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Función de pago aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_gestion_caja() {
    printf("\n--- Gestión de Caja ---\n");
    printf("Funcionalidad aún no implementada.\n");
}

void menu_reportes_estadisticas() {
    printf("\n--- Reportes y Estadísticas ---\n");
    printf("Funcionalidad aún no implementada.\n");
}

void menu_administracion_usuarios() {
    printf("\n--- Administración de Usuarios y Seguridad ---\n");
    printf("Funcionalidad aún no implementada.\n");
}

void menu_funcionalidades_adicionales() {
    printf("\n--- Funcionalidades Adicionales ---\n");
    printf("Funcionalidad aún no implementada.\n");
}
