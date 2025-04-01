#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/seguridad.h"
#include "../include/auditoria.h" // Incluir el archivo de auditoría

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

static int obtenerSiguienteId() {
    FILE *archivo = fopen("common/data/productos.csv", "r");
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

int registrarProducto(Producto *producto) {
    // Validación de los datos del producto
    if (producto == NULL || strlen(producto->nombre) == 0 || producto->precio <= 0 || producto->stock < 0 || producto->valorUnidad <= 0) {
        printf("Error: Datos del producto inválidos.\n");
        return 0; // Error en los datos del producto
    }

    // Abre el archivo en modo 'append' (agregar al final)
    FILE *archivo = fopen("common/data/productos.csv", "a");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");  // Mejor manejo de errores
        return 0; // Error al abrir el archivo
    }

    // Obtener el siguiente ID disponible para el producto
    int id = obtenerSiguienteId();

    // Escribir los datos del producto en el archivo CSV
    if (fprintf(archivo, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                id, producto->nombre, producto->precio, producto->stock, producto->activo,
                producto->unidad, producto->valorUnidad, producto->codigoBarras, producto->stockMinimo) < 0) {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return 0; // Error al escribir en el archivo
    }

    fclose(archivo);
    return 1; // Éxito
}

void registrarProductoMenu() {
    Producto nuevoProducto;

    // Recoger los datos del producto
    printf("----------------------------------------\n");
    printf("--- Registrar Nuevo Producto ---\n");

    // Ingresar el nombre del producto
    printf("Ingrese el nombre del producto: ");
    scanf("%s", nuevoProducto.nombre);  // Recibe un nombre simple sin validaciones complejas

    // Ingresar el precio del producto
    printf("Ingrese el precio del producto: ");
    scanf("%f", &nuevoProducto.precio);  // Recibe el precio

    // Ingresar la cantidad en stock
    printf("Ingrese la cantidad en stock del producto: ");
    scanf("%d", &nuevoProducto.stock);  // Recibe el stock

    // Ingresar si está activo o no
    printf("¿Está el producto activo? (1 para sí, 0 para no): ");
    scanf("%d", &nuevoProducto.activo);  // Recibe si está activo

    // Ingresar la unidad de medida
    printf("Ingrese la unidad de medida (Ejemplo: Botella, Paquete, Gramos, Litros): ");
    scanf("%s", nuevoProducto.unidad);  // Recibe la unidad

    // Ingresar el valor por unidad
    printf("Ingrese el valor por unidad (por ejemplo, gramos, ml, unidades): ");
    scanf("%d", &nuevoProducto.valorUnidad);  // Recibe el valor de la unidad

    // Ingresar el código de barras
    printf("Ingrese el código de barras del producto: ");
    scanf("%s", nuevoProducto.codigoBarras);  // Recibe el código de barras

    // Ingresar el stock mínimo
    printf("Ingrese el stock mínimo del producto: ");
    scanf("%d", &nuevoProducto.stockMinimo);  // Recibe el stock mínimo

    // Llamar a la función para registrar el producto
    if (registrarProducto(&nuevoProducto)) {
        printf("Producto registrado exitosamente.\n");
        registrarRegistroAuditoria(usuario_global.id, "REGISTRAR_PRODUCTO", "Registrar producto", "Producto", nuevoProducto.id, "Producto registrado", "Informativo", "Éxito");
    } else {
        printf("Error al registrar el producto.\n");
    }
}

// Función para obtener los productos desde el archivo CSV
int obtenerProductos(Producto **productos) {
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
        sscanf(linea, "%d,%[^,],%f,%d,%d,%[^,],%d,%[^,],%d",
               &p.id, p.nombre, &p.precio, &p.stock, &p.activo, p.unidad,
               &p.valorUnidad, p.codigoBarras, &p.stockMinimo);
        (*productos)[i] = p;
        i++;
    }

    fclose(archivo);
    return count; // Retorna la cantidad de productos leídos
}

