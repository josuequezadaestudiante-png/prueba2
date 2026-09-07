//Version final de la simulacion del parqueadero en C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
int LeerEntero();
int MostrarMenu(int espacios);
void MostrarEstadoCajones(int cajones[], int m_dia[], int m_mes[], int m_h[], int m_min[]);
float ObtenerTarifa();
int LeerFechaHora(int tipo_evento);
int CalcularEstancia(int minutos_entrada, int minutos_salida);
float AplicarCobro(int tiempo_estancia, float tarifa);
void ImprimirTicket(int tiempo_estancia, float monto_pagar, int espacios_restantes, int d_in, int m_in, int h_in, int min_in, int d_out, int m_out, int h_out, int min_out);

int temp_mes, temp_dia, temp_h, temp_m;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleOutputCP(65001);
	int espacios_disponibles, opcion_menu;
	float tarifa_actual, monto_final;
	int min_entrada, min_salida, tiempo_total;
	
	bool validacion_fechas;
	int resp_cruce;
	int i, cajon_elegido;
	
	int cajones_entrada[10];
	int mem_dia[10], mem_mes[10], mem_h[10], mem_m[10];
	int out_dia, out_mes, out_h, out_m;
	
	for (i = 0; i < 10; i++) {
		cajones_entrada[i] = -1;
		mem_dia[i] = 0;
		mem_mes[i] = 0;
		mem_h[i] = 0;
		mem_m[i] = 0;
	}
	
	do {
		espacios_disponibles = 0;
		for (i = 0; i < 10; i++) {
			if (cajones_entrada[i] == -1) {
				espacios_disponibles++;
			}
		}
		
		opcion_menu = MostrarMenu(espacios_disponibles);
		
		switch (opcion_menu) {
		case 1: 
			if (espacios_disponibles == 0) {
				printf("\n>> AVISO: El parqueadero esta completamente lleno.\n");
				system("pause"); 
			} else {
				do {
					system("cls"); 
					printf("=========================================\n");
					printf("        REGISTRO DE ENTRADA (1-10)       \n");
					printf("=========================================\n");
					printf("Ingrese el numero de cajon a ocupar: ");
					cajon_elegido = LeerEntero();
					if (cajon_elegido < 1 || cajon_elegido > 10) {
						printf(">> ERROR: Espacio invalido.\n");
						system("pause");
					}
				} while (cajon_elegido < 1 || cajon_elegido > 10);
				
				if (cajones_entrada[cajon_elegido - 1] != -1) {
					printf("\n>> ERROR: El cajon %d ya esta ocupado fisicamente.\n", cajon_elegido);
					printf("Por favor, elija otro espacio.\n");
					system("pause");
				} else {
					min_entrada = LeerFechaHora(1);
					cajones_entrada[cajon_elegido - 1] = min_entrada;
					mem_dia[cajon_elegido - 1] = temp_dia;
					mem_mes[cajon_elegido - 1] = temp_mes;
					mem_h[cajon_elegido - 1] = temp_h;
					mem_m[cajon_elegido - 1] = temp_m;
					
					system("cls");
					printf(">> �Ingreso registrado exitosamente en el cajon %d!\n", cajon_elegido);
					system("pause");
				}
			}
			break;
			
		case 2: 
			if (espacios_disponibles == 10) {
				printf("\n>> AVISO: El parqueadero esta completamente vacio.\n");
				printf("No hay vehiculos para retirar.\n");
				system("pause");
			} else {
				do {
					system("cls");
					printf("=========================================\n");
					printf("        REGISTRO DE SALIDA (1-10)        \n");
					printf("=========================================\n");
					printf("Ingrese el numero de cajon a desocupar: ");
					cajon_elegido = LeerEntero();
					if (cajon_elegido < 1 || cajon_elegido > 10) {
						printf(">> ERROR: Espacio invalido.\n");
						system("pause");
					}
				} while (cajon_elegido < 1 || cajon_elegido > 10);
				
				if (cajones_entrada[cajon_elegido - 1] == -1) {
					printf("\n>> ERROR: El cajon %d esta vacio. No hay vehiculo ahi.\n", cajon_elegido);
					system("pause");
				} else {
					min_entrada = cajones_entrada[cajon_elegido - 1];
					
					do {
						validacion_fechas = true;
						min_salida = LeerFechaHora(2);
						out_dia = temp_dia; out_mes = temp_mes; 
						out_h = temp_h; out_m = temp_m;
						
						if (min_salida < min_entrada) {
							do {
								system("cls");
								printf(">> ALERTA LOGICA DE TIEMPO <<\n");
								printf("Entrada: %02d/%02d %02d:%02d\n", mem_dia[cajon_elegido-1], mem_mes[cajon_elegido-1], mem_h[cajon_elegido-1], mem_m[cajon_elegido-1]);
								printf("La fecha/hora de salida es anterior a la entrada.\n");
								printf("�Que provoco esto?\n");
								printf("1. Error de digitacion (Volver a escribir fechas)\n");
								printf("2. El vehiculo cruzo de a�o (Estancia mayor a 1 a�o)\n");
								printf("Seleccione (1-2): ");
								
								resp_cruce = LeerEntero();
								if (resp_cruce != 1 && resp_cruce != 2) {
									printf(">> Error: Seleccione 1 o 2.\n");
									system("pause");
								}
							} while (resp_cruce != 1 && resp_cruce != 2);
							
							if (resp_cruce == 1) {
								validacion_fechas = false;
							} else {
								min_salida = min_salida + 525600;
							}
						}
					} while (validacion_fechas == false);
					
					tiempo_total = CalcularEstancia(min_entrada, min_salida);
					
					tarifa_actual = ObtenerTarifa();
					monto_final = AplicarCobro(tiempo_total, tarifa_actual);
					
					espacios_disponibles = espacios_disponibles + 1;
					
					ImprimirTicket(tiempo_total, monto_final, espacios_disponibles, 
								   mem_dia[cajon_elegido-1], mem_mes[cajon_elegido-1], mem_h[cajon_elegido-1], mem_m[cajon_elegido-1], 
								   out_dia, out_mes, out_h, out_m);
					
					cajones_entrada[cajon_elegido - 1] = -1;
					mem_dia[cajon_elegido - 1] = 0;
					mem_mes[cajon_elegido - 1] = 0;
					mem_h[cajon_elegido - 1] = 0;
					mem_m[cajon_elegido - 1] = 0;
				}
			}
			break;
			
		case 3: 
			MostrarEstadoCajones(cajones_entrada, mem_dia, mem_mes, mem_h, mem_m);
			break;
		}
		
	} while (opcion_menu != 4);
	
	system("cls");
	printf("=========================================\n");
	printf("      Cerrando el sistema... �Buen dia!  \n");
	printf("=========================================\n");
	
	return 0; 
}

