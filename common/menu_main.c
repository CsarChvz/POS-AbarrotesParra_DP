#include <stdio.h>
#include <string.h>

#include "logo.h"
#include "../include/ventas.h"
#include "../include/reportes.h"
#include "../include/gestion_usuarios.h"
#include "../include/inventario.h"
#include "../include/seguridad.h"
#include "../include/control_stock.h"
#include "../include/transacciones.h"
#include "../include/auditoria.h"
#include "../include/apertura_caja.h"
#include "../include/corte_caja.h"
#include "../include/descuentos.h"

// Definimos constantes para roles
#define ROL_VENDEDOR 1
#define ROL_ADMIN 2
#define T 100
#define USUARIO_LENGTH_C 50

// Prototipos de funciones
void menu_inventario(int role);
void menu_ventas();
void menu_gestion_caja(int role);
void menu_reportes_estadisticas();
void menu_administracion(int role);

void menu_administracion_productos();
void menu_control_stock();
void menu_descuentos_promociones();

void menu_apertura_caja();
void menu_ingresos_egresos();
void menu_corte_caja();

void menu_reportes_ventas();
void menu_reportes_inventario();
void menu_reportes_caja();
void menu_exportacion_datos();
void menu_gestion_usuarios();

void menu_registro_auditoria();

void menu_mi_cuenta();

// Definir estructura de opción de menú
typedef struct {
    char *nombre;
    void (*funcion)(int role);
    int permiso_minimo; // 1 = Vendedor, 2 = Admin
} OpcionMenu;

// Función para verificar permisos
int tiene_permiso(int role, int permiso_minimo) {
    return role >= permiso_minimo;
}

