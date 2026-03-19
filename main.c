#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
//ANDREY
// Pinos
#define semC_RED  23
#define semC_YLW  22
#define semC_GRN  19

//VARIÁVEL DE ESTADO 

int semC_estado_atual = 0;
int semC_time_template = 3000;
int semC_timeL = 1;
//0=semC_RED
//1 semC_GRN
//2=semC_YLW
int TRF_3_BOTAO = 1;

void inicializar_pinos() {
    gpio_reset_pin(semC_RED);
    gpio_set_direction(semC_RED, GPIO_MODE_OUTPUT);

    gpio_reset_pin(semC_YLW);
    gpio_set_direction(semC_YLW, GPIO_MODE_OUTPUT);

    gpio_reset_pin (semC_GRN);
    gpio_set_direction (semC_GRN, GPIO_MODE_OUTPUT);
}

void semC_time_latch(int semC_var){
  for (semC_timeL = 1; semC_timeL <= 3; semC_timeL++)
    vTaskDelay((semC_time_template / semC_var) /portTICK_PERIOD_MS);
}

void semaforo_carros() {
    printf("Semáforo\n");
    inicializar_pinos();

    while (true) {
        
        switch(semC_estado_atual) {
            case 0: // semC_RED
                printf("Pare!\n");
                gpio_set_level(semC_RED, 1);
                gpio_set_level(semC_YLW, 0);
                gpio_set_level (semC_GRN, 0);
                if (TRF_3_BOTAO == 1 && semC_estado_atual == 0)
                  semC_time_latch(2);
                  else if (TRF_3_BOTAO == 0 && semC_estado_atual == 0)
                    semC_time_latch(1);
                semC_estado_atual = 1; // Próximo passo: semC_GRN
                break;

            case 1: // semC_GRN
                printf("Siga!\n");
                gpio_set_level(semC_RED, 0);
                gpio_set_level(semC_YLW, 0);
                gpio_set_level (semC_GRN, 1);
                semC_time_latch(1);
                semC_estado_atual = 2; // Próximo passo: semC_YLW
                break;

            case 2: // semC_YLW
                printf("Atenção!\n");
                gpio_set_level(semC_RED, 0);
                gpio_set_level(semC_YLW, 1);
                gpio_set_level (semC_GRN, 0);
                semC_time_latch(3);
                semC_estado_atual = 0; // Próximo passo: semC_RED
                break;
        }
    }
}
