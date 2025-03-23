#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VENTAS 100
#define MAX_VENTAS_PRODUCTOS (MAX_VENTAS * 10)

#define NOMBRE_LENGTH 100
#define UNIDAD_LENGTH 20
#define CODIGO_BARRAS_LENGTH 20

typedef struct {
    int idVenta;
    time_t fechaVenta;
    char metodoPago[20];
    int idUsuario;
    float precioTotal;
} Venta;

typedef struct {
    int id;
    char nombre[NOMBRE_LENGTH];
    float precio;
    int stock;
    int activo;
    char unidad[UNIDAD_LENGTH];
    int valorUnidad;
    char codigoBarras[CODIGO_BARRAS_LENGTH];
    int stockMinimo;
} Producto;

typedef struct {
    int idVP;
    int idVenta;
    int idProducto;
    float cantidad;
    float precioUnitario;
    float subtotal;
} VentaProducto;

Producto *buscarProductoPorId(Producto productos[], int numProductos, int idProducto) {
	int i;
    for (i= 0; i < numProductos; i++) {
        if (productos[i].id == idProducto) {
            return &productos[i];
        }
    }
    return NULL;
}

void mostrarCarrito(VentaProducto ventasProductos[], int numVentasProductos, Producto productos[], int numProductos, int idVentaActual) {
    int i;
    printf("\n--- Carrito de Compra ---\n");
    for (i = 0; i < numVentasProductos; i++) {
        if (ventasProductos[i].idVenta == idVentaActual) {
            Producto *producto = buscarProductoPorId(productos, numProductos, ventasProductos[i].idProducto);
            if (producto != NULL) {
                printf("%s x%.2f %s: %.2f\n", producto->nombre, ventasProductos[i].cantidad, producto->unidad, ventasProductos[i].subtotal);
            }
        }
    }
}

void registrarVenta(Producto productos[], int numProductos, Venta ventas[], int *numVentas, VentaProducto ventasProductos[], int *numVentasProductos) {
    int opcion;
    int idProducto;
    float cantidad;
    float total = 0;
    int productoEncontrado = 0;
    int idVentaActual;
    Venta ventaActual;
    char metodoPago[20];
    int idUsuario;
    int i;

    ventaActual.idVenta = *numVentas + 1;
    ventaActual.fechaVenta = time(NULL);

    printf("\n--- Nueva Venta ---\n");

    do {
        mostrarCarrito(ventasProductos, *numVentasProductos, productos, numProductos, ventaActual.idVenta);

        printf("\n1. Agregar Producto (ID) | 2. Finalizar Venta | 3. Cancelar\n");
        printf("Ingrese opción: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("ID Producto: ");
            scanf("%d", &idProducto);
            printf("Cantidad (o Gramos): ");
            scanf("%f", &cantidad);

            productoEncontrado = 0;
            for (i = 0; i < numProductos; i++) {
                if (productos[i].id == idProducto) {
                    if (productos[i].stock >= cantidad || strcmp(productos[i].unidad, "Gramos") == 0) {
                        VentaProducto ventaProducto;
                        ventaProducto.idVP = *numVentasProductos + 1;
                        ventaProducto.idVenta = ventaActual.idVenta;
                        ventaProducto.idProducto = productos[i].id;
                        ventaProducto.cantidad = cantidad;
                        ventaProducto.precioUnitario = productos[i].precio;

                        if (strcmp(productos[i].unidad, "Gramos") == 0) {
                            ventaProducto.subtotal = (cantidad / productos[i].valorUnidad) * productos[i].precio;
                        } else {
                            ventaProducto.subtotal = productos[i].precio * cantidad;
                        }

                        ventasProductos[*numVentasProductos] = ventaProducto;
                        (*numVentasProductos)++;

                        printf("%s x%.2f %s agregado.\n", productos[i].nombre, cantidad, productos[i].unidad);
                        if (strcmp(productos[i].unidad, "Gramos") != 0) {
                            productos[i].stock -= (int)cantidad;
                        }
                        total += ventaProducto.subtotal;
                        productoEncontrado = 1;
                        break;
                    } else {
                        printf("Stock insuficiente.\n");
                        productoEncontrado = 1;
                    }
                    break;
                }
            }
            if (!productoEncontrado) {
                printf("Producto no encontrado.\n");
            }
        } else if (opcion == 2) {
            printf("\n--- Resumen Venta ---\n");
            for (i = 0; i < *numVentasProductos; i++) {
                if (ventasProductos[i].idVenta == ventaActual.idVenta) {
                    Producto *producto = buscarProductoPorId(productos, numProductos, ventasProductos[i].idProducto);
                    if (producto != NULL) {
                        printf(
                            "Producto: %s, Cantidad: %.2f %s, Precio Unitario: %.2f, Subtotal: %.2f\n",
                            producto->nombre,
                            ventasProductos[i].cantidad,
                            producto->unidad,
                            ventasProductos[i].precioUnitario,
                            ventasProductos[i].subtotal
                        );
                    }
                }
            }
            printf("Total: %.2f\n", total);

            printf("Metodo de pago: ");
            scanf("%s", metodoPago);
            printf("ID Usuario: ");
            scanf("%d", &idUsuario);

            ventaActual.precioTotal = total;
            strcpy(ventaActual.metodoPago, metodoPago);
            ventaActual.idUsuario = idUsuario;

            ventas[*numVentas] = ventaActual;
            (*numVentas)++;

            printf("Venta finalizada.\n");
            break;
        } else if (opcion == 3) {
            printf("Venta cancelada.\n");
            break;
        } else {
            printf("Opción inválida.\n");
        }
    } while (1);
}

static int obtenerProductos(Producto **productos) {
    FILE *archivo = fopen("common/data/productos.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    char linea[256];
    int count = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        count++;
    }

    fseek(archivo, 0, SEEK_SET);

    *productos = (Producto *)malloc(count * sizeof(Producto));
    if (*productos == NULL) {
        fclose(archivo);
        return 0;
    }

    fgets(linea, sizeof(linea), archivo);

    int i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Producto p;
        sscanf(linea, "%d,%49[^,],%f,%d,%d,%19[^,],%d,%19[^,],%d",
               &p.id, p.nombre, &p.precio, &p.stock, &p.activo, p.unidad,
               &p.valorUnidad, p.codigoBarras, &p.stockMinimo);
        (*productos)[i] = p;
        i++;
    }

    fclose(archivo);
    return count;
}

void registrarVentaMenu() {
    int i;
    Producto *productos = NULL;
    int numProductos = obtenerProductos(&productos);

    if (numProductos== 0) {
        printf("No se pudieron obtener los productos.\n");
        return;
    }

    printf("Número total de productos: %d\n", numProductos);

    for (i = 0; i < numProductos; i++) {
        printf("ID: %d, Nombre: %s, Precio: %.2f\n", productos[i].id, productos[i].nombre, productos[i].precio);
    }

    Venta ventas[MAX_VENTAS];
    int numVentas = 0;

    VentaProducto ventasProductos[MAX_VENTAS_PRODUCTOS];
    int numVentasProductos = 0;

    registrarVenta(productos, numProductos, ventas, &numVentas, ventasProductos, &numVentasProductos);

    free(productos);
}
