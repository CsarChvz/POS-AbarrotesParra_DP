#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/seguridad.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

#define NOMBRE_LENGTH 100
#define UNIDAD_LENGTH 20
#define CODIGO_BARRAS_LENGTH 20

#define ARCHIVO_PRODUCTOS "common/data/productos.csv"

typedef struct {
    int id;
    char nombre[NOMBRE_LENGTH];
    float precio;
    int stock;
    int activo;  // 1 para activo, 0 para no activo
    char unidad[UNIDAD_LENGTH];
    int valorUnidad;  // Valor en gramos, litros, unidades, etc.
    char codigoBarras[CODIGO_BARRAS_LENGTH];
    int stockMinimo;
} Producto;

// Función para obtener productos (solo ID, nombre y stock)
int obtenerProductosStock(Producto **productos) {
    FILE *archivo = fopen("common/data/productos.csv", "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }

    char linea[256];
    int count = 0;

    // Contamos la cantidad de productos en el archivo
    while (fgets(linea, 256, archivo) != NULL) {
        count++;
    }

    // Volvemos al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Reservamos memoria para los productos
    *productos = (Producto *)malloc(count * sizeof(Producto));
    if (*productos == NULL) {
        fclose(archivo);
        return 0; // Error al asignar memoria
    }

    // Saltar la línea de cabecera si existe
    fgets(linea, 256, archivo);

    // Leer los productos desde el archivo CSV
    int i = 0;
    while (fgets(linea, 256, archivo) != NULL) {
        Producto p;
        sscanf(linea, "%d,%[^,],%*f,%d,%*d,%*[^,],%*d,%*[^,],%*d",
                &p.id, p.nombre, &p.stock);
        (*productos)[i] = p;
        i++;
    }

    fclose(archivo);
    return count; // Retorna la cantidad de productos leídos
}

// Función para listar productos (solo ID, nombre y stock) con paginación
int listarStockProductos() {
    Producto *productos = NULL; // Puntero para almacenar productos
    int cantidadProductosLeidos, i, pagina = 1, productosPorPagina = 10;

    cantidadProductosLeidos = obtenerProductosStock(&productos);

    if (cantidadProductosLeidos == 0) {
        printf("\nNo hay productos registrados o error al leer el archivo.\n");
        return 1;
    }

    while (1) {
        printf("\n--- Lista de Productos (Stock) - P%cgina %d ---\n", 160, pagina);

        // Mostrar los productos de la página actual
        int inicio = (pagina - 1) * productosPorPagina;
        int fin = inicio + productosPorPagina;
        if (fin > cantidadProductosLeidos) {
            fin = cantidadProductosLeidos;
        }

        printf("ID\tNombre\t\t\tStock\n");
        printf("--------------------------------------\n");

        for (i = inicio; i < fin; i++) {
            printf("%d\t%-20s\t%d\n", productos[i].id, productos[i].nombre, productos[i].stock);
        }

        // Preguntar si desea ver la siguiente página o volver a la anterior
        char opcion;
        printf("\n--- Opciones ---\n");
        if (pagina > 1) {
            printf("a. P%cgina Anterior\n", 160);
        }
        if (fin < cantidadProductosLeidos) {
            printf("s. Siguiente P%cgina\n", 160);
        }
        printf("q. Salir\n");
        printf("Seleccione una opci%cn: ", 162);
        scanf(" %c", &opcion); // Espacio antes de %c para consumir el salto de línea

        if (opcion == 'a' && pagina > 1) {
            pagina--; // Ir a la página anterior
        } else if (opcion == 's' && fin < cantidadProductosLeidos) {
            pagina++; // Ir a la siguiente página
        } else if (opcion == 'q') {
            break; // Salir
        } else {
            printf("Opci%cn inv%clida.\n", 162, 160);
        }
    }

    // Liberar memoria
    free(productos);
    registrarRegistroAuditoria(usuario_global.id,"LISTADO_STOCK_PRODUCTOS","Listado de productos con stock","Producto",0,"Listado de productos con stock visto","Informativo","Exito");

    return 0;
}

// Función para obtener productos con bajo stock
int obtenerProductosBajoStock(Producto **productos) {
    FILE *archivo = fopen("common/data/productos.csv", "r");
    if (archivo == NULL) {
        return 0; // Error o archivo no existe
    }

    char linea[256];
    int count = 0;

    // Contamos la cantidad de productos en el archivo
    while (fgets(linea, 256, archivo) != NULL) {
        count++;
    }

    // Volvemos al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Reservamos memoria para los productos
    *productos = (Producto *)malloc(count * sizeof(Producto));
    if (*productos == NULL) {
        fclose(archivo);
        return 0;
    }

    fgets(linea, 256, archivo); // Saltar la cabecera si existe

    int i = 0, productosBajoStock = 0;
    while (fgets(linea, 256, archivo) != NULL) {
        Producto p;
        sscanf(linea, "%d,%[^,],%*f,%d,%d,%*[^,],%*d,%*[^,],%*d",
                &p.id, p.nombre, &p.stock, &p.stockMinimo);

        if (p.stock <= p.stockMinimo + 5) { // Filtra productos con bajo stock
            (*productos)[productosBajoStock++] = p;
        }
    }

    fclose(archivo);
    return productosBajoStock;
}

// Función para listar productos con bajo stock
void visualizarStockBajo() {
    Producto *productos = NULL;
    int cantidad = obtenerProductosBajoStock(&productos);
    int i;
    if (cantidad == 0) {
        printf("\nNo hay productos con bajo stock.\n");
        return;
    }

    printf("\n--- Productos con Bajo Stock ---\n");
    printf("ID\tNombre\t\tStock\tStock M%cnimo\n", 161);
    printf("-------------------------------------\n");
    for (i = 0; i < cantidad; i++) {
        printf("%d\t%-20s\t%d\t%d\n", productos[i].id, productos[i].nombre, productos[i].stock, productos[i].stockMinimo);
    }

    free(productos);
    registrarRegistroAuditoria(usuario_global.id,"LISTADO_STOCK_BAJO","Listado de productos con bajo stock","Producto",0,"Listado de productos con bajo stock visto","Informativo","Exito");
}

int buscarProducto(int id, const char *nombre, Producto *resultado) {
    FILE *archivo = fopen(ARCHIVO_PRODUCTOS, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de productos");
        return 0;
    }

    char linea[256];
    int encontrado = 0;

    // Saltar la cabecera
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
        return 0;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        Producto p;
        if (sscanf(linea, "%d,%49[^,],%f,%d,%d,%19[^,],%d,%14[^,],%d",
                    &p.id, p.nombre, &p.precio, &p.stock, &p.activo,
                    p.unidad, &p.valorUnidad, p.codigoBarras, &p.stockMinimo) == 9) {
            if ((id != -1 && p.id == id) || (nombre && strcmp(p.nombre, nombre) == 0)) {
                *resultado = p;
                encontrado = 1;
                break;
            }
        }
    }

    fclose(archivo);
    return encontrado;
}

