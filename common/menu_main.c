#include <stdio.h>
#include "logo.h"
#include "../include/ventas.h"
#include "../include/reportes.h"
// Definimos constantes para roles
#define ROL_VENDEDOR 1
#define ROL_ADMIN 2

#define T 100
// Prototipos de funciones
void menu_inventario(int role);
void menu_ventas(int role);
void menu_gestion_caja(int role);
void menu_reportes_estadisticas(int role);
void menu_administracion(int role);

void menu_administracion_productos();
void menu_control_stock(int role);
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


// Definir estructura de opción de menú
typedef struct {
    char *nombre;
    void (*funcion)(int);
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
        {"Administración", menu_administracion, ROL_ADMIN},
        {"Mi cuenta", menu_mi_cuenta, ROL_VENDEDOR},
        
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
        printf("%d. Salir\n", num_opcion);
        printf("==============================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_inventario(int role) {
    OpcionMenu opciones[] = {
        {"Gestión de Productos", menu_administracion_productos, ROL_ADMIN},
        {"Control de Stock", menu_control_stock, ROL_VENDEDOR},
        {"Descuentos y Promociones", menu_descuentos_promociones, ROL_ADMIN},
        {"Categorías", menu_categorias, ROL_ADMIN}
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
        printf("%d. Volver al Menú Principal\n", num_opcion);
        printf("==========================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_administracion_productos() {
    int opcion;
    do {
        printf("\n--- Gestión de Productos ---\n");
        printf("1. Registrar Nuevo Producto\n");
        printf("2. Actualizar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrar_producto(2); // Asumiendo que esta función existe
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

void menu_control_stock(int role) {
    int opcion;
    do {
        printf("\n--- Control de Stock ---\n");
        printf("1. Ver Stock Actual\n");
        printf("2. Ver Productos con Bajo Stock\n");
        
        // Solo mostrar estas opciones al administrador
        if (role == ROL_ADMIN) {
            printf("3. Registrar Reposición de Stock\n");
            printf("4. Configurar Stock Mínimo\n");
            printf("5. Volver\n");
        } else {
            printf("3. Volver\n");
        }
        
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                    printf("Función de Ver Stock aún no implementada.\n");
                    break;
                case 2:
                    printf("Función de Ver Bajo Stock aún no implementada.\n");
                    break;
                case 3:
                    printf("Función de Reposición aún no implementada.\n");
                    break;
                case 4:
                    printf("Función de Configuración de Stock aún no implementada.\n");
                    break;
                case 5:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                    printf("Función de Ver Stock aún no implementada.\n");
                    break;
                case 2:
                    printf("Función de Ver Bajo Stock aún no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opción inválida.\n");
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
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Función aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_categorias() {
    int opcion;
    do {
        printf("\n--- Categorías ---\n");
        printf("1. Ver Categorías\n");
        printf("2. Crear Nueva Categoría\n");
        printf("3. Modificar Categoría\n");
        printf("4. Eliminar Categoría\n");
        printf("5. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Función aún no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 5);
}

void menu_ventas(int role) {
    OpcionMenu opciones[] = {
        {"Nueva Venta", menu_nueva_venta, ROL_VENDEDOR},
        {"Historial de Ventas", menu_historial_ventas, ROL_VENDEDOR},
        {"Cancelación de Ventas", menu_cancelacion_ventas, ROL_ADMIN}
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
        printf("%d. Volver al Menú Principal\n", num_opcion);
        printf("=====================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
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
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Función aún no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opción inválida.\n");
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
        
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                case 4:
                    printf("Función aún no implementada.\n");
                    break;
                case 5:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Función aún no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        }
    } while (1);
}

void menu_cancelacion_ventas() {
    int opcion;
    do {
        printf("\n--- Cancelación de Ventas ---\n");
        printf("1. Buscar Venta a Cancelar\n");
        printf("2. Justificar Cancelación\n");
        printf("3. Ver Historial de Cancelaciones\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Función aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
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
        printf("%d. Volver al Menú Principal\n", num_opcion);
        printf("====================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
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
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
                printf("Función aún no implementada.\n");
                break;
            case 3:
                return;
            default:
                printf("Opción inválida.\n");
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
        
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Función aún no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                    printf("Función aún no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        }
    } while (1);
}

void menu_corte_caja(int role) {
    int opcion;
    do {
        printf("\n--- Corte de Caja ---\n");
        printf("1. Realizar Corte\n");
        printf("2. Ver Último Corte\n");
        
        // Solo admin puede ver todos los cortes
        if (role == ROL_ADMIN) {
            printf("3. Ver Historial de Cortes\n");
            printf("4. Volver\n");
        } else {
            printf("3. Volver\n");
        }
        
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            switch (opcion) {
                case 1:
                case 2:
                case 3:
                    printf("Función aún no implementada.\n");
                    break;
                case 4:
                    return;
                default:
                    printf("Opción inválida.\n");
            }
        } else {
            switch (opcion) {
                case 1:
                case 2:
                    printf("Función aún no implementada.\n");
                    break;
                case 3:
                    return;
                default:
                    printf("Opción inválida.\n");
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
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
                printf("Función aún no implementada.\n");
                break;
            case 3:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 3);
}

void menu_reportes_estadisticas(int role) {
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
        printf("%d. Volver al Menú Principal\n", num_opcion);
        printf("========================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
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
        
        // Mostrar opciones adicionales para administrador
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
		
		switch(opcion){
			case 1:
				mostrar_ventas_dia();
				break;
			case 2:
				printf("\n Ingrese la fecha apartir la cual quiere filtrar las venta: ");
				scanf("%s", &fecha);
				mostar_ventas_semana(fecha);
				return;
			case 3:
				printf("\n Ingrese el año: ");
				scanf("%d", &anio);
				printf("\n Ingrese el mes: ");
				scanf("%d", &mes);
				mostrar_ventas_anio(mes, anio);
				return;
			default:
				return;
        if (role == ROL_ADMIN) {
 
			}
        } else {
 			switch(opcion){
        		case 4:
        			break;
        		case 5:
					return;
				default:
					return;
						
			}
        }
    } while (1);
}

void menu_reportes_inventario(int role) {
    int opcion;
    do {
        printf("\n--- Reportes de Inventario ---\n");
        printf("1. Stock Actual\n");
        printf("2. Productos más Vendidos\n");
        printf("3. Productos menos Vendidos\n");
        
        // Mostrar opciones adicionales para administrador
        if (role == ROL_ADMIN) {
            printf("4. Rotación de Inventario\n");
            printf("5. Productos por Categoría\n");
            printf("6. Volver\n");
        } else {
            printf("4. Volver\n");
        }
        
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (role == ROL_ADMIN) {
            if (opcion >= 1 && opcion <= 5) {
                printf("Función aún no implementada.\n");
            } else if (opcion == 6) {
                return;
            } else {
                printf("Opción inválida.\n");
            }
        } else {
            if (opcion >= 1 && opcion <= 3) {
                printf("Función aún no implementada.\n");
            } else if (opcion == 4) {
                return;
            } else {
                printf("Opción inválida.\n");
            }
        }
    } while (1);
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
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Función aún no implementada.\n");
                break;
            case 5:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 5);
}

void menu_exportacion_datos() {
    int opcion;
    do {
        printf("\n--- Exportación de Datos ---\n");
        printf("1. Exportar a PDF\n");
        printf("2. Exportar a Excel\n");
        printf("3. Exportar a CSV\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
                printf("Función aún no implementada.\n");
                break;
            case 4:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 4);
}

void menu_administracion(int role) {
    OpcionMenu opciones[] = {
        {"Gestión de Usuarios", menu_gestion_usuarios, ROL_ADMIN},
        {"Registro de Auditoría", menu_registro_auditoria, ROL_ADMIN},
    };

    int total_opciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion;
    int i, num_opcion;
    do {
        printf("\n======= ADMINISTRACIÓN =======\n");
        for (i = 0, num_opcion = 1; i < total_opciones; i++) {
            if (tiene_permiso(role, opciones[i].permiso_minimo)) {
                printf("%d. %s\n", num_opcion++, opciones[i].nombre);
            }
        }
        printf("%d. Volver al Menú Principal\n", num_opcion);
        printf("==============================\n");
        printf("Seleccione una opción: ");

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
            printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (1);
}

void menu_gestion_usuarios() {
    int opcion;
    do {
        printf("\n--- Gestión de Usuarios ---\n");
        printf("1. Ver Usuarios\n");
        printf("2. Crear Usuario\n");
        printf("3. Modificar Usuario\n");
        printf("4. Desactivar Usuario\n");
        printf("5. Asignar Roles\n");
        printf("6. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                printf("Función aún no implementada.\n");
                break;
            case 6:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 6);
}

void menu_registro_auditoria() {
    int opcion;
    do {
        printf("\n--- Registro de Auditoría ---\n");
        printf("1. Ver Historial de Acciones\n");
        printf("2. Búsqueda Avanzada\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n?? Ver Historial de Acciones\n");
                printf("   1. Filtrar por usuario\n");
                printf("   2. Filtrar por fecha\n");
                printf("   3. Filtrar por tipo de acción\n");
                printf("   4. Volver\n");
                break;
            case 2:
                printf("\n?? Búsqueda Avanzada\n");
                printf("   1. Buscar por palabra clave\n");
                printf("   2. Buscar por nombre de usuario\n");
                printf("   3. Volver\n");
                break;

            case 3:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 3);
}


void menu_mi_cuenta(){
	int opcion;
    do {
        printf("\n--- Mi cuenta ---\n");
        printf("1. Cambiar contrasena\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
				printf("Función cambiar contraseña");
				cambiar_contrasena();
                break;
            case 3:
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 3);
}