// Menú principal
void menu_main(int role) {
    OpcionMenu opciones[] = {
        {"Inventario", menu_inventario, ROL_VENDEDOR},
        {"Ventas", menu_ventas, ROL_VENDEDOR},
        {"Caja", menu_gestion_caja, ROL_VENDEDOR},
        {"Reportes", menu_reportes_estadisticas, ROL_VENDEDOR},
        {"Administracion", menu_administracion, ROL_ADMIN}, // "ó" = \xF3 (ISO-8859-1)
        {"Mi cuenta", menu_mi_cuenta, ROL_VENDEDOR},
    };
    

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        imprimir_logo();
        printf("\n======= MEN%c PRINCIPAL =======\n", 233);
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Salir\n", num_opcion);
        printf("==============================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            printf("Saliendo del sistema...\n");
            break;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_inventario(int role) {
    OpcionMenu opciones[] = {
        {"Gestión de Productos", menu_administracion_productos, ROL_ADMIN},
        {"Control de Stock", menu_control_stock, ROL_VENDEDOR},
        {"Descuentos y Promociones", menu_descuentos_promociones, ROL_ADMIN},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= INVENTARIO =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men%c Principal\n", 233);
        printf("==========================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            return;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_administracion_productos() {
    int opcion;
    do {
        printf("\n--- Gesti%cn de Productos ---\n", 243);
        printf("1. Registrar Nuevo Producto\n");
        printf("2. Visualizar productos\n");
        printf("3. Actualizar Producto (No implementado)\n");
        printf("4. Eliminar Producto\n");
        printf("5. Volver\n");
        printf("Seleccione una opci%cn: ", 243);

        if (scanf("%d", &opcion) != 1) {
            // Si la entrada no es válida, limpiamos el buffer y mostramos un error
            while (getchar() != '\n'); // Limpiar buffer
            printf("Error: Entrada inv%clida. Por favor, ingrese un n%cmero.\n", 160, 250);
            continue; // Volver al inicio del menú
        }

        switch (opcion) {
            case 1:
                registrarProductoMenu();
                break;
            case 2:
                visualizarProductosMenu();
                break;
            case 3:
                printf("Funci%cn de Actualizaci%cn a%cn no implementada.\n", 243, 243, 161);
                break;
            case 4:
                eliminarProductoMenu();
                break;
            case 5:
                printf("Volviendo...\n");
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 5); // Seguir mostrando el menú hasta que se elija salir
}

void menu_control_stock() {
    int opcion;
    int role;
    role = obtener_rol(usuario_global.usuario);
    do {
        printf("\n--- Control de Stock ---\n");
        printf("1. Ver Stock Actual\n");
        printf("2. Ver Productos con Bajo Stock\n");

        // Solo mostrar estas opciones al administrador
        if (role == ROL_ADMIN) {
            printf("3. Registrar Reposici%cn de Stock\n", 243);
            printf("4. Configurar Stock M%cnimo\n", 237);
            printf("5. Volver\n");
        } else {
            printf("3. Volver\n");
        }

        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                listarStockProductos();
                break;
            case 2:
                visualizarStockBajo();
                break;
        }
        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 3:
                    registrarReposicionStock();
                    break;
                case 4:
                    modificarStockMinimo();
                    break;
                case 5:
                    return;
            }
        } else {
            switch (opcion) {
                case 3:
                    return;
            }
        }

    } while (1);
}

void menu_descuentos_promociones() {
    int opcion;
    do {
        printf("\n--- Descuentos y Promociones ---\n");
        printf("1. Aplicar Descuento a Producto\n");
        printf("2. Configurar Fechas de Descuento\n");
        printf("3. Ver Productos con Descuento\n");
        printf("4. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                aplicarDescuentoProducto();
                break;
            case 2:
                configurarFechasDescuento();
                break;
            case 3:
                verProductosConDescuento();
                break;
            case 4:
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 4);
}

void menu_ventas() {
    int role;
    role = obtener_rol(usuario_global.usuario);
    OpcionMenu opciones[] = {
        {"Nueva Venta", registrarVentaMenu, ROL_VENDEDOR},
        {"Historial de Ventas", visualizarHistorialVentas, ROL_ADMIN},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= VENTAS =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men%c Principal\n", 233);
        printf("=====================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            return;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_gestion_caja(int role) {
    OpcionMenu opciones[] = {
        {"Apertura de Caja", menu_apertura_caja, ROL_VENDEDOR},
        {"Ingresos/Egresos", menu_ingresos_egresos, ROL_VENDEDOR},
        {"Corte de Caja", menu_corte_caja, ROL_VENDEDOR},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= CAJA =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men%c Principal\n", 233);
        printf("====================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            return;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_apertura_caja() {
    int opcion;
    do {
        printf("\n--- Apertura de Caja ---\n");
        printf("1. Iniciar Nueva Caja\n");
        printf("2. Ver Estado Actual\n");
        printf("3. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuAperturaCaja();
                break;
            case 2:
                mostrarEstadoCajaODarOpcionApertura();
                break;
            case 3:
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 3);
}

void menu_ingresos_egresos() {
    int role;
    role = obtener_rol(usuario_global.usuario);
    int opcion;
    do {
        printf("\n--- Ingresos/Egresos ---\n");
        printf("1. Registrar Ingreso\n");

        // Solo admin puede registrar egresos
        if (role == ROL_ADMIN) {
            printf("2. Registrar Egreso\n");
            printf("3. Ver Historial de Transacciones\n");
            printf("4. Volver\n");
        } else {
            printf("2. Volver\n");
        }

        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                    registrarIngreso();
                    break;
                case 2:
                    registrarEgreso();
                    break;
                case 3:
                    listarTransacciones();
                    break;
                case 4:
                    return;
                default:
                    printf("Opci%cn inv%clida.\n", 243, 160);
            }
        } else {
            switch (opcion) {
                case 1:
                    registrarIngreso();
                    break;
                case 2:
                    return;
                default:
                    printf("Opci%cn inv%clida.\n", 243, 160);
            }
        }
    } while (1);
}

void menu_corte_caja() {
    int opcion;
    int role;
    role = obtener_rol(usuario_global.usuario);
    do {
        printf("\n--- Corte de Caja ---\n");
        printf("1. Realizar Corte\n");
        printf("2. Ver %cltimo Corte\n", 233);

        // Solo admin puede ver todos los cortes
        if (role == ROL_ADMIN) {
            printf("3. Ver Historial de Cortes\n");
            printf("4. Volver\n");
        } else {
            printf("3. Volver\n");
        }

        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                    realizarCorteCaja();
                    break;
                case 2:
                    mostrarUltimoCorteCaja();
                    break;
                case 3:
                    mostrarHistorialCortesCaja();
                    break;
                case 4:
                    return;
                default:
                    printf("Opci%cn inv%clida.\n", 243, 160);
            }
        } else {
            switch (opcion) {
                case 1:
                    realizarCorteCaja();
                    break;
                case 2:
                    mostrarUltimoCorteCaja();
                    break;
                case 3:
                    return;
                default:
                    printf("Opci%cn inv%clida.\n", 243, 160);
            }
        }
    } while (1);
}

void menu_reportes_estadisticas() {
    int role;
    role = obtener_rol(usuario_global.usuario);
    OpcionMenu opciones[] = {
        {"Reportes de Ventas", menu_reportes_ventas, ROL_VENDEDOR},
        {"Reportes de Inventario", menu_reportes_inventario, ROL_VENDEDOR},
        {"Reportes de Caja", menu_reportes_caja, ROL_ADMIN},
        {"Exportación de Datos", menu_exportacion_datos, ROL_ADMIN}
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= REPORTES =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men%c Principal\n", 233);
        printf("========================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            return;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_reportes_ventas() {
    int role;
    role = obtener_rol(usuario_global.usuario);
    mostrarMenuReportesVentas(role);
}

void menu_reportes_inventario() {
    int role;
    role = obtener_rol(usuario_global.usuario);
    menuReportesInventario(role);
}

void menu_reportes_caja() {
    menuReportesCaja();
}

void menu_exportacion_datos() {
    int opcion;
    do {
        printf("\n--- Exportaci%cn de Datos ---\n", 243);
        printf("1. Exportar a PDF (No implementado)\n");
        printf("2. Exportar a Excel (No implementado)\n");
        printf("3. Exportar a CSV (No implementado)\n");
        printf("4. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Funci%cn a%cn no implementada.\n", 243, 161);
                break;
            case 4:
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 4);
}

void menu_administracion(int role) {
    OpcionMenu opciones[] = {
        {"Gesti\243n de Usuarios", menu_gestion_usuarios, ROL_ADMIN},
        {"Registros de Auditor%ca", menu_registro_auditoria, ROL_ADMIN},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= ADMINISTRACI%cN =======\n", 243);
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men%c Principal\n", 233);
        printf("==============================\n");
        printf("Seleccione una opci%cn: ", 243);

        scanf("%d", &opcion);

        // Ajustar opción basado en permisos visibles
        int opcion_real = -1;
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                if (opcion == num_opcion++) {
                    opcion_real = i;
                    break;
                }
            }
        }

        if (opcion_real >= 0) {
            opciones[opcion_real].funcion(role);
        } else if (opcion == num_opcion) {
            return;
        } else {
            printf("Opci%cn inv%clida. Intente nuevamente.\n", 243, 160);
        }
    } while (1);
}

void menu_gestion_usuarios() {
    int opcion, role, success;
    char nombre_usuario[USUARIO_LENGTH_C];

    do {
        printf("\n--- Gesti%cn de Usuarios ---\n", 243);
        printf("1. Ver Usuarios\n");
        printf("2. Crear Usuario\n");
        printf("3. Cambiar Estatus de Usuario\n");
        printf("4. Asignar Roles\n");
        printf("5. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                listarUsuarios();
                break;

            case 2:
                while (1) { // Bucle para reintentar
                    printf("Ingrese el nombre del usuario: ");
                    scanf("%s", nombre_usuario);

                    printf("Ingrese que rol quiere que tenga el usuario [2 - Admin / 1 - Vendedor]: ");
                    // Usar `scanf` con "&role" para leer la entrada correctamente
                    if (scanf("%d", &role) != 1) {
                        // Si no se lee correctamente, mostrar mensaje de error
                        printf("Entrada inv%clida para el rol. Intente de nuevo.\n", 160);
                        continue; // Volver a pedir la entrada
                    }

                    success = guardarUsuario(nombre_usuario, role);
                    if (success) {
                        printf("Usuario guardado exitosamente.\n");
                        break; // Salir del bucle si éxito
                    } else {
                        printf("Error al guardar el usuario. Intente de nuevo.\n");
                        // Aquí puedes agregar opciones adicionales, como preguntar si quiere reintentar o cancelar
                    }
                }
                break;
            case 3:
                cambiarEstatusUsuarioMenu();
                break;
            case 4:
                cambiarRolUsuarioMenu();
                break;
            case 5:
                return;
                break;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 6);
}

void menu_registro_auditoria() {
    int opcion;
    do {
        printf("\n--- Registro de Auditor%ca ---\n", 237);
        printf("1. Ver Historial de Acciones\n");
        printf("2. B%csqueda Avanzada\n", 250);
        printf("3. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                solicitarYListarAuditoria();
                break;
            case 2:
                busquedaAvanzada();
                break;
            case 3:
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 3);
}

void menu_mi_cuenta() {
    int opcion;
    do {
        printf("\n--- Mi cuenta ---\n");
        printf("1. Cambiar contrase%ca\n", 164);
        printf("2. Volver\n");
        printf("Seleccione una opci%cn: ", 243);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Funci%cn cambiar contrase%ca.\n", 243, 164);
                cambiar_contrasena();
                break;
            case 2:
                return;
            default:
                printf("Opci%cn inv%clida.\n", 243, 160);
        }
    } while (opcion != 2);
}