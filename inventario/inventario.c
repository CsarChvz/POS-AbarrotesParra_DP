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
    } else {
        printf("Error al registrar el producto.\n");
    }
}
