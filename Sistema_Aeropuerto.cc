/* Modelo MM1 - Un Servidor, Una Cola  */

#include <stdio.h>
#include <stdlib.h> 
#include "simlib.c"
// Definición de eventos
#define Arribo_Migracion           1  /* Tipo de Evento 1: Arribo Migracion           */
#define Partida_Aduana			   2  /* Tipo de Evento 2: Partida Aduana*/
#define Partida_Migracion		   3  /*Tipo de evento  3: Partida Migracion*/

// Definición de Colas y listas
#define Agente_1         1  
#define Agente_2         2 
#define Agente_Control   3
#define Cola_Aduana      4
#define Cola_Migracion   5

// Definición Sampst
#define Media_Demora_colas 

/*Definición Posiciones Transfer*/  
#define Tipo_Evento       1
#define Tiempo_Evento	  2
#define Agente			  3
#define Cola              4


/* Declaraci¢n de variables  */
bool Control
int media_interarribos_aduana, media_servicio_no_revision, min_servicio_revision, max_servicio_revision, numero_pasajeros, aviones_por_dia, cantidad_dias ;
float porcentaje_pasajeros_revisados

/* Definición Rutinas */
int main(); /* Main function. */
void inicializar(void)
void Rutina_Arribo_Migracion(void)
void Generar_Arribo_Migracion(void)
void Rutina_Partida_Aduana(void)
void Generar_Partida_Aduana(int Agente)
void Rutina_Partida_Control_equipaje(void)
void Generar_Partida_Control_equipaje(int Agente, bool Control)
void Reporte();
// Principal
int main()
{
	// Inicializar simlib
	init_simlib();

	// Rutina inicializar
	inicializar();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados  */
	maxatr = 4;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */

	clientes_act = 0;

	while (sim_time <= cantidad_dias*86400)
	{

		/* Determinar pr¢ximo Evento */
		timing();

		/* Determinar tipo de evento y llamar a la rutina */

		switch( next_event_type )
		{
			case Arribo_Migracion:
				Rutina_Arribo_Migracion();
				break;
			case Partida_Aduana:
				Rutina_Partida_Aduana();
				break;
			case Partida_Control_equipaje:
				Rutina_Partida_Control_equipaje
			default
			break
        }
	}

	/* Llamada al Reporte para mostrar los resultados */
	reporte();
	//fin;
	system("pause");
}

/* Inicializar el Sistema */
void inicializa(void)  
{

	/* Se carga el primer Arribo en la Lista de Eventos */

	transfer[1] = sim_time + expon(media_interarribos,Arribo);
	transfer[2] = Arribo;
	list_file(INCREASING,LIST_EVENT);

}


void Rutina_arribos(void)  /* Evento Arribo */
{
	/* Determinar pr¢ximo arribo y cargar en Lista de Eventos */

	transfer[1] = sim_time + expon(media_interarribos,Arribo);
	transfer[2] = Arribo;
	list_file(INCREASING,LIST_EVENT);

	/* Chequear si el Servidor est  desocupado */

	if (list_size[Servidor] == 0)
	{

	   ++clientes_act;

	   /* Si está desocupado ocuparlo y generar la partida */

	   list_file(FIRST, Servidor);
	   sampst(0.0, Demora_cola);


	   transfer[1] = sim_time + expon(media_servicio, Partida);
	   transfer[2] = Partida;
	   list_file(INCREASING,LIST_EVENT);

	}

	else
	{

	   /* Si el Servidor est  ocupado poner el Trabajo en Cola */

	   transfer[1] = sim_time;
	   list_file(LAST, Cola);
	}

}


void Rutina_partidas(void)  /* Evento Partida */
{

	/* Desocupar el Servidor */

	list_remove(FIRST, Servidor);

	/* Ver si hay trabajos en cola */

	if (list_size[Cola] > 0)
	{

	   /* Sacar el primero de la cola y actualizar Demoras */

	   ++clientes_act;
	   list_remove(FIRST, Cola);
	   sampst(sim_time - transfer[1], Demora_cola);

	   /* Cargar en el Servidor y generar la partida */
	   list_file(FIRST, Servidor);

	   transfer[1] = sim_time + expon(media_servicio, Partida);
	   transfer[2] = Partida;
	   list_file(INCREASING,LIST_EVENT);
	}
}


void reporte( void )  /* Generar Reporte de la Simulaci¢n */
{

	/* Mostrar Par metros de Entrada */

	/* -------- Por pantalla -------- */

	printf( "Sistema M/M/1 - Simulaci¢n con Simlib \n\n" );
	printf( "Media de Interarribos          : %8.3f minutos\n", media_interarribos);
	printf( "Media de Servicios             : %8.3f minutos\n", media_servicio);
	printf( "Cantidad de Clientes Demorados : %i \n\n", num_clientes);

	/* Calcular los Estad¡sticos */

	/* Estad¡sticos Escalaras - Sampst */
	sampst(0.0,-Demora_cola);
	printf( "\nDemora en Cola                 : %f \n ",transfer[1]);


	/* Estad¡sticos Temporales - Timest y Filest */

	filest(Cola);
	printf( "\nN£mero Promedio en Cola        : %f \n ",transfer[1]);
	filest(Servidor);
	printf( "\nUtilizaci¢n Servidor           : %f \n ",transfer[1]);
	printf( "\nTiempo Final de Simulation     : %10.3f minutes\n", sim_time );


		/* Estad¡sticos Temporales - Timest y Filest */

	filest(Cola);
	printf( "\nN£mero Promedio en Cola        : %f \n ",transfer[1]);
	filest(Servidor);
	printf( "\nUtilizaci¢n Servidor           : %f \n ",transfer[1]);
	printf( "\nSimulation end time            : %10.3f minutes\n", sim_time );


}


