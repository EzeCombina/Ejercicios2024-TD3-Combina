#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../include/trafficLights.h"
#include "../include/led.h"

static gpio_int_type_t ledRojo, ledAmarillo, ledVerde, Cambio_S; //estas variables solamente son accesibles en este archivo

void configurarLed(gpio_int_type_t ledR, gpio_int_type_t ledA, gpio_int_type_t ledV, gpio_int_type_t Cambio)
{
    ledRojo = ledR;
    ledAmarillo = ledA;
    ledVerde = ledV;
    Cambio_S = Cambio;
    esp_rom_gpio_pad_select_gpio(ledRojo);
    gpio_set_direction(ledRojo, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(ledAmarillo);
    gpio_set_direction(ledAmarillo, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(ledVerde);
    gpio_set_direction(ledVerde, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(Cambio_S);
    gpio_set_direction(Cambio_S, GPIO_MODE_INPUT);
}

/*

    CONEXION EXTERIOR:

    3.3 V -> Pulsador -> Pin12

*/

void prenderLed(char led)
{
    switch(led)
    {
        case 'R':
        {
            gpio_set_level(ledRojo, 1);
            break;
        }
        case 'A':
        {
            gpio_set_level(ledAmarillo, 1);
            break;
        }
        case 'V':
        {
            gpio_set_level(ledVerde, 1);
            break;
        }
        default:
            break;
    }
}

void apagarLed(char led)
{
    switch(led)
    {
        case 'R':
        {
            gpio_set_level(ledRojo, 0);
            break;
        }
        case 'A':
        {
            gpio_set_level(ledAmarillo, 0);
            break;
        }
        case 'V':
        {
            gpio_set_level(ledVerde, 0);
            break;
        }
        default:
            break;
    }
}

bool check()
{
    if(gpio_get_level(Cambio_S))
    {
        return false;
    }else{return true;}
}

void change()
{
    if(estadoActual < 3)
    {
        estadoActual++;
    }else{estadoActual = 0;}
}

void changeIntermitente()
{
    if(estadoActualIntermitente == 0)
    {
        estadoActualIntermitente = 1;
    }else{estadoActualIntermitente = 0;}
}

void funcionInt()
{
    if(Funcionamiento == Normal)
    {
        Funcionamiento = Intermitente; 
        estadoActualIntermitente = Amarillo;
    }
    else
    {
        Funcionamiento = Normal;
        estadoActual = R;
    }

    apagarTodo();

    vTaskDelay(50);
}

void apagarTodo()
{
    gpio_set_level(ledRojo, 0);
    gpio_set_level(ledAmarillo, 0);
    gpio_set_level(ledVerde, 0);
}