#pragma once

#include <cstdint>

#include "I2C_Handler.hpp"
#include "OLED_SSD1306.hpp"
#include "PixelColor.hpp"

namespace gfx {
void drawLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color);

void drawRectangle(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, uint16_t width, uint16_t height, PixelColor color);
}  // namespace gfx
