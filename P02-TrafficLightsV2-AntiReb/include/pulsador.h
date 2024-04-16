#ifndef PULSADOR_H
#define PULSADOR_H

#include "driver/gpio.h"

typedef enum{
    ALTO,
    BAJO,
    DESCENDENTE,
    ASCENDENTE
}EstadoBoton;

/**
 * @brief Funcion que devuelve "error" y pone el pulsador en BAJO 
 * 
 */
void errorBoton();

/**
 * @brief Funcion para inicializar el pulsador 
 * 
 */
void inicializarBoton();

/**
 * @brief Funcion para revisar si el boton esta presionado
 * 
 */
void actualizarBoton();

/**
 * @brief Funicon para realizar el cambio de estado 
 * 
 */
void botonPresionado();

/**
 * @brief Funcion para apagar todos los leds
 * 
 */
void apagarTodo();

#endif