int LeerEntero() {
	int numero;
	scanf("%d", &numero);
	while (getchar() != '\n'); 
	return numero;
}

int MostrarMenu(int espacios) {
	int opcion_elegida;
	do {
		system("cls"); 
		printf("                   ______\n");
		printf("                  /|_||_\\`.__\n");
		printf("                 (   _    _ _\\\n");
		printf("                  =-(_)--(_)-\n");
		printf("==========================================\n");
		printf("   Bienvenido al sistema de parqueadero\n");
		printf("              PARQUEADERO  \n");
		printf("=========================================\n");
		printf("Espacios disponibles actuales: %d\n", espacios);
		printf("-----------------------------------------\n");
		printf("1. Registrar ENTRADA de vehiculo\n");
		printf("2. Registrar SALIDA y Cobrar\n");
		printf("3. Ver ESTADO de los espacios\n");
		printf("4. Salir del sistema\n");
		printf("=========================================\n");
		printf("Seleccione una opcion (1-4): ");
		
		opcion_elegida = LeerEntero();
		
		if (opcion_elegida < 1 || opcion_elegida > 4) {
			printf("\n>> ERROR: Opcion invalida. Por favor, elija del 1 al 4.\n");
			system("pause"); 
		}
	} while (opcion_elegida < 1 || opcion_elegida > 4);
	
	return opcion_elegida; 
}

