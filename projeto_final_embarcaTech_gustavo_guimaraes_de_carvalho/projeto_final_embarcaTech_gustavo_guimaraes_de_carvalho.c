//Inclusão das bibliotecas 
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h" //Por qual motivo isso fica apitando erro?
#include "ws2818b.pio.h"

//Definição do número de LED'S e pino para a matriz de LED's

#define LED_COUNT 25
#define LED_PIN 7 

//Definição do que é um pixel GRB par ao código entender quando estivermos operando ele 

struct pixel_t{    
    uint8_t G, R, B; // 3 valores de 8 bits irão compor um pixel
};

typedef struct pixel_t pixel_t; //Mudando o nome da estrutura apenas para pixel_t
typedef pixel_t npLED_t; //Mudando o nome da estrutura de "struct pixel_t" para "npLED_t", isso deixa o código claro e limpo 

//Declarando os buffers de LED para armazenar dados de cor do LED
npLED_t leds[LED_COUNT]; //Buffers que compõe a matriz

//Declaração das variáveis da máquina PIO
    //Cria variáveis para acompanhar o status da máquina PIO utilizada "ws2818b.pio"

PIO np_pio; 
uint sm; 

//Inicialização da matriz de LED's NeoPixel

/**
 * Inicialize a máquina PIO para controle da matriz de LED's
 */

 void npInit(uint pin){
    //Criando programa PIO --> a função cria o PIO prog configurado no ws2818b.pio e registra a unidade de PIO na variável np_pio
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_io = pio0;
 }

//Agora é necessário tomar posse de um máquina PIO --> Se nenhuma máquina estiver livre o programa dá erro e para.
//Tomada de posse de uma máquina PIO 
sm = pio_claim_unsed_sm(n_pio, false);
if (sm < 0){
    np_pio = pio1;
    sm = pio_claim_unsed_sm(np_pio, true); //Caso em que nenhuma máquina está livre, pare! 
}

