# 📌 Utilização da Plataforma BitDogLab

A **BitDogLab**, uma iniciativa do Projeto Escola 4.0 da Unicamp, é uma ferramenta educacional dedicada à eletrônica e computação. Baseada na **Raspberry Pi Pico H ou W**, permite aos usuários explorar, montar e programar utilizando componentes montados na sua placa e também externos conectados de forma organizada e segura. Selecionados meticulosamente, os componentes promovem um aprendizado "mão na massa", incentivando os usuários a aprimorar habilidades de programação e eletrônica de maneira sinérgica e progressiva.

Um diferencial da **BitDogLab** é que seu projeto é totalmente aberto, permitindo que seja livremente copiada, fabricada, montada e melhorada pelos usuários.

Mais informações em: [BitDogLab GitHub](https://github.com/Fruett/BitDogLab).

## 📌 Conexões e Configurações de Hardware

Na placa da **BitDogLab**, as conexões da **Raspberry Pi Pico** com outros componentes estão realizadas da seguinte forma:

- **LED RGB (Cátodo comum)**:
  - Vermelho no **GPIO 13** através de um resistor de **220Ω**.
  - Verde no **GPIO 11** através de um resistor de **220Ω**.
  - Azul no **GPIO 12** através de um resistor de **150Ω**.

- **Botões**:
  - **Botão A**: **GPIO 5**, com resistor de pull-up interno.
  - **Botão B**: **GPIO 6**, com resistor de pull-up interno.
  - **Botão RESET**: Conectado ao **RUN (pino 30)** da Raspberry Pi Pico.

- **Buzzers**:
  - **Buzzer A**: GPIO **21** (controlado por um transistor).
  - **Buzzer B**: GPIO **10**.

- **Matriz de LEDs RGB WS2812B (Neopixel)**:
  - Conectada ao **GPIO 7**.
  - Possui um conector para expansão de mais LEDs coloridos via jumper "Ext. RGB Neopixel" no verso da placa.

- **Joystick Analógico KY-023**:
  - **VRy**: **GPIO 26**.
  - **VRx**: **GPIO 27**.
  - **Botão SW**: **GPIO 22**, com resistor de pull-up interno.

- **Display OLED 128x64 (I2C)**:
  - **SDA**: **GPIO 14**.
  - **SCL**: **GPIO 15**.
  - Utiliza **I2C1** por padrão.
  - Pode utilizar **SoftI2C** para comunicação mais robusta.
  
 

---

# 📜 Índice
- [Introdução](#-introdução)
- [Motivo](#-motivo)
- [Principais Características](#-principais-caracteristicas)
- [Instalação](#-instalação)
- [Quick Start](#-quick-start)
- [Exemplos](#-exemplos)
- [Documentação](#-documentação)
- [Contribuintes](#-contribuintes)

## 🌟 Introdução

Os periféricos são componentes cruciais em sistemas embarcados, permitindo a interação entre o microcontrolador e o mundo externo. Eles incluem dispositivos como sensores, atuadores e displays, entre outros.

Esses componentes são responsáveis por coletar dados, processá-los e executar ações baseadas nas informações recebidas, tornando-os essenciais para o funcionamento eficaz e eficiente de sistemas embarcados.

### Principais pontos dos periféricos:
- Coleta de dados
- Interface com o usuário
- Comunicação
- Controle
- Armazenamento
- Precisão e eficiência

### 🛠️ OLED SSD1306

O display **OLED SSD1306** é amplamente utilizado em projetos de sistemas embarcados devido à sua boa resolução e baixo consumo de energia.

A biblioteca **ssd_1306** foi construída para facilitar o uso do **OLED SSD1306**, promovendo conhecimento aberto e facilitando a sua integração com a **Raspberry Pi Pico W**.

### 🛠️ Joystick

Os **joysticks** são dispositivos de entrada que permitem controlar o movimento em várias direções nos eixos **X, Y e Z (rotação)**. São amplamente utilizados em videogames, simuladores de voo e outros aplicativos onde é necessário um controle preciso e intuitivo.

A **Raspberry Pi Pico** suporta as linguagens **C** e **MicroPython**.
O Joystick Analógico KY-023 permite o controle de movimentos nos eixos X e Y, além de possuir um botão de clique integrado.

Conexões:

VRx → GPIO 27 (Leitura do eixo X via ADC)

VRy → GPIO 26 (Leitura do eixo Y via ADC)

SW → GPIO 22 (Leitura do botão com pull-up interno)

#### 🔧 Explicação da Implementação

**Seleção do canal ADC para o eixo X (VRX)**
```c
adc_select_input(ADC_CHANNEL_0);
```
- O joystick possui dois potenciômetros internos, um para o eixo X (VRX) e outro para o eixo Y (VRY). Esses potenciômetros estão conectados aos pinos analógicos do Raspberry Pi Pico.
- `ADC_CHANNEL_0` corresponde ao pino **JOYSTICK_VRX** (pino 26).
- A função `adc_select_input()` configura o ADC para ler esse canal.

**Leitura do valor do ADC**
```c
*vrx_value = adc_read();
```
- `adc_read()` retorna o valor digitalizado do sinal analógico no pino correspondente (0 a 4095, ADC de 12 bits).

**Seleção do canal ADC para o eixo Y (VRY)**
```c
adc_select_input(ADC_CHANNEL_1);
```
- `ADC_CHANNEL_1` corresponde ao pino **JOYSTICK_VRY** (pino 27).
- O ADC agora será configurado para ler o eixo Y.

**Novo delay e leitura do ADC**
```c
sleep_us(2);
*vry_value = adc_read();
```
- Pequeno delay de **2 microssegundos**.
- O valor lido do eixo Y é armazenado na variável `vry_value`.


adc_select_input(ADC_CHANNEL_0);
O joystick possui dois potenciômetros internos, um para o eixo X (VRX) e outro para o eixo Y (VRY). Esses potenciômetros estão conectados aos pinos analógicos do Raspberry Pi Pico.

ADC_CHANNEL_0 corresponde ao pino JOYSTICK_VRX (pino 26 no código).
A função adc_select_input() configura o ADC para ler esse canal.

## 💡 Motivo

- Facilitar a integração dos periféricos com o microcontrolador.

## ✨ Principais Características - OLED

### Inicialização
- `ssd1306_Init()`: Cria a lista de comandos para inicialização do display.

### Controle do Display
- `render_on_display()`: Atualiza uma parte do display com uma área de renderização.
- `ssd1306_set_pixel()`: Determina o pixel a ser aceso no display.

### Desenho de Formas
- `ssd1306_draw_line()`: Desenha uma linha utilizando o algoritmo de Bresenham.
- `ssd1306_draw_bitmap()`: Desenha um bitmap no display.

### Envio e Manipulação de Textos
- `ssd1306_draw_char()`: Desenha um caractere no display.
- `ssd1306_draw_string()`: Desenha uma string, chamando a função de desenhar caractere várias vezes.

## 🔧 Instalação

Para instalar e utilizar a biblioteca, siga os passos:

- **Clonar o repositório:**

```bash
$ git clone https://github.com/Pwsousa/Sistemas_Embarcados.git
```

- **Adicionar os arquivos no CMakeLists.txt:**

```bash
add_executable(
    Sistemas_Embarcados
    display/ssd1306_i2c.c
    display/ssd1306.c
    Sistemas_Embarcados.c
)
```

- **Verificar `target_link_libraries()`**:

```bash
target_link_libraries(Sistemas_Embarcados
    hardware_gpio
    hardware_adc
    hardware_pwm
    hardware_i2c
    pico_stdlib
    hardware_timer
)
```
## 🚀 Quick Start

## 📋 Exemplos

## 📚 Documentação

Para mais informações, acesse:
- [Datasheet OLED1306](https://www.digikey.com/htmldatasheets/production/2047793/0/0/1/ssd1306.html?msockid=0fc5c8f0897d6ddd09eedc37882a6c9c).
- [Joystick Module](https://components101.com/modules/joystick-module).

## 👥 Contribuintes

