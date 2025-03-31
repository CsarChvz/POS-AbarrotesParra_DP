#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h"

#define MAX_VENTAS 100
#define MAX_VENTAS_PRODUCTOS 200
#define ROL_ADMIN 1 // Asumiendo que 1 representa el rol de administrador

#define ARCHIVOS_VENTAS "common/data/ventas.csv"
#define ARCHIVOS_VENTAS_PRODUCTOS "common/data/ventas_productos.csv"
#define ARCHIVOS_PRODUCTOS "common/data/productos.csv"

// Estructuras para almacenar los datos de ventas
typedef struct {
    int idVenta;
    char fechaVenta[11];
    char metodoPago[20];
    int idUsuario;
    float precioTotal;
} Venta;

typedef struct {
    int idVP;
    int idVenta;
    int idProducto;
    float cantidad;
    float precioUnitario;
    float subtotal;
} VentaProducto;

#define MAX_PRODUCTOS 100
#define MAX_VENTAS_PRODUCTOS 200

// Estructura para almacenar los datos de productos
typedef struct {
    int id;
    char nombre[50];
    float precio;
    int stock;
    int activo;
    char unidad[20];
    int valorUnidad;
    char codigoBarras[14];
    int stockMinimo;
} Producto;


// Funciones para leer datos de archivos CSV
int leerVentas(Venta ventas[]) {
    FILE *archivo = fopen(ARCHIVOS_VENTAS, "r");
    if (archivo == NULL) {
        perror("Error al abrir ventas.csv");
        return 0;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    int numVentas = 0;
    while (fgets(linea, sizeof(linea), archivo) && numVentas < MAX_VENTAS) {
        sscanf(linea, "%d,%10[^,],%19[^,],%d,%f",
               &ventas[numVentas].idVenta, ventas[numVentas].fechaVenta,
               ventas[numVentas].metodoPago, &ventas[numVentas].idUsuario,
               &ventas[numVentas].precioTotal);
        numVentas++;
    }

    fclose(archivo);
    return numVentas;
}

int leerVentasProductos(VentaProducto ventasProductos[]) {
    FILE *archivo = fopen(ARCHIVOS_VENTAS_PRODUCTOS, "r");
    if (archivo == NULL) {
        perror("Error al abrir ventas_productos.csv");
        return 0;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    int numVentasProductos = 0;
    while (fgets(linea, sizeof(linea), archivo) && numVentasProductos < MAX_VENTAS_PRODUCTOS) {
        sscanf(linea, "%d,%d,%d,%f,%f,%f",
               &ventasProductos[numVentasProductos].idVP,
               &ventasProductos[numVentasProductos].idVenta,
               &ventasProductos[numVentasProductos].idProducto,
               &ventasProductos[numVentasProductos].cantidad,
               &ventasProductos[numVentasProductos].precioUnitario,
               &ventasProductos[numVentasProductos].subtotal);
        numVentasProductos++;
    }

    fclose(archivo);
    return numVentasProductos;
}

// Funciones para generar reportes de ventas
void reporteVentasPorDia(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    char dia[11];
    printf("Ingrese el dia (YYYY-MM-DD): ");
    scanf("%10s", dia);
	int i;
    float totalVentas = 0;
    for (i = 0; i < numVentas; i++) {
        if (strcmp(ventas[i].fechaVenta, dia) == 0) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total ventas del dia: %.2f\n", totalVentas);
}

void reporteVentasPorSemana(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    char fechaInicio[11], fechaFin[11];
    printf("Ingrese la fecha de inicio (YYYY-MM-DD): ");
    scanf("%10s", fechaInicio);
    printf("Ingrese la fecha de fin (YYYY-MM-DD): ");
    scanf("%10s", fechaFin);
	int i;
    float totalVentas = 0;
    for (i = 0; i < numVentas; i++) {
        if (strcmp(ventas[i].fechaVenta, fechaInicio) >= 0 && strcmp(ventas[i].fechaVenta, fechaFin) <= 0) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total ventas de la semana: %.2f\n", totalVentas);
}

void reporteVentasPorMes(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    char mes[8]; // YYYY-MM
    printf("Ingrese el mes (YYYY-MM): ");
    scanf("%7s", mes);
	int i;
    float totalVentas = 0;
    for (i = 0; i < numVentas; i++) {
        if (strncmp(ventas[i].fechaVenta, mes, 7) == 0) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total ventas del mes: %.2f\n", totalVentas);
}

void reporteVentasPorVendedor(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    int idVendedor;
    printf("Ingrese el ID del vendedor: ");
    scanf("%d", &idVendedor);
	int i;
    float totalVentas = 0;
    for ( i = 0; i < numVentas; i++) {
        if (ventas[i].idUsuario == idVendedor) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total ventas del vendedor: %.2f\n", totalVentas);
}

void reporteVentasPorProducto(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    int idProducto;
    printf("Ingrese el ID del producto: ");
    scanf("%d", &idProducto);
	int i, j;
    float totalVentas = 0;
    for (i = 0; i < numVentasProductos; i++) {
        if (ventasProductos[i].idProducto == idProducto) {
            // Encuentra la venta correspondiente para obtener el total
            for (j = 0; j < numVentas; j++) {
                if (ventas[j].idVenta == ventasProductos[i].idVenta) {
                    printf("Venta ID: %d, Total: %.2f\n", ventas[j].idVenta, ventas[j].precioTotal);
                    totalVentas += ventas[j].precioTotal;
                    break;
                }
            }
        }
    }
    printf("Total ventas del producto: %.2f\n", totalVentas);
}

void reporteVentasPorMetodoPago(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos) {
    char metodoPago[20];
    printf("Ingrese el metodo de pago: ");
    scanf("%19s", metodoPago);
	int i;
    float totalVentas = 0;
    for (i = 0; i < numVentas; i++) {
        if (strcmp(ventas[i].metodoPago, metodoPago) == 0) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total ventas con metodo de pago %s: %.2f\n", metodoPago, totalVentas);
}

void reporteMisVentas(Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos, int idUsuario) {
    float totalVentas = 0;
    int i;
    for (i = 0; i < numVentas; i++) {
        if (ventas[i].idUsuario == idUsuario) {
            printf("Venta ID: %d, Total: %.2f\n", ventas[i].idVenta, ventas[i].precioTotal);
            totalVentas += ventas[i].precioTotal;
        }
    }
    printf("Total mis ventas: %.2f\n", totalVentas);
}


// Función para mostrar el menú principal
void mostrarMenuReportesVentas(int role) {
    int opcion, idUsuario;
    idUsuario = usuario_global.id;
 
    Venta ventas[MAX_VENTAS];
    VentaProducto ventasProductos[MAX_VENTAS_PRODUCTOS];

    int numVentas = leerVentas(ventas);
    int numVentasProductos = leerVentasProductos(ventasProductos);
    do {
        printf("\n--- Reportes de Ventas ---\n");
        printf("1. Ventas por Dia\n");
        printf("2. Ventas por Semana\n");
        printf("3. Ventas por Mes\n");

        if (role == ROL_ADMIN) {
            printf("4. Ventas por Vendedor\n");
            printf("5. Ventas por Producto\n");
            printf("6. Ventas por Metodo de Pago\n");
            printf("7. Volver\n");
        } else {
            printf("4. Mis Ventas\n");
            printf("5. Volver\n");
        }

        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                reporteVentasPorDia(ventas, numVentas, ventasProductos, numVentasProductos);
                break;
            case 2:
                reporteVentasPorSemana(ventas, numVentas, ventasProductos, numVentasProductos);
                break;
            case 3:
                reporteVentasPorMes(ventas, numVentas, ventasProductos, numVentasProductos);
                break;
            case 4:
                if (role == ROL_ADMIN) {
                    reporteVentasPorVendedor(ventas, numVentas, ventasProductos, numVentasProductos);
                } else {
                    reporteMisVentas(ventas, numVentas, ventasProductos, numVentasProductos, idUsuario);
                }
                break;
            case 5:
                if (role == ROL_ADMIN) {
                    reporteVentasPorProducto(ventas, numVentas, ventasProductos, numVentasProductos);
                } else {
                    printf("Volviendo al menú principal...\n");
                }
                break;
            case 6:
                if (role == ROL_ADMIN) {
                    reporteVentasPorMetodoPago(ventas, numVentas, ventasProductos, numVentasProductos);
                } else {
                    printf("Opción inválida.\n");
                }
                break;
            case 7:
                if (role == ROL_ADMIN) {
                    printf("Volviendo al menú principal...\n");
                } else {
                    printf("Opción inválida.\n");
                }
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != (role == ROL_ADMIN ? 7 : 5));
}

// Funciones para leer datos de archivos CSV
int leerProductos(Producto productos[]) {
    FILE *archivo = fopen(ARCHIVOS_PRODUCTOS, "r");
    if (archivo == NULL) {
        perror("Error al abrir productos.csv");
        return 0;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera

    int numProductos = 0;
    while (fgets(linea, sizeof(linea), archivo) && numProductos < MAX_PRODUCTOS) {
        sscanf(linea, "%d,%49[^,],%f,%d,%d,%19[^,],%d,%13[^,],%d",
               &productos[numProductos].id, productos[numProductos].nombre,
               &productos[numProductos].precio, &productos[numProductos].stock,
               &productos[numProductos].activo, productos[numProductos].unidad,
               &productos[numProductos].valorUnidad, productos[numProductos].codigoBarras,
               &productos[numProductos].stockMinimo);
        numProductos++;
    }

    fclose(archivo);
    return numProductos;
}

// Funciones para generar reportes de inventario
void reporteStockActual(Producto productos[], int numProductos) {
    printf("\n--- Stock Actual ---\n");
    int i;
    for (i = 0; i < numProductos; i++) {
        printf("ID: %d, Nombre: %s, Stock: %d %s\n",
               productos[i].id, productos[i].nombre, productos[i].stock, productos[i].unidad);
    }
}

void reporteProductosMasVendidos(Producto productos[], int numProductos, VentaProducto ventasProductos[], int numVentasProductos) {
    int ventasPorProducto[MAX_PRODUCTOS] = {0};
    int i,j;
    // Contar ventas por producto
    for ( i = 0; i < numVentasProductos; i++) {
        for ( j = 0; j < numProductos; j++) {
            if (ventasProductos[i].idProducto == productos[j].id) {
                ventasPorProducto[j]++;
                break;
            }
        }
    }

    // Encontrar el producto más vendido
    int maxVentas = 0;
    int idProductoMasVendido = -1;
    for ( i = 0; i < numProductos; i++) {
        if (ventasPorProducto[i] > maxVentas) {
            maxVentas = ventasPorProducto[i];
            idProductoMasVendido = productos[i].id;
        }
    }

    // Mostrar el producto más vendido
    if (idProductoMasVendido != -1) {
        printf("\n--- Producto más Vendido ---\n");
        for ( i = 0; i < numProductos; i++) {
            if (productos[i].id == idProductoMasVendido) {
                printf("ID: %d, Nombre: %s, Ventas: %d\n", productos[i].id, productos[i].nombre, maxVentas);
                break;
            }
        }
    } else {
        printf("\nNo hay ventas registradas.\n");
    }
}

void reporteProductosMenosVendidos(Producto productos[], int numProductos, VentaProducto ventasProductos[], int numVentasProductos) {
    int ventasPorProducto[MAX_PRODUCTOS] = {0};
    int i, j;
    // Contar ventas por producto
    for ( i = 0; i < numVentasProductos; i++) {
        for ( j = 0; j < numProductos; j++) {
            if (ventasProductos[i].idProducto == productos[j].id) {
                ventasPorProducto[j]++;
                break;
            }
        }
    }

    // Encontrar el producto menos vendido
    int minVentas = numVentasProductos + 1;
    int idProductoMenosVendido = -1;
    for ( i = 0; i < numProductos; i++) {
        if (ventasPorProducto[i] < minVentas) {
            minVentas = ventasPorProducto[i];
            idProductoMenosVendido = productos[i].id;
        }
    }

    // Mostrar el producto menos vendido
    if (idProductoMenosVendido != -1) {
        printf("\n--- Producto menos Vendido ---\n");
        for ( i = 0; i < numProductos; i++) {
            if (productos[i].id == idProductoMenosVendido) {
                printf("ID: %d, Nombre: %s, Ventas: %d\n", productos[i].id, productos[i].nombre, minVentas);
                break;
            }
        }
    } else {
        printf("\nNo hay ventas registradas.\n");
    }
}

// Función para mostrar el menú de reportes de inventario
void menuReportesInventario(int role) {
    int opcion;
    Producto productos[MAX_PRODUCTOS];
    VentaProducto ventasProductos[MAX_VENTAS_PRODUCTOS];

    int numProductos = leerProductos(productos);
    int numVentasProductos = leerVentasProductos(ventasProductos);

    do {
        printf("\n--- Reportes de Inventario ---\n");
        printf("1. Stock Actual\n");
        printf("2. Productos más Vendidos\n");
        printf("3. Productos menos Vendidos\n");

        if (role == 1) { // Asumiendo que 1 representa el rol de administrador
            printf("4. Volver\n");
        } else {
            printf("4. Volver\n");
        }

        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                reporteStockActual(productos, numProductos);
                break;
            case 2:
                reporteProductosMasVendidos(productos, numProductos, ventasProductos, numVentasProductos);
                break;
            case 3:
                reporteProductosMenosVendidos(productos, numProductos, ventasProductos, numVentasProductos);
                break;
            case 4:
                return; // Volver
            default:
                printf("Opción inválida.\n");
        }
    } while (1);
}
