# Atividade 4 – I2C e Conversor A/D (Joystick + Display OLED)
## Descrição geral

Este programa, escrito para a **Raspberry Pi Pico W** (placa BitDogLab), lê o valor analógico do eixo X de um joystick através do conversor A/D (ADC) e exibe essa leitura de duas formas diferentes em um **display OLED SSD1306**, conectado via protocolo **I2C**:

1. Como uma **tensão formatada** (ex.: `2.35`), representando o valor em Volts lido pelo pino do joystick.
2. Como o **valor bruto da leitura do ADC** (um número inteiro de 0 a 4095), representado dígito a dígito em uma string.

## Hardware e pinagem

| Função              | Pino GPIO |
|---------------------|-----------|
| I2C SDA (display)   | GP14      |
| I2C SCL (display)   | GP15      |
| Endereço I2C display| 0x3C      |
| Eixo X do joystick  | GP26      |

O display é inicializado a 400 kHz e configurado com resistores de *pull-up* internos ativados para as linhas SDA e SCL.

## Fluxo do programa

1. **Inicialização** (`joystick_configuration`, `stdio_init_all`, `i2c_init`): configura o ADC, os pinos do joystick/botão, a interface I2C e o display OLED, que começa limpo (todos os pixels apagados).

2. **Loop principal** (`while(true)`):
   - Lê o valor bruto do ADC (`adc_read()`), que varia de **0 a 4095**.
   - Converte esse valor bruto em uma tensão (0 a 3.3V), representada em **centésimos** (`valor_centesimos = vrx_bruto * 330 / 4095`) para evitar o uso de ponto flutuante nessa etapa.
   - Separa a parte inteira e a parte decimal desse valor e monta a string `tensao` no formato `"X.XX"` usando `snprintf` (ex.: `"2.35"`).
   - Também monta manualmente a string `texto`, contendo os 4 dígitos do valor bruto da leitura do ADC (0 a 4095), extraindo cada dígito com o operador `%10` e descartando-o com `/= 10`.
   - Desenha no display:
     - O texto `"TENSAO"` seguido do valor de tensão formatado.
     - O texto `"LEITURA"` seguido do valor bruto do ADC.
     - Um retângulo de borda ao redor da tela.
   - Envia os dados para o display (`ssd1306_send_data`) e aguarda 500 ms antes da próxima leitura.

## Resumo técnico

O núcleo do problema resolvido aqui é a **conversão de um valor analógico (inteiro de 0–4095) em uma representação textual de tensão (`"X.XX"`)** para ser exibida em um display de caracteres, evitando o uso de ponto flutuante através de aritmética com valores multiplicados por 100 (centésimos).

## Vídeo de apresentação

<a href="https://drive.google.com/file/d/1hmNQe4uaDIKaz60GmmInSdRQelu3xWa7/view?usp=sharing">LINKD DA SIMULAÇÃO</a>
