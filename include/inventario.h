#ifndef INVENTARIO_H
#define INVENTARIO_H

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

void registrarProductoMenu();

void visualizarProductosMenu();

void eliminarProductoMenu();
#endif