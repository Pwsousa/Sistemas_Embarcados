- [Introdução](#-introdução)
- [Motivo](#-motivo)
- [Principais caracteristicas](#-caracteristicas--capacidades)
- [Instalação](#-instalacao)
- [Quick Start](#-quick-start)
- [Exemplos](#-exemplos)
- [Documentacao](#-documentacao)
- [Contribuintes](#-contribuintes)


## 🌟 Introdução

Os periféricos são componentes cruciais em sistemas embarcados, permitindo a interação entre o microcontrolador e o mundo externo. Eles incluem dispositivos como sensores, atuadores, displays entre outros. 

Esses componentes são responsáveis por coletar dados, processá-los e executar ações baseadas nas informações recebidas, tornando-os essenciais para o funcionamento eficaz e eficiente de sistemas embarcados.

- **Principais pontos dos perifericos**:
    - Coleta de dados
    - Interface com o usuário
    - Comunicação
    - Controle
    - Armazenamento
    - Precisão e Eficiência

### 🛠️ OLED SSD1306

O display **OLED SSD1306** é amplamente utilizado em projetos de sistemas embarcardos por vários fatores como boa resolução e baixo conssumo de energia.

A biblioteca **ssd_1306** foi construida com o intuito de facilitar o uso do dispositivo *OLED SSD1306* contribuindo com a abertura de conhecimento e facilitando a sua integração com a *Raspberry Pi Pico W*. 

### 🛠️ Joystick 

Os **joysticks** é um dispositivos de entrada que permitem controlar o movimento em várias direções nos eixos x, y e z(rotação). Eles são amplamente utilizados em videogames, simuladores de voo e outros aplicativos onde é necessário um controle preciso e intuitivo, temos alguns tipos de **joystikcs** que pode ser utilizados segundo o criterio do projeto.

A *Raspberry Pi Pico* suporta a linguagem *C* e a *MicroPython*, o presente código foi escrito em C utilizando o ambiente de desenvolvimento *Visual Studio Code* juntamente com a extensão *Raspberry Pi Pico* presente na ferramenta de desenvolvimento.

## 💡 Motivo

- *Facilitar a integração dos perifericos com o microcontrolador*
- *

## ✨ Principais caracteristicas - OLED

### Inicialização
-`ssd1306_Init()` : Cria a lista de comandos (com base nos endereços definidos em ssd1306_i2c.h) para a inicialização do display.

### Controle do display

-`render_on_display()`: Atualiza uma parte do display com uma área de renderização
-`ssd1306_set_pixel()`: Determina o pixel a ser aceso (no display) de acordo com a coordenada fornecida
-` 

### Desenho de formas 

-`ssd1306_draw_line()`: Com o algoritmo de Bresenham básico desenha uma linha 
-`ssd1306_draw_bitmap()`: Desenha o bitmap (a ser fornecido em display_oled.c) no display.

### Envio e manipulação de textos

-`ssd1306_draw_char()`: Desenha um único caractere no display.
-`ssd1306_draw_string()`:Desenha uma string, chamando a função de desenhar caractere várias vezes.

### Manipulação do buffer

-`calculate_render_area_buffer_length()`:Calcular quanto do buffer será destinado à área de renderização.
-`

### Funções de baixo nível
-`ssd1306_send_command()`: Processo de escrita do i2c espera um byte de controle, seguido por dados.

-`ssd1306_config()`: Função de configuração do display para o caso do bitmap

-`ssd1306_init_bm()`: Inicializa o display para o caso de exibição de bitmap

-`ssd1306_send_command_list()`: Envia uma lista de comandos ao hardware.

-`ssd1306_send_buffer()`: Copia buffer de referência num novo buffer, a fim de adicionar o byte de controle desde o início.

-`ssd1306_scroll()`: Cria a lista de comandos para configurar o scrolling.

-`ssd1306_send_data()`: Envia os dados ao display.

## 🔧 Instalação

## 🚀 Quick Start

## 📋 Exemplos

## 📚 Documentação

## 👥 Contribuintes

