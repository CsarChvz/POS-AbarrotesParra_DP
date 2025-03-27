Función resultado <- TienePermiso (role,permiso_minimo)
	resultado <- role>=permiso_minimo
FinFunción

Algoritmo SistemaPuntoVenta
    Definir ROL_VENDEDOR, ROL_ADMIN, rolUsuario Como Entero
    ROL_VENDEDOR <- 1
    ROL_ADMIN <- 2
	Definir usuario, clave Como Cadena
    Definir intentos Como Entero
    intentos <- 0
    Repetir
        Escribir "Ingrese usuario:"
        Leer usuario
        Escribir "Ingrese clave:"
        Leer clave
        Si usuario = "admin" Y clave = "1234" Entonces
            Escribir "Inicio de sesión exitoso (Administrador)."
            intentos <- 3 // Para salir del bucle
			rolUsuario <- 2
        SiNo
            Si usuario = "vendedor" Y clave = "5678" Entonces
                Escribir "Inicio de sesión exitoso (Vendedor)."
                intentos <- 3 // Para salir del bucle
				rolUsuario <- 1
            SiNo
                intentos <- intentos + 1
                Escribir "Usuario o clave incorrectos."
            FinSi
        FinSi
    Hasta Que intentos = 3
    Si intentos <> 3 Entonces
        Escribir "Inicio de sesión fallido. Cuenta bloqueada."
    FinSi
	
    // Llamar al menú principal con el rol del usuario
    menu_principal(rolUsuario)
FinAlgoritmo