int actualizarStockProducto(int id, int cantidad) {
    FILE *archivoOriginal = fopen(ARCHIVO_PRODUCTOS, "r");
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
                p.stock += cantidad; // Incrementar el stock
                encontrado = 1;
                printf("\n[Stock actualizado] Producto: '%s' | Nuevo Stock: %d\n", p.nombre, p.stock);
            }
            fprintf(archivoTemporal, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                    p.id, p.nombre, p.precio, p.stock, p.activo,
                    p.unidad, p.valorUnidad, p.codigoBarras, p.stockMinimo);
        } else {
            printf("Advertencia: L%cnea corrupta en el archivo.\n", 161);
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/productos_temp.csv");
        printf("Error: No se encontr%c el producto con ID %d.\n", 162, id);
        return 0;
    }

    if (remove(ARCHIVO_PRODUCTOS) != 0) {
        perror("Error al eliminar el archivo original");
        return 0;
    }

    if (rename("common/data/productos_temp.csv", ARCHIVO_PRODUCTOS) != 0) {
        perror("Error al renombrar el archivo temporal");
        return 0;
    }

    return 1;
}

void registrarReposicionStock() {
    int id;
    char nombre[NOMBRE_LENGTH];
    Producto producto;

    printf("\n=== Registrar Reposici%cn de Stock ===\n", 162);
    printf("Desea buscar el producto por:\n");
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("Seleccione una opci%cn: ", 162);

    int opcion;
    if (scanf("%d", &opcion) != 1) {
        printf("Entrada inv%clida.\n", 160);
        return;
    }

    if (opcion == 1) {
        printf("Ingrese el ID del producto: ");
        if (scanf("%d", &id) != 1) {
            printf("Entrada inv%clida.\n", 160);
            return;
        }
        if (!buscarProducto(id, NULL, &producto)) {
            printf("Producto con ID %d no encontrado.\n", id);
            return;
        }
    } else if (opcion == 2) {
        printf("Ingrese el nombre del producto: ");
        scanf(" %49[^\n]", nombre); // Leer hasta el salto de línea
        if (!buscarProducto(-1, nombre, &producto)) {
            printf("Producto con nombre '%s' no encontrado.\n", nombre);
            return;
        }
        id = producto.id; // Obtener el ID del producto encontrado
    } else {
        printf("Opci%cn inv%clida.\n", 162, 160);
        return;
    }

    printf("\nProducto encontrado:\n");
    printf("ID: %d | Nombre: %s | Stock actual: %d | Unidad: %s\n",
            producto.id, producto.nombre, producto.stock, producto.unidad);

    int cantidad;
    printf("\nIngrese la cantidad a agregar al stock: ");
    if (scanf("%d", &cantidad) != 1 || cantidad <= 0) {
        printf("Cantidad inv%clida.\n", 160);
        return;
    }

    if (actualizarStockProducto(id, cantidad)) {
        printf("\nReposici%cn de stock realizada con %cxito.\n", 162, 130);
        registrarRegistroAuditoria(usuario_global.id,"REPOSICION_STOCK","Reposición de stock","Producto",id,"Reposición de stock realizada","Informativo","Exito");
    } else {
        printf("\nError al actualizar el stock.\n");
    }
}

