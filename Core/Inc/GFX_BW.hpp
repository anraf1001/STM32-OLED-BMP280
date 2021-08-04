#pragma once

#include <cstdint>

#include "I2C_Handler.hpp"
#include "OLED_SSD1306.hpp"
#include "PixelColor.hpp"

namespace gfx {
void drawChar(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, char ch, PixelColor color);

void drawLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color);

void drawRectangle(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, uint16_t width, uint16_t height, PixelColor color);
void drawFillRectangle(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, uint16_t width, uint16_t height, PixelColor color);

void drawCircle(OLED_SSD1306<I2C_Handler>& oled, int16_t x0, int16_t y0, uint16_t radius, PixelColor color);
}  // namespace gfx
