//#include "../include/led.h"
//#include "../include/trafficLights.h"
#include "led.c"

#define LED_VERDE GPIO_NUM_25
#define LED_AMARILLO GPIO_NUM_26
#define LED_ROJO GPIO_NUM_27
#define CAMBIO_ESTADO GPIO_NUM_12

#define T_VERDE		        300
#define T_AMARILLO	        100
#define T_ROJO		        500
#define T_INTERMITENTE		100

void iniciarSemaforo()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE, CAMBIO_ESTADO);
    Funcionamiento = Normal;
    estadoActual = R;
}

void actualizarSemaforo()
{
    switch(Funcionamiento)
    {
        case Normal:
        {
            switch(estadoActual) 
            {
                case R:
                {
                    prenderLed('R');
                    vTaskDelay(T_ROJO);
                    break;
                }
                case R_A:
                {
                    prenderLed('A');
                    vTaskDelay(T_AMARILLO);
                    break;
                }
                case V:
                {
                    apagarLed('R');
                    apagarLed('A');
                    prenderLed('V');
                    vTaskDelay(T_VERDE);
                    break;
                }
                case A:
                {
                    apagarLed('V');
                    prenderLed('A');
                    vTaskDelay(T_AMARILLO);
                    apagarLed('A');
                    break;
                }
                default:
                {
                    //Si cae en un estado no valido, reinicio
                    iniciarSemaforo();
                    break;
                }
            }
            
            change();
            break;
        }

        case Intermitente:
        {
            switch(estadoActualIntermitente) 
            {
                case Amarillo:
                {
                    prenderLed('A');
                    vTaskDelay(T_INTERMITENTE);
                    break;
                }
                case Apagado:
                {
                    apagarLed('A');
                    vTaskDelay(T_INTERMITENTE);
                    break;
                }
                default:
                {
                    //Si cae en un estado no valido, reinicio
                    iniciarSemaforo();
                    break;
                }
            }
            
            changeIntermitente();
            break;
        }

        default:
        {
            //Si cae en un estado no valido, reinicio
            iniciarSemaforo();
            break;
        }
    }
}