int actualizarStockMinimo(int id, int nuevoStockMinimo) {
    FILE *archivoOriginal = fopen(ARCHIVO_PRODUCTOS, "r");
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
                p.stockMinimo = nuevoStockMinimo; // Actualizar stock mínimo
                encontrado = 1;
                printf("\n[Stock m%cnimo actualizado] Producto: '%s' | Nuevo Stock M%cnimo: %d\n",
                        161, p.nombre, 161, p.stockMinimo);
            }
            fprintf(archivoTemporal, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                    p.id, p.nombre, p.precio, p.stock, p.activo,
                    p.unidad, p.valorUnidad, p.codigoBarras, p.stockMinimo);
        } else {
            printf("Advertencia: L%cnea corrupta en el archivo.\n", 161);
        }
    }

    fclose(archivoOriginal);
    fclose(archivoTemporal);

    if (!encontrado) {
        remove("common/data/productos_temp.csv");
        printf("Error: No se encontr%c el producto con ID %d.\n", 162, id);
        return 0;
    }

    if (remove(ARCHIVO_PRODUCTOS) != 0) {
        perror("Error al eliminar el archivo original");
        return 0;
    }

    if (rename("common/data/productos_temp.csv", ARCHIVO_PRODUCTOS) != 0) {
        perror("Error al renombrar el archivo temporal");
        return 0;
    }

    return 1;
}

void modificarStockMinimo() {
    int id;
    char nombre[NOMBRE_LENGTH];
    Producto producto;

    printf("\n=== Modificar Stock M%cnimo ===\n", 161);
    printf("Desea buscar el producto por:\n");
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("Seleccione una opci%cn: ", 162);

    int opcion;
    if (scanf("%d", &opcion) != 1) {
        printf("Entrada inv%clida.\n", 160);
        return;
    }

    if (opcion == 1) {
        printf("Ingrese el ID del producto: ");
        if (scanf("%d", &id) != 1) {
            printf("Entrada inv%clida.\n", 160);
            return;
        }
        if (!buscarProducto(id, NULL, &producto)) {
            printf("Producto con ID %d no encontrado.\n", id);
            return;
        }
    } else if (opcion == 2) {
        printf("Ingrese el nombre del producto: ");
        scanf(" %49[^\n]", nombre); // Leer hasta el salto de línea
        if (!buscarProducto(-1, nombre, &producto)) {
            printf("Producto con nombre '%s' no encontrado.\n", nombre);
            return;
        }
        id = producto.id; // Obtener el ID del producto encontrado
    } else {
        printf("Opci%cn inv%clida.\n", 162, 160);
        return;
    }

    printf("\nProducto encontrado:\n");
    printf("ID: %d | Nombre: %s | Stock m%cnimo actual: %d | Unidad: %s\n",
            producto.id, producto.nombre, 161, producto.stockMinimo, producto.unidad);

    int nuevoStockMinimo;
    printf("\nIngrese el nuevo stock m%cnimo: ",161);
    if (scanf("%d", &nuevoStockMinimo) != 1 || nuevoStockMinimo < 0) {
        printf("Cantidad inv%clida.\n", 160);
        return;
    }

    if (actualizarStockMinimo(id, nuevoStockMinimo)) {
        printf("\nStock m%cnimo actualizado con %cxito.\n", 161, 130);
        registrarRegistroAuditoria(usuario_global.id,"MODIFICACION_STOCK_MINIMO","Modificación de stock mínimo","Producto",id,"Stock mínimo actualizado","Informativo","Exito");
    } else {
        printf("\nError al actualizar el stock m%cnimo.\n", 161);
    }
}