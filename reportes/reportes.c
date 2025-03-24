#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura Inventario
typedef struct {
    int ID;
    int productoID;
    time_t timestamp;
    int cantidad;
} Inventario;


void mostrar_ventas_dia() {
	printf("Hoy se han tenido 8 ventas");
}

void mostar_ventas_semana(char *fecha_inicio){
	printf("Se han tenido n ventas desde %s hasta %s", fecha_inicio, fecha_inicio);
}

void mostrar_ventas_anio(int mes, int anio){
	printf("Se muestran las ventas que se han tenido en el mes: %d en el año: %d", mes, anio);
}
