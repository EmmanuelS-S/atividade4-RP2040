
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
// Definições dos pinos para o joystick e botão
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick (Canal ADC0).
#define SW_PIN 22     // Define o pino GP22 para o botão do joystick (entrada digital).

void joystick_configuration();

int main()
{
  joystick_configuration();
  
  stdio_init_all();
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  
  bool cor = true;
  char texto[5];
  char tensao[6];
  texto[4] = '\0';// serve como uma "placa de pare" para o compilador e para o display



  uint16_t vrx_value;
  int valor_tensao;
  adc_select_input(1);

  while (true)
  {
    vrx_value = adc_read();

    valor_tensao = (float)(vrx_value*3.3/4095);
    printf("tensao: %f\n",valor_tensao);

    uint16_t vrx_bruto = adc_read(); // uma leitura só, já que o canal foi selecionado antes do loop

    uint16_t valor_centesimos = (uint16_t)(((uint32_t)vrx_bruto * 330) / 4095);
    int parte_inteira = valor_centesimos / 100;
    int parte_decimal = valor_centesimos % 100;

    snprintf(tensao, sizeof(tensao), "%d.%02d", parte_inteira, parte_decimal);


    for(int i = 4 - 1; i >= 0; i--){
      texto[i] = (vrx_value % 10) + '0'; // 47 % 10 = 7; 7 + 48 = 55 pela tabela ASCII = 7
      vrx_value /= 10;
    }
    
    ssd1306_draw_string(&ssd, "TENSAO", 10, 15);
    ssd1306_draw_string(&ssd, tensao,70, 15);
    ssd1306_draw_string(&ssd, "LEITURA", 10, 40);
    ssd1306_draw_string(&ssd, texto,80, 40);
    ssd1306_rect(&ssd, 5, 5, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_send_data(&ssd);
    
    sleep_ms(500);

  }
}



//configura tanto o adc do joystick quanto o botão
void joystick_configuration(){
  adc_init();
  adc_gpio_init(VRX_PIN);
  gpio_init(SW_PIN);
  gpio_set_dir(SW_PIN, GPIO_IN);
  gpio_pull_up(SW_PIN);

}