void MostrarEstadoCajones(int cajones[], int m_dia[], int m_mes[], int m_h[], int m_min[]) {
	int i;
	system("cls");
	printf("=========================================\n");
	printf("       ESTADO ACTUAL DEL PARQUEADERO     \n");
	printf("=========================================\n");
	for (i = 0; i < 10; i++) {
		if (cajones[i] == -1) {
			printf("Cajon %02d: [ DISPONIBLE ]\n", i + 1);
		} else {
			printf("Cajon %02d: [ OCUPADO ] - Ingreso el: %02d/%02d a las %02d:%02d\n", 
				   i + 1, m_dia[i], m_mes[i], m_h[i], m_min[i]);
		}
	}
	printf("=========================================\n\n");
	printf("Presione Enter para volver al menu principal...\n");
	system("pause");
}

float ObtenerTarifa() {
	int tipo;
	float tarifa = 0.0;
	do {
		system("cls");
		printf("=========================================\n");
		printf("            TIPO DE VEHICULO             \n");
		printf("=========================================\n");
		printf("1. Motocicleta\n");
		printf("2. Automovil\n");
		printf("3. Camioneta\n");
		printf("Seleccione el tipo (1-3): ");
		
		tipo = LeerEntero();
		if (tipo < 1 || tipo > 3) {
			printf("\n>> ERROR: Tipo de vehiculo no reconocido.\n");
			system("pause");
		}
	} while (tipo < 1 || tipo > 3);
	
	switch(tipo) {
	case 1: tarifa = 0.05; break;
	case 2: tarifa = 0.10; break;
	case 3: tarifa = 0.15; break;
	}
	return tarifa;
}

int LeerFechaHora(int tipo_evento) {
	int max_dias, i, dias_acumulados = 0, min_totales;
	
	do {
		system("cls");
		printf("=========================================\n");
		if(tipo_evento == 1) printf("    FECHA Y HORA DE ENTRADA\n");
		else printf("    FECHA Y HORA DE SALIDA\n");
		printf("=========================================\n");
		printf("Ingrese el MES numerico (1-12): ");
		temp_mes = LeerEntero();
		if (temp_mes < 1 || temp_mes > 12) {
			printf("\n>> ERROR: El mes debe estar entre 1 y 12.\n");
			system("pause");
		}
	} while (temp_mes < 1 || temp_mes > 12);
	
	switch(temp_mes) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12: max_dias = 31; break;
	case 4: case 6: case 9: case 11: max_dias = 30; break;
	case 2: max_dias = 29; break;
	}
	
	do {
		system("cls");
		printf("=========================================\n");
		if(tipo_evento == 1) printf("    FECHA Y HORA DE ENTRADA\n");
		else printf("    FECHA Y HORA DE SALIDA\n");
		printf("=========================================\n");
		printf("Mes: %d\n", temp_mes);
		printf("Ingrese el DIA (1-%d): ", max_dias);
		temp_dia = LeerEntero();
		if (temp_dia < 1 || temp_dia > max_dias) {
			printf("\n>> ERROR: Dia valido de 1 a %d.\n", max_dias);
			system("pause");
		}
	} while (temp_dia < 1 || temp_dia > max_dias);
	
	do {
		system("cls");
		printf("=========================================\n");
		if(tipo_evento == 1) printf("    FECHA Y HORA DE ENTRADA (Formato 24h)\n");
		else printf("    FECHA Y HORA DE SALIDA (Formato 24h)\n");
		printf("=========================================\n");
		printf("Fecha: %d/%d\n", temp_dia, temp_mes);
		printf("Ingrese la HORA (0-23): ");
		temp_h = LeerEntero();
		if (temp_h < 0 || temp_h > 23) {
			printf("\n>> ERROR: La hora debe estar entre 0 y 23.\n");
			system("pause");
		}
	} while (temp_h < 0 || temp_h > 23);
	
	do {
		system("cls");
		printf("=========================================\n");
		if(tipo_evento == 1) printf("    FECHA Y HORA DE ENTRADA (Formato 24h)\n");
		else printf("    FECHA Y HORA DE SALIDA (Formato 24h)\n");
		printf("=========================================\n");
		printf("Fecha: %d/%d | Hora: %d\n", temp_dia, temp_mes, temp_h);
		printf("Ingrese los MINUTOS (0-59): ");
		temp_m = LeerEntero();
		if (temp_m < 0 || temp_m > 59) {
			printf("\n>> ERROR: Los minutos deben estar entre 0 y 59.\n");
			system("pause");
		}
	} while (temp_m < 0 || temp_m > 59);
	
	for (i = 1; i <= temp_mes - 1; i++) {
		switch (i) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12: dias_acumulados += 31; break;
		case 4: case 6: case 9: case 11: dias_acumulados += 30; break;
		case 2: dias_acumulados += 28; break;
		}
	}
	dias_acumulados = dias_acumulados + (temp_dia - 1);
	
	min_totales = (dias_acumulados * 1440) + (temp_h * 60) + temp_m;
	return min_totales;
}