// Función para listar productos con paginación
int listarProductos() {
    Producto *productos = NULL; // Puntero para almacenar productos
    int cantidadProductosLeidos, i, pagina = 1, productosPorPagina = 10;

    cantidadProductosLeidos = obtenerProductos(&productos);

    if (cantidadProductosLeidos == 0) {
        printf("\nNo hay productos registrados o error al leer el archivo.\n");
        return 1;
    }

    while (1) {
        printf("\n--- Lista de Productos (Página %d) ---\n", pagina);

        // Mostrar los productos de la página actual
        int inicio = (pagina - 1) * productosPorPagina;
        int fin = inicio + productosPorPagina;
        if (fin > cantidadProductosLeidos) {
            fin = cantidadProductosLeidos;
        }

        for (i = inicio; i < fin; i++) {
            printf("ID: %d, Nombre: %-20s, Precio: %.2f, Stock: %d, Activo: %d, Unidad: %-10s, ValorUnidad: %d, Codigo: %s, Stock Mínimo: %d\n",
                   productos[i].id, productos[i].nombre, productos[i].precio,
                   productos[i].stock, productos[i].activo, productos[i].unidad,
                   productos[i].valorUnidad, productos[i].codigoBarras, productos[i].stockMinimo);
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
    registrarRegistroAuditoria(usuario_global.id, "LISTAR_PRODUCTOS", "Listar productos", "Producto", 0, "Listado de productos visto", "Informativo", "Éxito");

    return 0;
}

// Función de menú para visualizar productos
void visualizarProductosMenu() {
    int opcion;
    do {
        printf("\n--- Menú de Productos ---\n");
        printf("1. Ver Lista de Productos\n");
        printf("2. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                listarProductos();
                break;
            case 2:
                printf("Volviendo al menú principal.\n");
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 2);
}

void eliminarProductoPorNombre(char *nombre) {
    Producto *productos = NULL;
    int cantidadProductos, i;

    cantidadProductos = obtenerProductos(&productos);
    if (cantidadProductos == 0) {
        printf("No hay productos registrados.\n");
        return;
    }

    for (i = 0; i < cantidadProductos; i++) {
        if (strcmp(productos[i].nombre, nombre) == 0) {
            // Mostrar producto encontrado
            printf("Producto encontrado: ID: %d, Nombre: %s, Precio: %.2f, Stock: %d, Activo: %d, Unidad: %s, ValorUnidad: %d, Codigo: %s, Stock Mínimo: %d\n",
                   productos[i].id, productos[i].nombre, productos[i].precio, productos[i].stock,
                   productos[i].activo, productos[i].unidad, productos[i].valorUnidad, productos[i].codigoBarras, productos[i].stockMinimo);

            // Confirmación para eliminar
            char opcion;
            printf("¿Está seguro de que desea eliminar este producto? (s/n): ");
            scanf(" %c", &opcion);

            if (opcion == 's' || opcion == 'S') {
                // Eliminar producto
                // Para eliminar, se puede escribir los productos restantes en un nuevo archivo
                FILE *archivo = fopen("common/data/productos.csv", "w");
                if (archivo == NULL) {
                    perror("Error al abrir el archivo para reescribir");
                    free(productos);
                    return;
                }

                // Reescribir productos que no son el que estamos eliminando
                for (i = 0; i < cantidadProductos; i++) {
                    if (strcmp(productos[i].nombre, nombre) != 0) {
                        fprintf(archivo, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                                productos[i].id, productos[i].nombre, productos[i].precio, productos[i].stock,
                                productos[i].activo, productos[i].unidad, productos[i].valorUnidad, productos[i].codigoBarras, productos[i].stockMinimo);
                    }
                }

                fclose(archivo);
                printf("Producto eliminado exitosamente.\n");
                registrarRegistroAuditoria(usuario_global.id, "ELIMINAR_PRODUCTO", "Eliminar producto", "Producto", productos[i].id, "Producto eliminado por nombre", "Informativo", "Éxito");
            } else {
                printf("El producto no fue eliminado.\n");
            }
            free(productos);
            return;
        }
    }

    printf("No se encontró un producto con el nombre: %s\n", nombre);
    free(productos);
}

void eliminarProductoPorId(int id) {
    Producto *productos = NULL;
    int cantidadProductos, i;

    cantidadProductos = obtenerProductos(&productos);
    if (cantidadProductos == 0) {
        printf("No hay productos registrados.\n");
        return;
    }

    for (i = 0; i < cantidadProductos; i++) {
        if (productos[i].id == id) {
            // Mostrar producto encontrado
            printf("Producto encontrado: ID: %d, Nombre: %s, Precio: %.2f, Stock: %d, Activo: %d, Unidad: %s, ValorUnidad: %d, Codigo: %s, Stock Mínimo: %d\n",
                   productos[i].id, productos[i].nombre, productos[i].precio, productos[i].stock,
                   productos[i].activo, productos[i].unidad, productos[i].valorUnidad, productos[i].codigoBarras, productos[i].stockMinimo);

            // Confirmación para eliminar
            char opcion;
            printf("¿Está seguro de que desea eliminar este producto? (s/n): ");
            scanf(" %c", &opcion);

            if (opcion == 's' || opcion == 'S') {
                // Eliminar producto
                // Reescribir los productos restantes en el archivo
                FILE *archivo = fopen("common/data/productos.csv", "w");
                if (archivo == NULL) {
                    perror("Error al abrir el archivo para reescribir");
                    free(productos);
                    return;
                }

                // Reescribir productos que no son el que estamos eliminando
                for (i = 0; i < cantidadProductos; i++) {
                    if (productos[i].id != id) {
                        fprintf(archivo, "%d,%s,%.2f,%d,%d,%s,%d,%s,%d\n",
                                productos[i].id, productos[i].nombre, productos[i].precio, productos[i].stock,
                                productos[i].activo, productos[i].unidad, productos[i].valorUnidad, productos[i].codigoBarras, productos[i].stockMinimo);
                    }
                }

                fclose(archivo);
                printf("Producto eliminado exitosamente.\n");
                registrarRegistroAuditoria(usuario_global.id, "ELIMINAR_PRODUCTO", "Eliminar producto", "Producto", id, "Producto eliminado por ID", "Informativo", "Éxito");
            } else {
                printf("El producto no fue eliminado.\n");
            }
            free(productos);
            return;
        }
    }

    printf("No se encontró un producto con ID: %d\n", id);
    free(productos);
}

void eliminarProductoMenu() {
    int opcion, id;
    char nombre[NOMBRE_LENGTH];

    do {
        printf("\n--- Menú de Eliminación de Productos ---\n");
        printf("1. Eliminar Producto por ID\n");
        printf("2. Eliminar Producto por Nombre\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese el ID del producto a eliminar: ");
                scanf("%d", &id);
                eliminarProductoPorId(id);
                break;
            case 2:
                printf("Ingrese el nombre del producto a eliminar: ");
                scanf("%s", nombre);
                eliminarProductoPorNombre(nombre);
                break;
            case 3:
                printf("Volviendo al menú principal.\n");
                return;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 3);
}