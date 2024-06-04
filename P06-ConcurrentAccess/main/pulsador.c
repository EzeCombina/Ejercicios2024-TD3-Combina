/*==================[ Inclusiones ]============================================*/
#include "../include/pulsador.h"
#include "esp_log.h"

/*==================[ Definiciones ]===================================*/
#define T_REBOTE_MS         40
#define T_REBOTE            pdMS_TO_TICKS(T_REBOTE_MS)
#define VALOR_MAX           900
#define VALOR_MIN           100
#define VALOR_SUMA          100
#define VALOR_RESTA         100

/*==================[Prototipos de funciones]======================*/
static void errorPulsador(uint8_t indice);
static void botonPresionado(uint8_t indice);
static void botonLiberado(uint8_t indice);

int ObtenerCont();
void DecrementarCont();

void TaskPulsador(void *taskParmPtr);

/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[N_PULSADOR] = {GPIO_NUM_12, GPIO_NUM_13};

pulsadorInfo pulsador[N_PULSADOR];

static const char *TAG = "PULSADOR";

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;    // Inicializa el spinlock desbloqueado

static uint16_t contador = 100;                     // Variable para el encendido de los leds

/*==================[Implementaciones]=================================*/

int ObtenerCont()
{
    portENTER_CRITICAL(&mux);       // Desactivamos las interrupciones de ambos procesadores por la variable mux
    uint16_t flag = contador;
    portEXIT_CRITICAL(&mux);
    return flag;
}

void DecrementarCont()
{
    portENTER_CRITICAL(&mux);
    if(contador > VALOR_MIN)
    {
        contador -= VALOR_RESTA;
    }
    else
    {
        contador = VALOR_MIN;
    } 
    portEXIT_CRITICAL(&mux);
}

void inicializarPulsador(void)
{
    for(int i = 0; i < N_PULSADOR; i++)
    {
        pulsador[i].tecla             = pulsadorPines[i];
        pulsador[i].estado            = BAJO;                       // Estado inicial
        //pulsador[i].semaforo          = xSemaphoreCreateMutex();

        esp_rom_gpio_pad_select_gpio(pulsador[i].tecla);
        gpio_set_direction(pulsador[i].tecla , GPIO_MODE_INPUT);
        gpio_set_pull_mode(pulsador[i].tecla, GPIO_PULLDOWN_ONLY);  // Habilita resistencia de PULLDOWN interna
    }
 
    // Crear tareas en freeRTOS
    BaseType_t err = xTaskCreatePinnedToCore(
    	TaskPulsador,                     	    // Funcion de la tarea a ejecutar
        "TaskPulsador",   	                    // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		    // Cantidad de stack de la tarea
        NULL,                          	        // Parametros de tarea
        tskIDLE_PRIORITY+1,         	        // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		    // Puntero a la tarea creada en el sistema
        1                                       // Numero de procesador (Procesador B)
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
}

/* Accion de el evento de tecla liberada */
static void botonLiberado(uint8_t indice)
{
    ESP_LOGI(TAG, "Boton liberado");

    if(indice == PULSADOR1)
    {
        //portENTER_CRITICAL(&mux);
        if(ObtenerCont() < VALOR_MAX)
        {
            portENTER_CRITICAL(&mux);
            contador += VALOR_SUMA;
            portEXIT_CRITICAL(&mux);
        }
        else
        {
            portENTER_CRITICAL(&mux);
            contador = VALOR_MAX;
            portEXIT_CRITICAL(&mux);
        }  
        //portEXIT_CRITICAL(&mux);  
    }
    else if(indice == PULSADOR2)
    {
        //portENTER_CRITICAL(&mux);
        if(ObtenerCont() > VALOR_MIN)
        {
            portENTER_CRITICAL(&mux);
            contador -= VALOR_RESTA;
            portEXIT_CRITICAL(&mux);
        }
        else
        {
            portENTER_CRITICAL(&mux);
            contador = VALOR_MIN;
            portEXIT_CRITICAL(&mux);
        } 
        //portEXIT_CRITICAL(&mux); 
    }
    else{ESP_LOGI(TAG, "Error en el contador.");}
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