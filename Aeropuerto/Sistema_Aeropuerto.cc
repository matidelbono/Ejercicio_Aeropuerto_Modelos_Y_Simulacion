#include <stdio.h>
#include <stdlib.h> 
#include "simlib.c"
// Definición de eventos
#define Arribo_Avion				1
#define Arribo_Aduana				2  /* Tipo de Evento 1: Arribo Aduana */
#define Partida_Aduana				3  /* Tipo de Evento 2: Partida Aduana */
#define Partida_Control_Equipaje	4  /*Tipo de evento  3: Partida Migracion */

// Definición de Colas y listas
#define Agente_Aduana_1         1  
#define Agente_Aduana_2         2 
#define Agente_Control_3		3
#define Cola_Aduana				4
#define Cola_Control_equipaje   5

// Definición Sampst
#define Media_Demora_Cola_Aduana 1
#define Media_Demora_Cola_Control_Equipaje 2


/*Definición Posiciones Transfer*/
#define Tiempo_Evento			1
#define Tipo_Evento				2
#define Cantidad_Pasajeros		3 //Si es necesario
#define Agente					4 //Si es necesario



/* Declaraci¢n de variables  */
int cod_control, cod_aleatorio, media_interarribos_aduana, media_servicio_no_revision, min_servicio_revision, max_servicio_revision, numero_min_pasajeros, numero_max_pasajeros, media_aviones_por_dia, cantidad_dias, min_numero_aleatorio, max_numero_aleatorio, media_interarribos_avion;
float porcentaje_pasajeros_revisados;
int contador_revison, contador_pasajeros;

/* Definición Rutinas */
int main(); /* Main function. */
void inicializa(void);
void Rutina_Arribo_Avion(void);
void Generar_Arribo_Avion(void);
void Rutina_Arribo_Aduana(void);
void Generar_Arribo_Aduana(int cantidadPasajeros);
void Rutina_Partida_Aduana(void);
void Generar_Partida_Aduana(int agente);
void Rutina_Partida_Control_equipaje(void);
void Generar_Partida_Control_equipaje(void);
void Estadisticas();

// Principal
int main()
{
	// Inicializar simlib
	init_simlib();

	// Rutina inicializar
	inicializa();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados  */
	maxatr = 4;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */


	while (sim_time <= cantidad_dias * 86400)
	{

		/* Determinar pr¢ximo Evento */
		timing();

		/* Determinar tipo de evento y llamar a la rutina */

		switch (next_event_type)
		{
		case Arribo_Avion:
			Rutina_Arribo_Avion();
			break;
		case Arribo_Aduana:
			Rutina_Arribo_Aduana();
			break;
		case Partida_Aduana:
			Rutina_Partida_Aduana();
			break;
		case Partida_Control_Equipaje:
			Rutina_Partida_Control_equipaje();
			break;
		}
	}

	/* Llamada al Reporte para mostrar los resultados */
	Estadisticas();
	//fin;
	system("pause");
}

/* Inicializar el Sistema */
void inicializa(void)
{
	// asignar valores  a las variables
	media_interarribos_avion = 3 * 3600;
	media_interarribos_aduana = 0.2;
	media_servicio_no_revision = 0.55;
	min_servicio_revision = 3;
	max_servicio_revision = 6;
	numero_min_pasajeros = 240;
	numero_max_pasajeros = 350;
	media_aviones_por_dia = 8;
	cantidad_dias = 7;
	min_numero_aleatorio = 1;
	max_numero_aleatorio = 10;
	cod_control = 0;
	cod_aleatorio = uniform(1, 10, 20);

	/* Se carga el primer Arribo del primer avion en la Lista de Eventos */
	Generar_Arribo_Avion();

}

