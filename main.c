#include <stdio.h>
#include <stdlib.h>
#include "common/logo.h"
#include "common/menu_main.h"
#include "include/seguridad.h"

int main() {
    while (1) {
        if (inicio_sesion() == 1) {
            while (1) {
                printf("\n--- Men\243 Principal ---\n"); // Menú (é = ASCII 243)
                printf("1. Acceder al sistema\n");
                printf("2. Cerrar sesi\242n\n"); // Sesión (ó = ASCII 162)
                printf("3. Salir\n");
                printf("Seleccione una opci\242n: "); // Opción (ó = ASCII 162)
                
                int opcion;
                scanf("%d", &opcion);
				
                switch (opcion) {
                    case 1:
                    	
                        menu_main(obtener_rol(usuario_actual)); // Función de tu sistema
                        break;
                    case 2:
                        cerrar_sesion();
                        break;
                    case 3:
                        printf("Saliendo del programa...\n");
                        return 0;
                    default:
                        printf("Opci\242n inv\240lida. Intente nuevamente.\n"); // Opción (ó = 162), Inválida (á = 160)
                }

                if (usuario_actual[0] == '\0') {
                    break; // Regresa al login si se cierra sesión
                }
            }
        } else {
            printf("Saliendo del programa...\n");
            return 0;
        }
    }
}

