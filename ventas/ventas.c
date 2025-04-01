#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/seguridad.h"
#include "../include/descuentos.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

#define MAX_VENTAS 100
#define MAX_VENTAS_PRODUCTOS (MAX_VENTAS * 10)

#define NOMBRE_LENGTH 100
#define UNIDAD_LENGTH 20
#define CODIGO_BARRAS_LENGTH 20

#define ARCHIVO_TRANSACCIONES "common/data/transacciones.csv"

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

typedef struct {
    int idTransaccion;
    char fecha[11]; // YYYY-MM-DD
    char tipoMovimiento[10]; // Ingreso o Egreso
    char categoriaTransaccion[20]; // Venta, Compra, etc.
    float ingreso;
    float egreso;
    float saldo;
    char metodoPago[20];
    int idUsuario;
    char observaciones[100];
    char corteCaja[4]; // "Sí" o "No"
    char inicioJornada[4]; // "Sí" o "No"
} Transaccion;

Producto *buscarProductoPorId(Producto productos[], int numProductos, int idProducto) {
    int i;
    for (i = 0; i < numProductos; i++) {
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

void guardarVentaProceso(VentaProducto ventasProductos[], int numVentasProductos, int idVenta) {
    FILE *archivo = fopen("common/data/ventas_productos.csv", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo ventas_proceso.csv\n");
        return;
    }

    int i;
    for (i = 0; i < numVentasProductos; i++) {
        if (ventasProductos[i].idVenta == idVenta) {
            fprintf(archivo, "%d,%d,%d,%.2f,%.2f,%.2f\n",
                    ventasProductos[i].idVP, ventasProductos[i].idVenta, ventasProductos[i].idProducto,
                    ventasProductos[i].cantidad, ventasProductos[i].precioUnitario, ventasProductos[i].subtotal);
        }
    }

    fclose(archivo);
}

void guardarVenta(Venta venta) {
    FILE *archivo = fopen("common/data/ventas.csv", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo ventas.csv\n");
        return;
    }

    struct tm *timeinfo;
    timeinfo = localtime(&venta.fechaVenta);

    fprintf(archivo, "%d,%d-%02d-%02d,%s,%d,%.2f\n",
            venta.idVenta, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            venta.metodoPago, venta.idUsuario, venta.precioTotal);

    fclose(archivo);
}

static int obtenerSiguienteIdVenta() {
    FILE *archivo = fopen("common/data/ventas.csv", "r");
    if (archivo == NULL) {
        return 1; // Si no existe el archivo, el primer ID es 1
    }

    char linea[256];
    int maxId = 0;
    int id;

    // Saltar cabecera
    fgets(linea, 256, archivo);

    while (fgets(linea, 256, archivo) != NULL) {
        sscanf(linea, "%d", &id);
        if (id > maxId) {
            maxId = id;
        }
    }

    fclose(archivo);
    return maxId + 1;
}

static int obtenerSiguienteIdVentaProducto() {
    FILE *archivo = fopen("common/data/ventas_productos.csv", "r");
    if (archivo == NULL) {
        return 1; // Si no existe el archivo, el primer ID es 1
    }

    char linea[256];
    int maxId = 0;
    int id;

    // Saltar cabecera
    fgets(linea, 256, archivo);

    while (fgets(linea, 256, archivo) != NULL) {
        sscanf(linea, "%d", &id);
        if (id > maxId) {
            maxId = id;
        }
    }

    fclose(archivo);
    return maxId + 1;
}
float aplicarDescuento(int idProducto, float subtotal) {
    FILE *archivo = fopen(ARCHIVO_DESCUENTOS, "r");
    if (archivo == NULL) {
        return subtotal; // No se pudo abrir el archivo, no se aplica descuento
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Saltar la cabecera
    time_t tiempoActual = time(NULL);
    struct tm *tiempoLocal = localtime(&tiempoActual);
    char fechaActual[FECHA_LENGTH];
    strftime(fechaActual, sizeof(fechaActual), "%Y-%m-%d", tiempoLocal);

    float subtotalConDescuento = subtotal;
    int descuentoAplicado = 0; // Bandera para verificar si se aplicó un descuento

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Descuento descuento;
        if (sscanf(linea, "%d,%d,%d,%10[^,],%10[^\n]",
                    &descuento.idDescuento, &descuento.idProducto, &descuento.porcentajeDescuento,
                    descuento.fechaInicio, descuento.fechaFin) == 5) {
            if (descuento.idProducto == idProducto &&
                strcmp(descuento.fechaInicio, fechaActual) <= 0 &&
                strcmp(descuento.fechaFin, fechaActual) >= 0) {
                // Se aplica el descuento
                subtotalConDescuento = subtotal * (1 - (float)descuento.porcentajeDescuento / 100);
                descuentoAplicado = 1; // Se aplicó un descuento
                break; // Se encontró y aplicó el descuento, no es necesario buscar más
            }
        }
    }

    fclose(archivo);

    // Registrar auditoría si se aplicó un descuento
    if (descuentoAplicado) {
        registrarRegistroAuditoria(usuario_global.id, "APLICAR_DESCUENTO", "Aplicar descuento", "Descuento", idProducto, "Descuento aplicado", "Modificación", "Éxito");
    }

    return subtotalConDescuento;
}



static int actualizarStockProducto(int id, float cantidad, int restar) {
    FILE *archivoOriginal = fopen("common/data/productos.csv", "r");
    FILE *archivoTemporal = fopen("common/data/productos_temp.csv", "w");

    if (!archivoOriginal || !archivoTemporal) {
        perror("Error al abrir los archivos");
        if (archivoOriginal) fclose(archivoOriginal);
        if (archivoTemporal) fclose(archivoTemporal);
        return 0;
    }

    char linea[256];
    int encontrado = 0;

    // Copiar la cabecera
    if (fgets(linea, sizeof(linea), archivoOriginal)) {
        fputs(linea, archivoTemporal);
    }

    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        Producto p;
        if (sscanf(linea, "%d,%49[^,],%f,%d,%d,%19[^,],%d,%14[^,],%d",
                    &p.id, p.nombre, &p.precio, &p.stock, &p.activo,
                    p.unidad, &p.valorUnidad, p.codigoBarras, &p.stockMinimo) == 9) {
            if (p.id == id) {
                if (restar) {
                    p.stock -= (int)cantidad; // Restar la cantidad
                } else {
                    p.stock += (int)cantidad; // Incrementar la cantidad
                }
                encontrado = 1;
                printf("\n[Stock actualizado] Producto: '%s' | Nuevo Stock: %d\n", p.nombre, p.stock);
            }
            fprintf(archivoTemporal, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                    p.id, p.nombre, p.precio, p.stock, p.activo,
                    p.unidad, p.valorUnidad, p.codigoBarras, p.stockMinimo);
        } else {
            printf("Advertencia: Línea corrupta en el archivo.\n");
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/productos_temp.csv");
        printf("Error: No se encontró el producto con ID %d.\n", id);
        return 0;
    }

    if (remove("common/data/productos.csv") != 0) {
        perror("Error al eliminar el archivo original");
        return 0;
    }

    if (rename("common/data/productos_temp.csv", "common/data/productos.csv") != 0) {
        perror("Error al renombrar el archivo temporal");
        return 0;
    }

    return 1;
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

    ventaActual.idVenta = obtenerSiguienteIdVenta();
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
                        ventaProducto.idVP = obtenerSiguienteIdVentaProducto();
                        ventaProducto.idVenta = ventaActual.idVenta;
                        ventaProducto.idProducto = productos[i].id;
                        ventaProducto.cantidad = cantidad;
                        ventaProducto.precioUnitario = productos[i].precio;

                        if (strcmp(productos[i].unidad, "Gramos") == 0) {
                            ventaProducto.subtotal = (cantidad / productos[i].valorUnidad) * productos[i].precio;
                        } else {
                            ventaProducto.subtotal = productos[i].precio * cantidad;
                        }

                        // Aplicar descuento aquí
                        ventaProducto.subtotal = aplicarDescuento(productos[i].id, ventaProducto.subtotal);

                        ventasProductos[*numVentasProductos] = ventaProducto;
                        (*numVentasProductos)++;

                        actualizarStockProducto(productos[i].id, cantidad, 1);

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
                        break;
                    }
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

            int opcionPago;
            printf("Metodo de pago:\n");
            printf("1. Efectivo\n");
            printf("2. Tarjeta\n");
            printf("3. Transferencia\n");
            printf("Ingrese opción: ");
            scanf("%d", &opcionPago);

            switch (opcionPago) {
                case 1:
                    strcpy(metodoPago, "Efectivo");
                    float montoRecibido;
                    do {
                        printf("Ingrese el monto recibido: ");
                        scanf("%f", &montoRecibido);
                        if (montoRecibido < total) {
                            printf("El monto recibido es insuficiente. Ingrese un monto mayor o igual al total.\n");
                        }
                    } while (montoRecibido < total);

                    float cambio = montoRecibido - total;
                    printf("Cambio: %.2f\n", cambio);
                    break;
                case 2:
                    strcpy(metodoPago, "Tarjeta");
                    break;
                case 3:
                    strcpy(metodoPago, "Transferencia");
                    break;
                default:
                    printf("Opción de pago inválida. Se usará 'Efectivo' por defecto.\n");
                    strcpy(metodoPago, "Efectivo");
                    break;
            }

            ventaActual.precioTotal = total;
            strcpy(ventaActual.metodoPago, metodoPago);
            ventaActual.idUsuario = usuario_global.id;

            ventas[*numVentas] = ventaActual;
            (*numVentas)++;

            guardarVentaProceso(ventasProductos, *numVentasProductos, ventaActual.idVenta);
            guardarVenta(ventaActual);

            Transaccion transaccionVenta;
            transaccionVenta.idTransaccion = obtenerSiguienteIdTransaccion();
            transaccionVenta.idUsuario = usuario_global.id;

            time_t tiempoActual = time(NULL);
            struct tm *tiempoLocal = localtime(&tiempoActual);
            strftime(transaccionVenta.fecha, sizeof(transaccionVenta.fecha), "%Y-%m-%d", tiempoLocal);

            strcpy(transaccionVenta.tipoMovimiento, "Ingreso");
            strcpy(transaccionVenta.categoriaTransaccion, "Venta");
            transaccionVenta.ingreso = total;
            transaccionVenta.egreso = 0.0;
            strcpy(transaccionVenta.metodoPago, metodoPago);
            sprintf(transaccionVenta.observaciones, "Venta ID: %d", ventaActual.idVenta);
            strcpy(transaccionVenta.corteCaja, "No");
            strcpy(transaccionVenta.inicioJornada, "No");

            FILE *archivoTransacciones = fopen(ARCHIVO_TRANSACCIONES, "r");
            float saldoAnterior = 0.0;
            if (archivoTransacciones != NULL) {
                char linea[256];
                fgets(linea, sizeof(linea), archivoTransacciones);
                while (fgets(linea, sizeof(linea), archivoTransacciones)) {
                    Transaccion t;
                    if (sscanf(linea, "%d,%10[^,],%9[^,],%19[^,],%f,%f,%f,%19[^,],%d,%99[^,],%3[^,],%3[^\n]",
                                &t.idTransaccion, t.fecha, t.tipoMovimiento, t.categoriaTransaccion,
                                &t.ingreso, &t.egreso, &t.saldo, t.metodoPago, &t.idUsuario,
                                t.observaciones, t.corteCaja, t.inicioJornada) == 12) {
                        saldoAnterior = t.saldo;
                    }
                }
                fclose(archivoTransacciones);
            }

            transaccionVenta.saldo = saldoAnterior + transaccionVenta.ingreso;

            archivoTransacciones = fopen(ARCHIVO_TRANSACCIONES, "a");
            if (archivoTransacciones == NULL) {
                printf("Error al abrir el archivo de transacciones.\n");
                return;
            }

            fprintf(archivoTransacciones, "%d,%s,%s,%s,%.2f,%.2f,%.2f,%s,%d,%s,%s,%s\n",
                    transaccionVenta.idTransaccion, transaccionVenta.fecha, transaccionVenta.tipoMovimiento,
                    transaccionVenta.categoriaTransaccion, transaccionVenta.ingreso, transaccionVenta.egreso,
                    transaccionVenta.saldo, transaccionVenta.metodoPago, transaccionVenta.idUsuario,
                    transaccionVenta.observaciones, transaccionVenta.corteCaja, transaccionVenta.inicioJornada);

            fclose(archivoTransacciones);

            registrarRegistroAuditoria(usuario_global.id, "REGISTRAR_VENTA", "Registrar venta", "Venta", ventaActual.idVenta, "Venta registrada", "Venta", "Éxito");

            printf("Venta finalizada y datos guardados en ventas.csv y transacciones.csv.\n");
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

void generarTicketVenta(int idVenta, Venta ventas[], int numVentas, VentaProducto ventasProductos[], int numVentasProductos, Producto productos[], int numProductos) {
    int i, j;
    Venta venta;
    struct tm *timeinfo;

    // Buscar la venta por ID
    for (i = 0; i < numVentas; i++) {
        if (ventas[i].idVenta == idVenta) {
            venta = ventas[i];
            break;
        }
    }

    if (i == numVentas) {
        printf("Venta con ID %d no encontrada.\n", idVenta);
        return;
    }

    timeinfo = localtime(&venta.fechaVenta);

    printf("\n--- Ticket de Venta ---\n");
    printf("ID Venta: %d\n", venta.idVenta);
    printf("Fecha: %04d-%02d-%02d %02d:%02d:%02d\n",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printf("Método de Pago: %s\n", venta.metodoPago);
    printf("Usuario: %d\n", venta.idUsuario);
    printf("\n--- Productos ---\n");

    float total = 0;

    // Buscar y mostrar los productos de la venta
    for (i = 0; i < numVentasProductos; i++) {
        if (ventasProductos[i].idVenta == idVenta) {
            Producto *producto = buscarProductoPorId(productos, numProductos, ventasProductos[i].idProducto);
            if (producto != NULL) {
                printf("%s x%.2f %s: %.2f\n",
                        producto->nombre, ventasProductos[i].cantidad, producto->unidad, ventasProductos[i].subtotal);
                total += ventasProductos[i].subtotal;
            }
        }
    }

    printf("\nTotal: %.2f\n", total);
    printf("--- Fin del Ticket ---\n\n");
}

void registrarVentaMenu() {
    int i;
    Producto *productos = NULL;
    int numProductos = obtenerProductos(&productos);

    if (numProductos == 0) {
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

    if (numVentas > 0) {
        generarTicketVenta(ventas[numVentas - 1].idVenta, ventas, numVentas, ventasProductos, numVentasProductos, productos, numProductos);
    }

    free(productos);
}
void visualizarHistorialVentas() {
    FILE *archivo = fopen("common/data/ventas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo ventas.csv\n");
        return;
    }

    char linea[256];
    int idVenta, idUsuario, pagina = 1, ventasPorPagina = 10, totalVentas = 0, i, inicio, fin;
    time_t fechaVenta;
    char metodoPago[20];
    float precioTotal;
    struct tm *timeinfo;

    // Contar el número total de ventas
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        totalVentas++;
    }
    rewind(archivo); // Volver al inicio del archivo

    // Saltar la cabecera
    fgets(linea, sizeof(linea), archivo);

    while (1) {
        printf("\n--- Historial de Ventas (Página %d) ---\n", pagina);
        printf("ID Venta | Fecha     | Metodo de Pago | Precio Total\n");
        printf("------------------------------------------------------\n");

        inicio = (pagina - 1) * ventasPorPagina;
        fin = inicio + ventasPorPagina;
        if (fin > totalVentas - 1) { // Restar 1 por la cabecera
            fin = totalVentas - 1;
        }

        i = 0;
        while (fgets(linea, sizeof(linea), archivo) != NULL) {
            if (i >= inicio && i < fin) {
                sscanf(linea, "%d,%ld,%s,%d,%f", &idVenta, &fechaVenta, metodoPago, &idUsuario, &precioTotal);
                timeinfo = localtime(&fechaVenta);
                printf("%-8d | %04d-%02d-%02d | %-14s | %.2f\n",
                        idVenta, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                        metodoPago, precioTotal);

                // Registrar auditoría para cada venta visualizada
                registrarRegistroAuditoria(usuario_global.id, "VISUALIZAR_VENTA", "Visualizar venta", "Venta", idVenta, "Venta visualizada en historial", "Visualización", "Éxito");
            }
            i++;
        }

        char opcion;
        printf("\n--- Opciones ---\n");
        if (pagina > 1) {
            printf("a. Página Anterior\n");
        }
        if (fin < totalVentas - 1) { // Restar 1 por la cabecera
            printf("s. Siguiente Página\n");
        }
        printf("q. Salir\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);

        if (opcion == 'a' && pagina > 1) {
            pagina--;
            rewind(archivo); // Volver al inicio del archivo
            fgets(linea, sizeof(linea), archivo); // Saltar la cabecera
        } else if (opcion == 's' && fin < totalVentas - 1) {
            pagina++;
            rewind(archivo); // Volver al inicio del archivo
            fgets(linea, sizeof(linea), archivo); // Saltar la cabecera
        } else if (opcion == 'q') {
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    fclose(archivo);
}