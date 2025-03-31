#ifndef DESCUENTOS_H
#define DESCUENTOS_H

#include <stdio.h>
#include <time.h>

#define ARCHIVO_DESCUENTOS "common/data/descuentos.csv"
#define FECHA_LENGTH 11

// Estructura para representar un descuento
typedef struct {
    int idDescuento;
    int idProducto;
    int porcentajeDescuento;
    char fechaInicio[FECHA_LENGTH];
    char fechaFin[FECHA_LENGTH];
} Descuento;

// Función para aplicar un descuento a un subtotal
float aplicarDescuento(int idProducto, float subtotal);

#endif