Funcion menu_principal(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero 
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones(6)
    Dimensionar permisos(6)
	opciones[1] <- 'Inventario' 
	opciones[2] <- 'Ventas'
	opciones[3] <- 'Caja'
	opciones[4] <- 'Reportes'
	opciones[5] <- 'Administración'
	opciones[6] <- 'Mi cuenta'
	permisos[1] <- 1
	permisos[2] <- 1
	permisos[3] <- 1
	permisos[4] <- 1
	permisos[5] <- 2
	permisos[6] <- 1
	
    Repetir
        Escribir "======= MENÚ PRINCIPAL ======="
        num_opcion <- 1
        Para i <- 1 Hasta 6 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Salir"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion <- 1
        Para i <- 1 Hasta 6 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion Entonces
                    opcion_real <- i
                FinSi
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1: 
				menu_inventario(role)
            2:
				menu_ventas(role)
            3: 
				menu_caja(role)
			4:
				menu_reportes_estadisticas(role)
			5:
				menu_administracion(role)
			6: 
				menu_mi_cuenta(role)
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

// =================================== INVENTARIO ==========================================

Funcion menu_inventario(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones(3)
    Dimensionar permisos(3)
    opciones[1] <- "Gestión de Productos"
    opciones[2] <- "Control de Stock"
    opciones[3] <- "Descuentos y Promociones"
    permisos[1] <- 2
    permisos[2] <- 1
    permisos[3] <- 2
	
    Repetir
        Escribir "======= INVENTARIO ======="
        num_opcion <- 1
        Para i <- 1 Hasta 3 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Volver al Menú Principal"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion <- 1
        Para i <- 1 Hasta 3 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion Entonces
                    opcion_real <- i
                FinSi
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1: 
				menu_administracion_productos(role)
            2: 
				menu_control_stock(role)
            3: 
				menu_descuentos_promociones(role)
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

Funcion menu_administracion_productos(role)
    Definir opciones Como Cadena
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones[4]
    opciones[1] <- "Registrar Nuevo Producto"
    opciones[2] <- "Visualizar productos"
    opciones[3] <- "Actualizar Producto"
    opciones[4] <- "Eliminar Producto"
	
    Repetir
        Escribir "--- Gestión de Productos ---"
        num_opcion <- 1
        Para i <- 1 Hasta 4 Hacer
            Escribir num_opcion, ". ", opciones[i]
            num_opcion <- num_opcion + 1
        FinPara
        Escribir num_opcion, ". Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Ingrese nombre del producto: "
                Leer nombre_producto
                Escribir "Producto registrado: ", nombre_producto
            2:
                Escribir "Lista de productos:"
                Escribir "Producto 1: Nombre - Precio"
                Escribir "Producto 2: Nombre - Precio"
            3:
                Escribir "Función de Actualización aún no implementada."
            4:
                Escribir "Ingrese ID del producto a eliminar: "
                Leer id_producto
                Escribir "Producto ", id_producto, " eliminado."
            5:
                Escribir "Volviendo..."
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion


Funcion menu_control_stock(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones(4)
    Dimensionar permisos(4)
    opciones[1] <- "Ver Stock Actual"
    opciones[2] <- "Ver Productos con Bajo Stock"
    opciones[3] <- "Registrar Reposición de Stock"
    opciones[4] <- "Configurar Stock Mínimo"
    permisos[1] <- 1
    permisos[2] <- 1
    permisos[3] <- 2
    permisos[4] <- 2
	
    Repetir
        Escribir "--- Control de Stock ---"
        num_opcion <- 1
        Para i <- 1 Hasta 4 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion_real <- 1
        Para i <- 1 Hasta 4 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion_real Entonces
                    opcion_real <- i
                FinSi
                num_opcion_real <- num_opcion_real + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1:
                Escribir "Stock actual de productos:"
                Escribir "Producto 1: Cantidad - Precio"
                Escribir "Producto 2: Cantidad - Precio"
            2:
                Escribir "Productos con bajo stock:"
                Escribir "Producto 3: Cantidad - Precio"
            3:
                Escribir "Ingrese ID del producto a reponer: "
                Leer id_producto
                Escribir "Reponer cantidad: "
                Leer cantidad_reponer
                Escribir "Reposición registrada: Producto ", id_producto, " - Cantidad: ", cantidad_reponer
            4:
                Escribir "Ingrese ID del producto a configurar stock mínimo: "
                Leer id_producto
                Escribir "Ingrese stock mínimo: "
                Leer stock_minimo
                Escribir "Stock mínimo configurado: Producto ", id_producto, " - Stock mínimo: ", stock_minimo
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

Funcion menu_descuentos_promociones(role)
    Definir opciones Como Cadena
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones[3]
    opciones[1] <- "Aplicar Descuento a Producto"
    opciones[2] <- "Configurar Fechas de Descuento"
    opciones[3] <- "Ver Productos con Descuento"
	
    Repetir
        Escribir "--- Descuentos y Promociones ---"
        num_opcion <- 1
        Para i <- 1 Hasta 3 Hacer
            Escribir num_opcion, ". ", opciones[i]
            num_opcion <- num_opcion + 1
        FinPara
        Escribir num_opcion, ". Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Ingrese ID del producto a aplicar descuento: "
                Leer id_producto
                Escribir "Ingrese porcentaje de descuento: "
                Leer porcentaje_descuento
                Escribir "Descuento aplicado: Producto ", id_producto, " - Descuento: ", porcentaje_descuento, "%"
            2:
                Escribir "Ingrese fecha de inicio del descuento: "
                Leer fecha_inicio
                Escribir "Ingrese fecha de fin del descuento: "
                Leer fecha_fin
                Escribir "Fechas de descuento configuradas: Inicio: ", fecha_inicio, " - Fin: ", fecha_fin
            3:
                Escribir "Función aún no implementada."
            4:
                // Volver
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

// =================================== FIN INVENTARIO ==========================================

// =================================== VENTAS ==========================================

Funcion menu_ventas(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones[2]
    Dimensionar permisos[2]
    opciones[1] <- "Nueva Venta"
    opciones[2] <- "Historial de Ventas"
    permisos[1] <- 1
    permisos[2] <- 2
	
    Repetir
        Escribir "======= VENTAS ======="
        num_opcion <- 1
        Para i <- 1 Hasta 2 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Volver al Menú Principal"
        Escribir "====================="
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion_real <- 1
        Para i <- 1 Hasta 2 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion_real Entonces
                    opcion_real <- i
                FinSi
                num_opcion_real <- num_opcion_real + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1:
                Escribir "Ingrese ID del producto a vender: "
                Leer id_producto
                Escribir "Ingrese cantidad: "
                Leer cantidad
                Escribir "Venta registrada: Producto ", id_producto, " - Cantidad: ", cantidad
            2:
                Escribir "Historial de ventas:"
                Escribir "Venta 1: Producto - Cantidad - Total"
                Escribir "Venta 2: Producto - Cantidad - Total"
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

// =================================== FIN VENTAS ==========================================

// =================================== CAJA ==========================================

Funcion menu_caja(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones(3)
    Dimensionar permisos(3)
    opciones[1] <- "Apertura de Caja"
    opciones[2] <- "Ingresos/Egresos"
    opciones[3] <- "Corte de Caja"
    permisos[1] <- 1
    permisos[2] <- 1
    permisos[3] <- 1
	
    Repetir
        Escribir "======= CAJA ======="
        num_opcion <- 1
        Para i <- 1 Hasta 3 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Volver al Menú Principal"
        Escribir "===================="
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion_real <- 1
        Para i <- 1 Hasta 3 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion_real Entonces
                    opcion_real <- i
                FinSi
                num_opcion_real <- num_opcion_real + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1: 
				menu_apertura_caja(role)
            2: 
				menu_ingresos_egresos(role)
            3: 
				menu_corte_caja(role)
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

Funcion menu_apertura_caja(role)
    Definir opcion Como Entero
    Definir monto_inicial Como Real
    Definir estado_caja Como Cadena
	
    Repetir
        Escribir "--- Apertura de Caja ---"
        Escribir "1. Iniciar Nueva Caja"
        Escribir "2. Ver Estado Actual"
        Escribir "3. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Ingrese el monto inicial de la caja: "
                Leer monto_inicial
                // Aquí iría la lógica para iniciar una nueva caja con el monto inicial
                Escribir "Caja iniciada con monto inicial de: ", monto_inicial
            2:
                // Aquí iría la lógica para obtener el estado actual de la caja
                estado_caja <- "Abierta" // Ejemplo de estado
                Escribir "Estado actual de la caja: ", estado_caja
            3:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion = 3
FinFuncion

Funcion menu_ingresos_egresos(role)
    Definir opcion Como Entero
    Definir monto Como Real
    Definir descripcion Como Cadena
	
    Repetir
        Escribir "--- Ingresos/Egresos ---"
        Escribir "1. Registrar Ingreso"
        Si role = ROL_ADMIN Entonces
            Escribir "2. Registrar Egreso"
            Escribir "3. Ver Historial de Transacciones"
            Escribir "4. Volver"
        SiNo
            Escribir "2. Volver"
        FinSi
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Si role = ROL_ADMIN Entonces
            Segun opcion Hacer
                1:
                    Escribir "Ingrese el monto del ingreso: "
                    Leer monto
                    Escribir "Ingrese la descripción del ingreso: "
                    Leer descripcion
                    // Aquí iría la lógica para registrar el ingreso
                    Escribir "Ingreso registrado: ", monto, " - ", descripcion
                2:
                    Escribir "Ingrese el monto del egreso: "
                    Leer monto
                    Escribir "Ingrese la descripción del egreso: "
                    Leer descripcion
                    // Aquí iría la lógica para registrar el egreso
                    Escribir "Egreso registrado: ", monto, " - ", descripcion
                3:
                    // Aquí iría la lógica para listar las transacciones
                    Escribir "Historial de transacciones:"
                    Escribir "Ingreso: 100.00 - Venta de producto"
                    Escribir "Egreso: 50.00 - Compra de insumos"
                4:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        SiNo
            Segun opcion Hacer
                1:
                    Escribir "Ingrese el monto del ingreso: "
                    Leer monto
                    Escribir "Ingrese la descripción del ingreso: "
                    Leer descripcion
                    // Aquí iría la lógica para registrar el ingreso
                    Escribir "Ingreso registrado: ", monto, " - ", descripcion
                2:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        FinSi
    Hasta Que (role = ROL_ADMIN Y opcion = 4) O (role = ROL_VENDEDOR Y opcion = 2)
FinFuncion

Funcion menu_corte_caja(role)
    Definir opcion Como Entero
    Definir monto_cierre Como Real
	
    Repetir
        Escribir "--- Corte de Caja ---"
        Escribir "1. Realizar Corte"
        Escribir "2. Ver último Corte"
        Si role = ROL_ADMIN Entonces
            Escribir "3. Ver Historial de Cortes"
            Escribir "4. Volver"
        SiNo
            Escribir "3. Volver"
        FinSi
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Si role = ROL_ADMIN Entonces
            Segun opcion Hacer
                1:
                    // Aquí iría la lógica para realizar el corte de caja
                    monto_cierre <- 500.00 // Ejemplo de monto de cierre
                    Escribir "Corte de caja realizado. Monto de cierre: ", monto_cierre
                2:
                    // Aquí iría la lógica para ver el último corte de caja
                    Escribir "Último corte de caja: Monto de cierre: 500.00"
                3:
                    // Aquí iría la lógica para ver el historial de cortes de caja
                    Escribir "Historial de cortes de caja:"
                    Escribir "Corte 1: Monto de cierre: 450.00"
                    Escribir "Corte 2: Monto de cierre: 500.00"
                4:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        SiNo
            Segun opcion Hacer
                1:
                    // Aquí iría la lógica para realizar el corte de caja
                    monto_cierre <- 500.00 // Ejemplo de monto de cierre
                    Escribir "Corte de caja realizado. Monto de cierre: ", monto_cierre
                2:
                    // Aquí iría la lógica para ver el último corte de caja
                    Escribir "Último corte de caja: Monto de cierre: 500.00"
                3:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        FinSi
    Hasta Que (role = ROL_ADMIN Y opcion = 4) O (role = ROL_VENDEDOR Y opcion = 3)
FinFuncion

// =================================== FIN CAJA ==========================================

// =================================== REPORTES ==========================================
Funcion menu_reportes_estadisticas(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
		Definir opcion, num_opcion, i, opcion_real Como Entero
		Dimensionar opciones[4]
		Dimensionar permisos[4]

		opciones[1] <- "Reportes de Ventas"
		opciones[2] <- "Reportes de Inventario"
		opciones[3] <- "Reportes de Caja"
		opciones[4] <- "Exportación de Datos"
		
		permisos[1] <- 1
		permisos[2] <- 1
		permisos[3] <- 2
		permisos[4] <- 2
	
		Repetir
			Escribir "======= INVENTARIO ======="
			num_opcion <- 1
			Para i <- 1 Hasta 3 Hacer
				Si TienePermiso(role, permisos[i]) Entonces
					Escribir num_opcion, ". ", opciones[i]
					num_opcion <- num_opcion + 1
				FinSi
			FinPara
			Escribir num_opcion, ". Volver al Menú Principal"
			Escribir "Seleccione una opción: "
			Leer opcion
			
			opcion_real <- -1
			num_opcion <- 1
			Para i <- 1 Hasta 3 Hacer
				Si TienePermiso(role, permisos[i]) Entonces
					Si opcion = num_opcion Entonces
						opcion_real <- i
					FinSi
					num_opcion <- num_opcion + 1
				FinSi
			FinPara
			
			Segun opcion_real Hacer
				1: 
					menu_reportes_ventas(role)
				2: 
					menu_reportes_inventario(role)
				3: 
					menu_reportes_caja(role)
				4:
					menu_exportacion_datos(role)
			FinSegun
		Hasta Que opcion = num_opcion
FinFuncion

Funcion menu_reportes_ventas(role)
    Definir opcion Como Entero
    Definir fecha Como Cadena
    Definir mes, anio Como Entero
	
    Repetir
        Escribir "--- Reportes de Ventas ---"
        Escribir "1. Ventas por Día"
        Escribir "2. Ventas por Semana"
        Escribir "3. Ventas por Mes"
		
        Si role = ROL_ADMIN Entonces
            Escribir "4. Ventas por Vendedor"
            Escribir "5. Ventas por Producto"
            Escribir "6. Ventas por Método de Pago"
            Escribir "7. Volver"
        SiNo
            Escribir "4. Mis Ventas"
            Escribir "5. Volver"
        FinSi
		
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Ventas del día: [Lista de ventas]"
            2:
                Escribir "Ingrese la fecha (YYYY-MM-DD): "
                Leer fecha
                Escribir "Ventas de la semana: [Lista de ventas]"
            3:
                Escribir "Ingrese el año: "
                Leer anio
                Escribir "Ingrese el mes: "
                Leer mes
                Escribir "Ventas del mes: [Lista de ventas]"
            4:
                Si role = ROL_ADMIN Entonces
                    Escribir "Ventas por vendedor: [Lista de ventas por vendedor]"
                SiNo
                    Escribir "Mis ventas: [Lista de mis ventas]"
                FinSi
            5:
                Si role = ROL_ADMIN Entonces
                    Escribir "Ventas por producto: [Lista de ventas por producto]"
                SiNo
                    // Volver
                FinSi
            6:
                Si role = ROL_ADMIN Entonces
                    Escribir "Ventas por método de pago: [Lista de ventas por método de pago]"
                SiNo
                    Escribir "Opción inválida."
                FinSi
			7:
				Si role = ROL_ADMIN Entonces
					// Volver
				FinSi
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que (role = ROL_ADMIN Y opcion = 7) O (role = ROL_VENDEDOR Y opcion = 5)
FinFuncion

Funcion menu_reportes_inventario(role)
    Definir opcion Como Entero
	
    Repetir
        Escribir "--- Reportes de Inventario ---"
        Escribir "1. Stock Actual"
        Escribir "2. Productos más Vendidos"
        Escribir "3. Productos menos Vendidos"
		
        Si role = ROL_ADMIN Entonces
            Escribir "4. Rotación de Inventario"
            Escribir "5. Volver"
        SiNo
            Escribir "4. Volver"
        FinSi
		
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Si role = ROL_ADMIN Entonces
            Segun opcion Hacer
                1:
                    Escribir "Stock actual: [Lista de productos y stock]"
                2:
                    Escribir "Productos más vendidos: [Lista de productos]"
                3:
                    Escribir "Productos menos vendidos: [Lista de productos]"
                4:
                    Escribir "Rotación de inventario: [Datos de rotación]"
                5:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        SiNo
            Segun opcion Hacer
                1:
                    Escribir "Stock actual: [Lista de productos y stock]"
                2:
                    Escribir "Productos más vendidos: [Lista de productos]"
                3:
                    Escribir "Productos menos vendidos: [Lista de productos]"
                4:
                    // Volver
                De Otro Modo:
                    Escribir "Opción inválida."
            FinSegun
        FinSi
    Hasta Que (role = ROL_ADMIN Y opcion = 5) O (role = ROL_VENDEDOR Y opcion = 4)
FinFuncion

Funcion menu_reportes_caja(role)
    Definir opcion Como Entero
	
    Repetir
        Escribir "--- Reportes de Caja ---"
        Escribir "1. Balance Diario"
        Escribir "2. Balance Semanal"
        Escribir "3. Balance Mensual"
        Escribir "4. Reporte de Ingresos/Egresos"
        Escribir "5. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Balance diario: [Datos del balance]"
            2:
                Escribir "Balance semanal: [Datos del balance]"
            3:
                Escribir "Balance mensual: [Datos del balance]"
            4:
                Escribir "Reporte de ingresos/egresos: [Lista de transacciones]"
            5:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion = 5
FinFuncion

Funcion menu_exportacion_datos(role)
    Definir opcion Como Entero
	
    Repetir
        Escribir "--- Exportación de Datos ---"
        Escribir "1. Exportar a PDF"
        Escribir "2. Exportar a Excel"
        Escribir "3. Exportar a CSV"
        Escribir "4. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Exportando datos a PDF..."
                // Aquí iría la lógica para exportar a PDF
                Escribir "Exportación a PDF completada."
            2:
                Escribir "Exportando datos a Excel..."
                // Aquí iría la lógica para exportar a Excel
                Escribir "Exportación a Excel completada."
            3:
                Escribir "Exportando datos a CSV..."
                // Aquí iría la lógica para exportar a CSV
                Escribir "Exportación a CSV completada."
            4:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion = 4
FinFuncion

// =================================== FIN REPORTES ==========================================

// =================================== ADMINISTRACIÓN ==========================================
Funcion menu_administracion(role)
    Definir opciones Como Cadena
    Definir permisos Como Entero
    Definir opcion, num_opcion, i, opcion_real Como Entero
    Dimensionar opciones[2]
    Dimensionar permisos[2]
	
    opciones[1] <- "Gestión de Usuarios"
    opciones[2] <- "Registros de Auditoría"
	
    permisos[1] <- 2
    permisos[2] <- 2
	
    Repetir
        Escribir "======= ADMINISTRACIÓN ======="
        num_opcion <- 1
        Para i <- 1 Hasta 2 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Escribir num_opcion, ". ", opciones[i]
                num_opcion <- num_opcion + 1
            FinSi
        FinPara
        Escribir num_opcion, ". Volver al Menú Principal"
        Escribir "=============================="
        Escribir "Seleccione una opción: "
        Leer opcion
		
        opcion_real <- -1
        num_opcion_real <- 1
        Para i <- 1 Hasta 2 Hacer
            Si TienePermiso(role, permisos[i]) Entonces
                Si opcion = num_opcion_real Entonces
                    opcion_real <- i
                FinSi
                num_opcion_real <- num_opcion_real + 1
            FinSi
        FinPara
		
        Segun opcion_real Hacer
            1:
                menu_gestion_usuarios(role)
            2:
                menu_registro_auditoria(role)
        FinSegun
    Hasta Que opcion = num_opcion
FinFuncion

Funcion menu_gestion_usuarios(role)
    Definir opcion_real, success Como Entero
    Definir nombre_usuario Como Cadena
	
    Repetir
        Escribir "--- Gestión de Usuarios ---"
        Escribir "1. Ver Usuarios"
        Escribir "2. Crear Usuario"
        Escribir "3. Cambiar Estatus de Usuario"
        Escribir "4. Asignar Roles"
        Escribir "5. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion_real
		
        Segun opcion_real Hacer
            1:
                Escribir "Lista de usuarios: [Lista de usuarios]"
            2:
                Escribir "Ingrese el nombre del usuario: "
                Leer nombre_usuario
                Escribir "Ingrese el rol del usuario [2 - Admin / 1 - Vendedor]: "
                Leer role
                // Asumiendo que 'guardarUsuario' devuelve un valor booleano (0 o 1)
                Escribir "Usuario guardado exitosamente."
            3:
                Escribir "Función para cambiar estatus de usuario."
            4:
                Escribir "Función para asignar roles."
            5:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion_real = 5
FinFuncion

Funcion menu_registro_auditoria(role)
    Definir opcion Como Entero
	
    Repetir
        Escribir "--- Registro de Auditoría ---"
        Escribir "1. Ver Historial de Acciones"
        Escribir "2. Búsqueda Avanzada"
        Escribir "3. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion
		
        Segun opcion Hacer
            1:
                Escribir "Historial de acciones: [Lista de acciones]"
            2:
                Escribir "Función de búsqueda avanzada."
            3:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion = 3
FinFuncion

// =================================== FIN ADMINISTRACION ==========================================


// =================================== MI CUENTA ==========================================

Funcion menu_mi_cuenta(role)
    Definir opcion_real Como Entero
	
    Repetir
        Escribir "--- Mi Cuenta ---"
        Escribir "1. Cambiar contraseña"
        Escribir "2. Volver"
        Escribir "Seleccione una opción: "
        Leer opcion_real
		
        Segun opcion_real Hacer
            1:
                Escribir "Función cambiar contraseña."
                // Aquí iría la lógica para cambiar la contraseña
                Escribir "Contraseña cambiada exitosamente."
            2:
                // Volver
            De Otro Modo:
                Escribir "Opción inválida."
        FinSegun
    Hasta Que opcion_real = 2
FinFuncion