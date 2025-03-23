#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMBRE_LENGTH 100
#define UNIDAD_LENGTH 20
#define CODIGO_BARRAS_LENGTH 20

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
        printf("\n--- Lista de Productos (Stock) - Página %d ---\n", pagina);

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
            printf("a. Página Anterior\n");
        }
        if (fin < cantidadProductosLeidos) {
            printf("s. Siguiente Página\n");
        }
        printf("q. Salir\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion); // Espacio antes de %c para consumir el salto de línea

        if (opcion == 'a' && pagina > 1) {
            pagina--; // Ir a la página anterior
        } else if (opcion == 's' && fin < cantidadProductosLeidos) {
            pagina++; // Ir a la siguiente página
        } else if (opcion == 'q') {
            break; // Salir
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Liberar memoria
    free(productos);

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
    printf("ID\tNombre\t\tStock\tStock Mínimo\n");
    printf("-------------------------------------\n");
    for (i = 0; i < cantidad; i++) {
        printf("%d\t%-20s\t%d\t%d\n", productos[i].id, productos[i].nombre, productos[i].stock, productos[i].stockMinimo);
    }
    
    free(productos);
}
