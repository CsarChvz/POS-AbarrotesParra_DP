#include <stdio.h>
#include <string.h>

#include "logo.h"
#include "../include/ventas.h"
#include "../include/reportes.h"
#include "../include/gestion_usuarios.h"
#include "../include/inventario.h"
#include "../include/seguridad.h"
// Definimos constantes para roles
#define ROL_VENDEDOR 1
#define ROL_ADMIN 2
#define T 100
#define USUARIO_LENGTH_C 50
// Prototipos de funciones
void menu_inventario(int role);
void menu_ventas(int role);
void menu_gestion_caja(int role);
void menu_reportes_estadisticas(int role);
void menu_administracion(int role);

void menu_administracion_productos();
void menu_control_stock();
void menu_descuentos_promociones();
void menu_categorias();
void menu_nueva_venta();
void menu_historial_ventas(int role);
void menu_cancelacion_ventas();
void menu_apertura_caja();
void menu_ingresos_egresos(int role);
void menu_corte_caja(int role);
void menu_retiro_efectivo();
void menu_reportes_ventas(int role);
void menu_reportes_inventario(int role);
void menu_reportes_caja();
void menu_exportacion_datos();
void menu_gestion_usuarios();

void menu_registro_auditoria();

void menu_mi_cuenta();


// Definir estructura de opci�n de men�
typedef struct {
    char *nombre;
    void (*funcion)(int role);
    int permiso_minimo; // 1 = Vendedor, 2 = Admin
} OpcionMenu;

// Funci�n para verificar permisos
int tiene_permiso(int role, int permiso_minimo) {
    return role >= permiso_minimo;
}

