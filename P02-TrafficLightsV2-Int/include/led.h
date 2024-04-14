/**
 * @file led.h
 * @author Ezequiel Combina
 * @brief Prototipos de funciones referidas al encendido y apagado de los leds
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef LED_H
#define LED_H

#include "driver/gpio.h"

typedef enum{
    Normal,
    Intermitente
} Estado;

Estado Funcionamiento;

//volatile int c = 0;

/**
 * @brief 
 * 
 * @param ledR Pin donde está conectado el led rojo
 * @param ledA Pin donde está conectado el led amarillo
 * @param ledV Pin donde está conectado el led verde
 * @param Cambio Pin cambio de estado
 */
void configurarLed(gpio_int_type_t ledR, gpio_int_type_t ledA, gpio_int_type_t ledV, gpio_int_type_t Cambio); //especifica en que pines conectar cada led: ROJO, AMARILLO, VERDE

/**
 * @brief Función para encender un led
 * 
 * @param led Led a encender R para rojo, V para verde A para amarillo
 */
void prenderLed(char led);

/**
 * @brief Función para apagar un led
 * 
 * @param led Led a apagar R para rojo, V para verde A para amarillo
 */
void apagarLed(char led);

/**
 * @brief Funcion de Modificacion de estado
 * 
 */
void change();

/**
 * @brief Funcion de Modificacion de estado (Intermitente)
 * 
 */
void changeIntermitente();

/**
 * @brief Funcion llamada al pulsar el boton y realizar el Cambio de Estado
 * 
 */
void Interrupcion(void *args);

/**
 * @brief Funcion para apagar todos los leds
 * 
 */
void apagarTodo();

#endif