int CalcularEstancia(int minutos_entrada, int minutos_salida) {
	if (minutos_salida < minutos_entrada) {
		return 0;
	} else {
		return minutos_salida - minutos_entrada;
	}
}

float AplicarCobro(int tiempo_estancia, float tarifa) {
	char tiene_cupon;
	float monto = tiempo_estancia * tarifa;
	
	do {
		system("cls");
		printf("=========================================\n");
		printf("           VALIDACION DE CUPON           \n");
		printf("=========================================\n");
		printf("�El cliente tiene un cupon de descuento? (S/N): ");
		scanf(" %c", &tiene_cupon);
		while (getchar() != '\n'); 
		
		if (tiene_cupon != 'S' && tiene_cupon != 's' && tiene_cupon != 'N' && tiene_cupon != 'n') {
			printf("\n>> ERROR: Ingrese unicamente S o N.\n");
			system("pause");
		}
	} while (tiene_cupon != 'S' && tiene_cupon != 's' && tiene_cupon != 'N' && tiene_cupon != 'n');
	
	if (tiene_cupon == 'S' || tiene_cupon == 's') {
		monto = monto - (monto * 0.20);
		printf("\n>> �Cupon aplicado exitosamente! (20%% de descuento)\n");
	} else {
		printf("\n>> Sin descuento. Cobro a tarifa regular.\n");
	}
	system("pause");
	return monto;
}

void ImprimirTicket(int tiempo, float monto, int espacios, int d_in, int m_in, int h_in, int min_in, int d_out, int m_out, int h_out, int min_out) {
	printf("\n=========================================\n");
	printf("            TICKET DE SALIDA             \n");
	printf("=========================================\n");
	printf("Registro Entrada   : %02d/%02d %02d:%02d\n", d_in, m_in, h_in, min_in);
	printf("Registro Salida    : %02d/%02d %02d:%02d\n", d_out, m_out, h_out, min_out);
	printf("Tiempo de estancia : %d minutos\n", tiempo);
	printf("Monto a pagar      : $%.2f\n", monto);
	printf("Espacios restantes : %d\n", espacios);
	printf("=========================================\n\n");
	printf("Presione Enter para continuar...\n");
	system("pause");
}
//proyecto previo para posterior mejora e implementacion en un formato fisico de parqueadero en maqueta a pequeña escala 
