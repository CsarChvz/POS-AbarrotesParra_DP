#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

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

void registrarIngreso();
void registrarEgreso();

#endif