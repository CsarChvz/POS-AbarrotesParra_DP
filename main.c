#include <stdio.h>
#include <stdlib.h>
#include "common/logo.h"
#include "common/menu_main.h"
#include "include/seguridad.h"

int main() {
    int rol;
    while (1) {
        if (inicio_sesion() == 1) {
            while (1) {
                printf("\n--- Men\243 Principal ---\n"); // Men� (� = ASCII 243)
                printf("1. Acceder al sistema\n");
                printf("2. Cerrar sesi\242n\n"); // Sesi�n (� = ASCII 162)
                printf("3. Salir\n");
                printf("Seleccione una opci\242n: "); // Opci�n (� = ASCII 162)
                
                int opcion;
                scanf("%d", &opcion);
				
                switch (opcion) {
                    case 1:
                        rol = obtener_rol(usuario_global.usuario);
                        menu_main(&rol); // Funci�n de tu sistema
                        break;
                    case 2:
                        cerrar_sesion();
                        break;
                    case 3:
                        printf("Saliendo del programa...\n");
                        return 0;
                    default:
                        printf("Opci\242n inv\240lida. Intente nuevamente.\n"); // Opci�n (� = 162), Inv�lida (� = 160)
                }

                if (usuario_global.usuario[0] == '\0') {
                    break; // Regresa al login si se cierra sesi�n
                }
            }
        } else {
            printf("Saliendo del programa...\n");
            return 0;
        }
    }
}

