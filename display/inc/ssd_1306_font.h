#ifndef SSD1306_FONT_H
#define SSD1306_FONT_H

/*********************** includes *******************************/
#include "ssd_1306.h"

/*********************** definiçoes *******************************/
#ifdef SSD1306_INCLUDE_FONT_6x8
extern const ssd_1306_font Font_6x8;
#endif
#ifdef SSD1306_INCLUDE_FONT_7x10
extern const ssd_1306_font Font_7x10;
#endif
#ifdef SSD1306_INCLUDE_FONT_11x18
extern const ssd_1306_font Font_11x18;
#endif
#ifdef SSD1306_INCLUDE_FONT_16x26
extern const ssd_1306_font Font_16x26;
#endif
#ifdef SSD1306_INCLUDE_FONT_16x24
extern const ssd_1306_font Font_16x24;
#endif
#ifdef SSD1306_INCLUDE_FONT_16x15
/** Generated Roboto Thin 15 
 * @copyright Google https://github.com/googlefonts/roboto
 * @license This font is licensed under the Apache License, Version 2.0.
*/
extern const ssd_1306_font Font_16x15;
#endif

#endif // SSD1306_FONT_H