// Men� principal
void menu_main(int role) {
    OpcionMenu opciones[] = {
        {"Inventario", menu_inventario, ROL_VENDEDOR},
        {"Ventas", menu_ventas, ROL_VENDEDOR},
        {"Caja", menu_gestion_caja, ROL_VENDEDOR},
        {"Reportes", menu_reportes_estadisticas, ROL_VENDEDOR},
        {"Administraci�n", menu_administracion, ROL_ADMIN},
        {"Mi cuenta", menu_mi_cuenta, ROL_VENDEDOR},
        
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
        printf("%d. Salir\n", num_opcion);
        printf("==============================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_inventario(int role) {
    OpcionMenu opciones[] = {
        {"Gesti�n de Productos", menu_administracion_productos, ROL_ADMIN},
        {"Control de Stock", menu_control_stock, ROL_VENDEDOR},
        {"Descuentos y Promociones", menu_descuentos_promociones, ROL_ADMIN},
        {"Categor�as", menu_categorias, ROL_ADMIN}
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
        printf("%d. Volver al Men� Principal\n", num_opcion);
        printf("==========================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (1);
}


void menu_administracion_productos() {
    int opcion;
    do {
        printf("\n--- Gestión de Productos ---\n");
        printf("1. Registrar Nuevo Producto\n");
        printf("2. Visualizar productos\n");
        printf("3. Actualizar Producto\n");
        printf("4. Eliminar Producto\n");
        printf("5. Volver\n");
        printf("Seleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            // Si la entrada no es válida, limpiamos el buffer y mostramos un error
            while (getchar() != '\n');  // Limpiar buffer
            printf("Error: Entrada inválida. Por favor, ingrese un número.\n");
            continue;  // Volver al inicio del menú
        }

        switch (opcion) {
            case 1:
                registrarProductoMenu();
                break;  
            case 2:
                visualizarProductosMenu();
                break;
            case 3:
                printf("Función de Actualizacion aún no implementada.\n");
                // @TODO: 
                break;
            case 4:
                eliminarProductoMenu();
                break;
            case 5:
                printf("Volviendo...\n");
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);  // Seguir mostrando el menú hasta que se elija salir
}


void menu_control_stock() {
    int opcion;
    int role;
    role = obtener_rol(usuario_actual);
    do {
        printf("\n--- Control de Stock ---\n");
        printf("1. Ver Stock Actual\n");
        printf("2. Ver Productos con Bajo Stock\n");
        
        // Solo mostrar estas opciones al administrador
        if (role == ROL_ADMIN) {
            printf("3. Registrar Reposici�n de Stock\n");
            printf("4. Configurar Stock M�nimo\n");
            printf("5. Volver\n");
        } else {
            printf("3. Volver\n");
        }
        
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                    printf("Funci�n de Ver Stock a�n no implementada.\n");
                    break;
                case 2:
                    printf("Funci�n de Ver Bajo Stock a�n no implementada.\n");
                    break;
                case 3:
                    printf("Funci�n de Reposici�n a�n no implementada.\n");
                    break;
                case 4:
                    printf("Funci�n de Configuraci�n de Stock a�n no implementada.\n");
                    break;
                case 5:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                    printf("Funci�n de Ver Stock a�n no implementada.\n");
                    break;
                case 2:
                    printf("Funci�n de Ver Bajo Stock a�n no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
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
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_categorias() {
    int opcion;
    do {
        printf("\n--- Categor�as ---\n");
        printf("1. Ver Categor�as\n");
        printf("2. Crear Nueva Categor�a\n");
        printf("3. Modificar Categor�a\n");
        printf("4. Eliminar Categor�a\n");
        printf("5. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 5);
}

void menu_ventas(int role) {
    OpcionMenu opciones[] = {
        {"Nueva Venta", menu_nueva_venta, ROL_VENDEDOR},
        {"Historial de Ventas", menu_historial_ventas, ROL_VENDEDOR},
        {"Cancelaci�n de Ventas", menu_cancelacion_ventas, ROL_ADMIN}
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
        printf("%d. Volver al Men� Principal\n", num_opcion);
        printf("=====================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_nueva_venta() {
    int opcion;
    do {
        printf("\n--- Nueva Venta ---\n");
        printf("1. Buscar y Seleccionar Productos\n");
        printf("2. Ver Carrito\n");
        printf("3. Procesar Pago\n");
        printf("4. Cancelar Venta\n");
        printf("5. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 5);
}

void menu_historial_ventas(int role) {
    int opcion;
    do {
        printf("\n--- Historial de Ventas ---\n");
        printf("1. Ver Mis Ventas\n");
        
        // Solo admin puede ver todas las ventas
        if (role == ROL_ADMIN) {
            printf("2. Ver Todas las Ventas\n");
            printf("3. Buscar Venta por ID\n");
            printf("4. Filtrar por Fecha\n");
            printf("5. Volver\n");
        } else {
            printf("2. Buscar Venta por ID\n");
            printf("3. Filtrar por Fecha\n");
            printf("4. Volver\n");
        }
        
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                case 4:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 5:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        }
    } while (1);
}

void menu_cancelacion_ventas() {
    int opcion;
    do {
        printf("\n--- Cancelaci�n de Ventas ---\n");
        printf("1. Buscar Venta a Cancelar\n");
        printf("2. Justificar Cancelaci�n\n");
        printf("3. Ver Historial de Cancelaciones\n");
        printf("4. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_gestion_caja(int role) {
    OpcionMenu opciones[] = {
        {"Apertura de Caja", menu_apertura_caja, ROL_VENDEDOR},
        {"Ingresos/Egresos", menu_ingresos_egresos, ROL_VENDEDOR},
        {"Corte de Caja", menu_corte_caja, ROL_VENDEDOR},
        {"Retiro de Efectivo", menu_retiro_efectivo, ROL_ADMIN}
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
        printf("%d. Volver al Men� Principal\n", num_opcion);
        printf("====================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
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
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 3:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 3);
}

void menu_ingresos_egresos(int role) {
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
            printf("2. Ver Mis Transacciones\n");
            printf("3. Volver\n");
        }
        
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        }
    } while (1);
}

void menu_corte_caja(int role) {
    int opcion;
    do {
        printf("\n--- Corte de Caja ---\n");
        printf("1. Realizar Corte\n");
        printf("2. Ver �ltimo Corte\n");
        
        // Solo admin puede ver todos los cortes
        if (role == ROL_ADMIN) {
            printf("3. Ver Historial de Cortes\n");
            printf("4. Volver\n");
        } else {
            printf("3. Volver\n");
        }
        
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                    printf("Funci�n a�n no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opci�n inv�lida.\n");
            }
        }
    } while (1);
}

void menu_retiro_efectivo() {
    int opcion;
    do {
        printf("\n--- Retiro de Efectivo ---\n");
        printf("1. Registrar Nuevo Retiro\n");
        printf("2. Ver Historial de Retiros\n");
        printf("3. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 3:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 3);
}

void menu_reportes_estadisticas(int role) {
    OpcionMenu opciones[] = {
        {"Reportes de Ventas", menu_reportes_ventas, ROL_VENDEDOR},
        {"Reportes de Inventario", menu_reportes_inventario, ROL_VENDEDOR},
        {"Reportes de Caja", menu_reportes_caja, ROL_ADMIN},
        {"Exportaci�n de Datos", menu_exportacion_datos, ROL_ADMIN}
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
        printf("%d. Volver al Men� Principal\n", num_opcion);
        printf("========================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (1);
}
void menu_reportes_ventas(int role) {
    int opcion;
    char fecha[11];
    int mes, anio;

    do {
        printf("\n--- Reportes de Ventas ---\n");
        printf("1. Ventas por Día\n");
        printf("2. Ventas por Semana\n");
        printf("3. Ventas por Mes\n");

        if (role == ROL_ADMIN) {
            printf("4. Ventas por Vendedor\n");
            printf("5. Ventas por Producto\n");
            printf("6. Ventas por Método de Pago\n");
            printf("7. Volver\n");
        } else {
            printf("4. Mis Ventas\n");
            printf("5. Volver\n");
        }

        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

    //     switch (opcion) {
    //         case 1:
    //             mostrar_ventas_dia();
    //             break;
    //         case 2:
    //             printf("\nIngrese la fecha a partir de la cual quiere filtrar las ventas (YYYY-MM-DD): ");
    //             scanf("%s", fecha);  // No usar & con cadenas
    //             mostrar_ventas_semana(fecha);
    //             break;
    //         case 3:
    //             printf("\nIngrese el año: ");
    //             scanf("%d", &anio);
    //             printf("\nIngrese el mes: ");
    //             scanf("%d", &mes);
    //             mostrar_ventas_anio(mes, anio);
    //             break;
    //         case 4:
    //             if (role == ROL_ADMIN) {
    //                 printf("Función: Ventas por vendedor (aún no implementada).\n");
    //             } else {
    //                 printf("Función: Mis ventas (aún no implementada).\n");
    //             }
    //             break;
    //         case 5:
    //             if (role == ROL_ADMIN) {
    //                 printf("Función: Ventas por producto (aún no implementada).\n");
    //             } else {
    //                 return;  // Salir del menú si no es admin
    //             }
    //             break;
    //         case 6:
    //             if (role == ROL_ADMIN) {
    //                 printf("Función: Ventas por método de pago (aún no implementada).\n");
    //             } else {
    //                 printf("Opción inválida.\n");
    //             }
    //             break;
    //         case 7:
    //             if (role == ROL_ADMIN) {
    //                 return;
    //             }
    //             printf("Opción inválida.\n");
    //             break;
    //         default:
    //             printf("Opción inválida.\n");
    //     }
     } while (1);
}


void menu_reportes_inventario(int role) {
    int opcion;
    do {
        printf("\n--- Reportes de Inventario ---\n");
        printf("1. Stock Actual\n");
        printf("2. Productos m�s Vendidos\n");
        printf("3. Productos menos Vendidos\n");

    
        // Mostrar opciones adicionales para administrador
        if (role == ROL_ADMIN) {
            printf("4. Rotaci�n de Inventario\n");
            printf("5. Productos por Categor�a\n");
            printf("6. Volver\n");
        } else {
            printf("4. Volver\n");
        }

        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            if (opcion >= 1 && opcion <= 5) {
                printf("Funci�n a�n no implementada.\n");
            } else if (opcion == 6) {
                return; // Salir del men�
            } else {
                printf("Opci�n inv�lida.\n");
            }
        } else {
            if (opcion >= 1 && opcion <= 3) {
                printf("Funci�n a�n no implementada.\n");
            } else if (opcion == 4) {
                return; // Salir del men� para usuarios no admin
            } else {
                printf("Opci�n inv�lida.\n");
            }
        }
    } while (1); // El bucle sigue hasta que se elija "Volver"
}


void menu_reportes_caja() {
    int opcion;
    do {
        printf("\n--- Reportes de Caja ---\n");
        printf("1. Balance Diario\n");
        printf("2. Balance Semanal\n");
        printf("3. Balance Mensual\n");
        printf("4. Reporte de Ingresos/Egresos\n");
        printf("5. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 5);
}

void menu_exportacion_datos() {
    int opcion;
    do {
        printf("\n--- Exportaci�n de Datos ---\n");
        printf("1. Exportar a PDF\n");
        printf("2. Exportar a Excel\n");
        printf("3. Exportar a CSV\n");
        printf("4. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Funci�n a�n no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 4);
}

void menu_administracion(int role) {
    OpcionMenu opciones[] = {
        {"Gesti�n de Usuarios", menu_gestion_usuarios, ROL_ADMIN},
        {"Registro de Auditor�a", menu_registro_auditoria, ROL_ADMIN},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= ADMINISTRACI�N =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Men� Principal\n", num_opcion);
        printf("==============================\n");
        printf("Seleccione una opci�n: ");

        scanf("%d", &opcion);

        // Ajustar opci�n basado en permisos visibles
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
            printf("Opci�n inv�lida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_gestion_usuarios() {
    int opcion, role, success;
    char nombre_usuario[USUARIO_LENGTH_C];

    do {
        printf("\n--- Gesti�n de Usuarios ---\n");
        printf("1. Ver Usuarios\n");
        printf("2. Crear Usuario\n");
        printf("3. Cambiar Estatus de Usuario\n");
        printf("4. Asignar Roles\n");
        printf("5. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                listarUsuarios();
                break;
            case 2:
                printf("Ingrese el nombre del usuario: ");
                scanf("%s", nombre_usuario);
    
                printf("Ingrese que rol quiere que tenga el usuario [2 - Admin / 1 - Vendedor]: ");
                // Usar `scanf` con "&role" para leer la entrada correctamente
                if (scanf("%d", &role) != 1) {
                    // Si no se lee correctamente, mostrar mensaje de error
                    printf("Entrada inválida para el rol.\n");
                    break; // Salir del case si hay error
                }
    
                success = guardarUsuario(nombre_usuario, role);
                if (success) {
                    printf("Usuario guardado exitosamente.\n");
                } else {
                    printf("Error al guardar el usuario.\n");
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
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 6);
}

void menu_registro_auditoria() {
    int opcion;
    do {
        printf("\n--- Registro de Auditor�a ---\n");
        printf("1. Ver Historial de Acciones\n");
        printf("2. B�squeda Avanzada\n");
        printf("3. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n?? Ver Historial de Acciones\n");
                printf("   1. Filtrar por usuario\n");
                printf("   2. Filtrar por fecha\n");
                printf("   3. Filtrar por tipo de acci�n\n");
                printf("   4. Volver\n");
                break;
            case 2:
                printf("\n?? B�squeda Avanzada\n");
                printf("   1. Buscar por palabra clave\n");
                printf("   2. Buscar por nombre de usuario\n");
                printf("   3. Volver\n");
                break;

            case 3:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 3);
}


void menu_mi_cuenta(){
	int opcion;
    do {
        printf("\n--- Mi cuenta ---\n");
        printf("1. Cambiar contrasena\n");
        printf("3. Volver\n");
        printf("Seleccione una opci�n: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
				printf("Funci�n cambiar contrase�a");
				cambiar_contrasena();
                break;
            case 3:
                return;
            default:
                printf("Opci�n inv�lida.\n");
        }
    } while (opcion != 3);
}
