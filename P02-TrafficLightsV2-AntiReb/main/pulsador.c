#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "../include/pulsador.h"
#include "../include/trafficLights.h"
#include "../include/led.h"

#define PULSADOR GPIO_NUM_12

static const char *TAG = "MAIN";

#define T_ESPERA 4 //cantidad de veces que va a ingresar antes de cambiar de estado
EstadoBoton estadoActualBoton;

void inicializarBoton()
{
    estadoActualBoton = BAJO;  // Se configura estado inicial
    gpio_set_direction(PULSADOR, GPIO_MODE_INPUT); //Configura el pin como entrada digital
    gpio_set_pull_mode(PULSADOR, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna
}

void errorBoton()
{
    printf("Error con el pulsador");
    estadoActualBoton = BAJO;
}

//Actualización de la MEF
void actualizarBoton()
{

    /*
    
        Al definir estas variables de manera static, se inicializan con el valor dado y luego
        toman el valor con el que se las guardo la ultima vez en la memoria, con esto se ahorra el hecho 
        de crear mas variables globales.

    */

   static uint8_t contAscendente = 0; 
   static uint8_t contDescendente = 0;

   switch(estadoActualBoton){

        case BAJO: 
            if(gpio_get_level(PULSADOR))
            {
                estadoActualBoton = ASCENDENTE;
            }
            break;

        case ASCENDENTE:      
            if(contAscendente >= T_ESPERA)
            {
                if(gpio_get_level(PULSADOR))
                {
                    estadoActualBoton = ALTO;
                    botonPresionado();
                }else{estadoActualBoton = BAJO;}
                contAscendente = 0;
            }
            contAscendente++;
            break;

        case ALTO:
            if(!gpio_get_level(PULSADOR))
            {
                estadoActualBoton = DESCENDENTE;
            }
            break;

        case DESCENDENTE:      
            if(contDescendente >= T_ESPERA){
                if(!gpio_get_level(PULSADOR))
                {
                    estadoActualBoton = BAJO;
                }else{estadoActualBoton = ALTO;}
                contDescendente = 0;
            }
            contDescendente++;
            break;

        default:
            errorBoton();
            break;
   }
}


void botonPresionado()
{
    ESP_LOGI(TAG, "Pulsador");
    if(Funcionamiento == Normal)
    {
        Funcionamiento = Intermitente;
    }
    else
    {
        Funcionamiento = Normal;
        estadoActualSemaforo = R;
    }
    apagarTodo();
}