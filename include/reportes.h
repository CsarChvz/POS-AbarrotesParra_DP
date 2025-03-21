#ifndef REPORTES_H
#define REPORTES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int ID;
    int productoID;
    time_t timestamp;
    int cantidad;
} Inventario;

void mostrar_ventas_dia();

void mostar_ventas_semana(char *fecha_inicio);

void mostrar_ventas_anio(int mes, int anio);

#endif