void Rutina_Arribo_Avion(void) {
	int cantidadPasajeros = transfer[3];

	//Generamos el arribo a aduana
	Generar_Arribo_Aduana(cantidadPasajeros);

	//Genero proximo arribo avion
	Generar_Arribo_Avion();
}
void Generar_Arribo_Avion(void) {
	transfer[1] = sim_time + expon(media_interarribos_avion, Arribo_Avion);
	transfer[2] = Arribo_Avion;
	transfer[3] = uniform(numero_min_pasajeros, numero_max_pasajeros, Arribo_Avion);

	list_file(INCREASING, LIST_EVENT);
}
void Rutina_Arribo_Aduana(void)  /* Evento Arribo */
{
	//Descontar un pasajero
	int cantidadPasajeros = transfer[3];
	cantidadPasajeros--;

	//Sumo un pasajero al contador para las estadisticas
	contador_pasajeros++;

	//Genero el proximo arribo a Aduana
	if (cantidadPasajeros > 1)
	{
		Generar_Arribo_Aduana(cantidadPasajeros);
	}

	//Ver si debe ser atendido rapidamente o ir al control de equipaje
	cod_control++;
	if (cod_aleatorio == cod_control)
	{
		//Debe ir al control de equipaje 
		cod_control = 0;
		cod_aleatorio = uniform(1, 10, 20);

		//Ver si puede ser atendido
		if (list_size[Agente_Control_3] == 0)
		{
			list_file(FIRST, Agente_Control_3);

			//Genero la partida de control equipaje y actualizo demora
			sampst(0.0, Media_Demora_Cola_Control_Equipaje);
			Generar_Partida_Control_equipaje();
		}
		else
		{
			//Mando a cola 
			transfer[1] = sim_time;
			list_file(LAST, Cola_Control_equipaje);
		}
	}
	else
	{
		//Debe pasar por la aduana rapida

		if (list_size[Agente_Aduana_1] == 0)
		{
			list_file(FIRST, Agente_Aduana_1);

			//Genero la partida de aduana rapida y actualizo demora
			sampst(0.0, Media_Demora_Cola_Aduana);
			Generar_Partida_Aduana(1);
		}
		else if (list_size[Agente_Aduana_2] == 0)
		{

			list_file(FIRST, Agente_Aduana_2);

			//Genero la partida de aduana rapida
			sampst(0.0, Media_Demora_Cola_Aduana);
			Generar_Partida_Aduana(2);
		}
		else
		{
			//Va a cola
			transfer[1] = sim_time;
			list_file(LAST, Cola_Aduana);
		}
	}

}
void Generar_Arribo_Aduana(int cantidadPasajeros)
{
	transfer[1] = sim_time + expon(media_interarribos_aduana, Arribo_Aduana);
	transfer[2] = Arribo_Aduana;
	transfer[3] = cantidadPasajeros;

	list_file(INCREASING, LIST_EVENT);
}
void Rutina_Partida_Aduana()
{
	// Desocupar el agente que atendió al pasajero
	int NroAgente = transfer[3];
	list_remove(FIRST, NroAgente);

	// ver si hay trabajo en cola
	if (list_size[Cola_Aduana] > 0)
	{
		//Saco de la cola
		list_remove(FIRST, Cola_Aduana);

		//Actualizo demora
		sampst(sim_time - transfer[1], Media_Demora_Cola_Aduana);

		//Ocupar agente
		list_file(FIRST, NroAgente);


		//Generar proxima partida aduana
		Generar_Partida_Aduana(NroAgente);
	}
}
void Generar_Partida_Aduana(int agente)
{
	transfer[1] = sim_time + expon(media_servicio_no_revision, Partida_Aduana);
	transfer[2] = Partida_Aduana;
	transfer[3] = agente;

	list_file(INCREASING, LIST_EVENT);
}
void Rutina_Partida_Control_equipaje(void) {

	//Desocupar al agente
	list_remove(FIRST, Agente_Control_3);

	//ver si hay trabajo en cola
	if (list_size[Cola_Control_equipaje] > 0)
	{
		//Saco el primero de la cola
		list_remove(FIRST, Cola_Control_equipaje);

		//Ocupo el agente
		list_file(FIRST, Agente_Control_3);

		//Actualizo demora
		sampst(sim_time - transfer[1], Media_Demora_Cola_Control_Equipaje);

		//Genero la proxima partida de control equipaje
		Generar_Partida_Control_equipaje();
	}
}
void Generar_Partida_Control_equipaje(void) {

	//Sumo 1 al contador de pasajeros que fueron al control de equipaje para las estadisticas
	contador_revison++;

	transfer[1] = sim_time + uniform(min_servicio_revision, max_servicio_revision, Partida_Control_Equipaje);
	transfer[2] = Partida_Control_Equipaje;

	list_file(INCREASING, LIST_EVENT);
}

void Estadisticas() {
	printf("Comienza reporte... \n\n");

	//Utilizacion de cada agente
	filest(Agente_Aduana_1);
	printf("\n Utilizacion Agente de la aduana 1: %f \n ", transfer[1]);

	filest(Agente_Aduana_2);
	printf("\n Utilizacion Agente de la aduana 2: %f \n ", transfer[1]);

	filest(Agente_Control_3);
	printf("\n Utilizacion Agente de control de equipaje: %f \n ", transfer[1]);

	//Demora media de los pasajeros en cola de aduana y cola de control equipaje
	sampst(0.0, -Media_Demora_Cola_Aduana);
	printf("\nDemora en cola de aduana: %f \n ", transfer[1]);

	sampst(0.0, -Media_Demora_Cola_Control_Equipaje);
	printf("\nDemora en cola de control de equipaje: %f \n ", transfer[1]);

	//Número medio en cola de pasajeros en las dos colas
	filest(Cola_Aduana);
	printf("\n Numero Promedio en Cola de aduana: %f \n ", transfer[1]);

	filest(Cola_Control_equipaje);
	printf("\n Numero Promedio en Cola de control de equipaje: %f \n ", transfer[1]);

	//Porcentaje de pasajeros a los que se les revisa el equipaje
	int porcentaje = (contador_revison * 100) / contador_pasajeros;
	printf("\n Porcentaje de pasajeros a los que se les revisa el equipaje: %f \n ", porcentaje);
}


