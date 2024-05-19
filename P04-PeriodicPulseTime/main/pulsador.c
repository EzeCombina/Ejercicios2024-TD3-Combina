/*==================[ Inclusiones ]============================================*/
#include "../include/pulsador.h"
#include "esp_log.h"

/*==================[ Definiciones ]===================================*/
#define T_REBOTE_MS     40
#define T_REBOTE        pdMS_TO_TICKS(T_REBOTE_MS)

/*==================[Prototipos de funciones]======================*/
static void errorPulsador(uint8_t indice);
static void botonPresionado(uint8_t indice);
static void botonLiberado(uint8_t indice);

void TaskPulsador(void *taskParmPtr);

/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[N_PULSADOR] = {GPIO_NUM_12};

pulsadorInfo pulsador[N_PULSADOR];

TickType_t tiempo, tiempoflag;

static const char *TAG = "PULSADOR";

/*==================[Implementaciones]=================================*/
TickType_t obtenerDiferencia(uint8_t indice, TickType_t periodo)
{
    if(pulsador[indice].diferenciaTiempo < periodo)
    {
        tiempo = pulsador[indice].diferenciaTiempo;
        tiempoflag = tiempo;
        return tiempo;
    }
    else
    {
        ESP_LOGI(TAG, "El valor de tiempo ingresado es mayor al periodo.");
        pulsador[indice].diferenciaTiempo = tiempoflag;
        return tiempoflag;
    }
}

void inicializarPulsador(void)
{
    for(int i = 0; i < N_PULSADOR; i++)
    {
        pulsador[i].tecla             = pulsadorPines[i];
        pulsador[i].estado            = BAJO;                     // Estado inicial
        pulsador[i].tiempoBajo        = TIEMPO_NO_VALIDO;
        pulsador[i].tiempoAlto        = TIEMPO_NO_VALIDO;
        pulsador[i].diferenciaTiempo  = TIEMPO_NO_VALIDO;

        esp_rom_gpio_pad_select_gpio(pulsador[i].tecla);
        gpio_set_direction(pulsador[i].tecla , GPIO_MODE_INPUT);
        gpio_set_pull_mode(pulsador[i].tecla, GPIO_PULLDOWN_ONLY); // Habilita resistencia de PULLDOWN interna
    }
 
    // Crear tareas en freeRTOS
    BaseType_t err = xTaskCreatePinnedToCore(
    	TaskPulsador,                     	    // Funcion de la tarea a ejecutar
        "TaskPulsador",   	                    // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		    // Cantidad de stack de la tarea
        NULL,                          	        // Parametros de tarea
        tskIDLE_PRIORITY+1,         	        // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		    // Puntero a la tarea creada en el sistema
        1                                       // Numero de procesador
    );

    // Gestion de errores
	if(err == pdFAIL)
	{
		ESP_LOGI(TAG, "Error al crear la tarea.");
		while(1);   // Si no pudo crear la tarea queda en un bucle infinito
	}
}

static void errorPulsador(uint8_t indice)
{
    ESP_LOGI(TAG, "Error al pulsar boton.");
    pulsador[indice].estado = BAJO;
}

// Actualización de pulsador
void actualizarPulsador(uint8_t indice)
{
    switch(pulsador[indice].estado)
    {
        case BAJO:
            if(gpio_get_level(pulsador[indice].tecla))
            {
                pulsador[indice].estado = ASCENDENTE;
            }
            break;

        case ASCENDENTE:
            if(gpio_get_level(pulsador[indice].tecla))
            {
                pulsador[indice].estado = ALTO;
                botonPresionado(indice);
            }else{pulsador[indice].estado = BAJO;}
            break;

        case ALTO:
            if(!gpio_get_level(pulsador[indice].tecla))
            {
                pulsador[indice].estado = DESCENDENTE;
            }
            break;

        case DESCENDENTE:
            if(!gpio_get_level(pulsador[indice].tecla))
            {
                pulsador[indice].estado = BAJO;
                botonLiberado(indice);
            }
            else{pulsador[indice].estado = ALTO;}
            break;

        default:
            errorPulsador(indice);
            break;
    }
}

/* Accion de el evento de tecla pulsada */
static void botonPresionado(uint8_t indice)
{
    ESP_LOGI(TAG, "Boton presionado");
    TickType_t conteoTicksActuales = xTaskGetTickCount();           // Medimos el tiempo en ticks desde que inició el scheduler
    pulsador[indice].tiempoBajo = conteoTicksActuales;              // Guardamos ese tiempo como referencia
}

/* Accion de el evento de tecla liberada */
static void botonLiberado(uint8_t indice)
{
    ESP_LOGI(TAG, "Boton liberado");
    TickType_t conteoTicksActuales = xTaskGetTickCount();           // Medimos el tiempo en ticks desde que inició el scheduler
    pulsador[indice].tiempoAlto = conteoTicksActuales;
    pulsador[indice].diferenciaTiempo  = pulsador[indice].tiempoAlto - pulsador[indice].tiempoBajo; // Da el tiempo que el pulsador estuvo en estado alto
}

void TaskPulsador(void *taskParmPtr)
{
    while(1)
    {
        for(int i = 0; i < N_PULSADOR; i++)
        {
            actualizarPulsador(i);
        }
        vTaskDelay(T_REBOTE);
    }
}