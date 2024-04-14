/**
 * @file trafficLights.h
 * @author Ezequiel Combina
 * @brief Prototipos de funciones referidas al inicio y actualizacion del semaforo 
 * @version 0.1
 * @date 2024-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef TRAFFICLIGHTS_H
#define TRAFFICLIGHTS_H

typedef enum{
    R,
    R_A,
    V,
    A
} EstadoSemaforo;

typedef enum{
    Amarillo,
    Apagado
} EstadoIntermitente;

// Variables de estado (globales)
EstadoSemaforo estadoActual;

EstadoIntermitente estadoActualIntermitente;

/**
 * @brief Función para inicializar el semáforo. Configura el estado inicial del semaforo
 * 
 */
void iniciarSemaforo();

/**
 * @brief Actualiza el estado del semaforo
 * 
 */
void actualizarSemaforo();

#endif