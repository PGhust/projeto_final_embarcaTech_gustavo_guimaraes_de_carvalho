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
 

//Agora é necessário tomar posse de um máquina PIO --> Se nenhuma máquina estiver livre o programa dá erro e para.
//Tomada de posse de uma máquina PIO 
sm = pio_claim_unsed_sm(n_pio, false);
if (sm < 0){
    np_pio = pio1;
    sm = pio_claim_unsed_sm(np_pio, true); //Caso em que nenhuma máquina está livre, pare! 
}

//Inicializa o programa na máquina PIO obtida.
ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

//Limpamos o buffer de pixels dos LED's
for (uint i = 0; i < LED_COUNT; ++i){
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
}
}

//Criação da função npSetLED --> Muda a cor do LED do índice selecionado (alterando o valor dos buffers)

//Vamos atribuir uma cor RGB a um LED 
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b){

    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;

}

//Void de limpeza do buffer de pixels 
void npClear(){
    for(uint i = 0; i < LED_COUNT; i++)
    npSetLED(i, 0, 0, 0);
}

//Função de escrita de dados do buffer para os LED's --> envia os dados do buffer p/ o LED utilizando o sistema PIO escrevendo um byte por vez através na ordem GRB 
void npWrite(){
    //Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO
    for(uint i = 0; i < LED_COUNT; i++){
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

//Inicializando o programa principal 
//Main ira demarcar o que será feito na execução do prograa, importante se atentar que tal qual o exemplo inicialiamos os recursos da matriz de LEDs pela funçõa nplinit e limpamos o buffer com npClear

int main(){
    //inicializa I/O
    stdio_init_all();

    //Inicializa matriz de LED's NeoPixel
    npInit(LED_PIN);
    npClear();

    //Aqui podemos alterar a imagem desejada a ser demonstrada na matriz!
    //Como se estivesse desenhando nos LED's
    npSetLED(0, 255, 0, 0); //LED índice 0 para vermelho
    npSetLED(12, 0, 255, 0); // LED 12 (central) para verde

    //Acabamos de modificar os buffers certo? Escrevemos esses dados nos LED's, agora não fazemos mais nada!
    npWrite(); //Escreve os dados nos LED's

    //Entra em loop infinito (não faz mais nada)~
    while (true){
        sleep_ms(1000);
    }
}