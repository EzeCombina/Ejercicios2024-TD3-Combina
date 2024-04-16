#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.c"
//#include "../include/led.h"
//#include "../include/trafficLights.h"

#define LED_VERDE GPIO_NUM_25
#define LED_AMARILLO GPIO_NUM_26
#define LED_ROJO GPIO_NUM_27

// Como la secuencia se va a realizar cada 10 ms, el tiempo de cada led va a ser la cantidad de secuencias de c/u * 10

#define T_VERDE		        250
#define T_AMARILLO	        70
#define T_ROJO		        400
#define T_INTERMITENTE		70

void iniciarSemaforo()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
    Funcionamiento = Normal;
    estadoActualSemaforo = R;
}

void actualizarSemaforo()
{

    static int contSemaforo = 0;

    switch(Funcionamiento)
    {
        case Normal:
        {
            switch(estadoActualSemaforo) 
            {
                case R:
                {
                    prenderLed('R');
                    //vTaskDelay(T_ROJO / portTICK_PERIOD_MS);
                    if(contSemaforo > T_ROJO)
                    {
                        change();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }

                case R_A:
                {
                    prenderLed('A');
                    //vTaskDelay(T_AMARILLO / portTICK_PERIOD_MS);
                    if(contSemaforo > T_AMARILLO)
                    {
                        change();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }

                case V:
                {
                    apagarLed('R');
                    apagarLed('A');
                    prenderLed('V');
                    //vTaskDelay(T_VERDE / portTICK_PERIOD_MS);
                    if(contSemaforo > T_VERDE)
                    {
                        change();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }

                case A:
                {
                    apagarLed('V');
                    prenderLed('A');
                    //vTaskDelay(T_AMARILLO / portTICK_PERIOD_MS);
                    if(contSemaforo > T_AMARILLO)
                    {
                        apagarLed('A');
                        change();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }

                default:
                {
                    //Si cae en un estado no valido, reinicio
                    iniciarSemaforo();
                    break;
                }
            }
            
            break;
        }

        case Intermitente:
        {
            switch(estadoActualIntermitente) 
            {
                case Amarillo:
                {
                    prenderLed('A');
                    //vTaskDelay(T_INTERMITENTE/portTICK_PERIOD_MS);
                    if(contSemaforo > T_AMARILLO)
                    {
                        changeIntermitente();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }
                case Apagado:
                {
                    apagarLed('A');
                    //vTaskDelay(T_INTERMITENTE/portTICK_PERIOD_MS);
                    if(contSemaforo > T_AMARILLO)
                    {
                        changeIntermitente();
                        contSemaforo = 0;
                    }
                    contSemaforo++;
                    break;
                }
                default:
                {
                    //Si cae en un estado no valido, reinicio
                    iniciarSemaforo();
                    break;
                }
            }
        
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