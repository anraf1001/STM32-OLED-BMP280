#include "GFX_BW.hpp"

#include <cmath>

namespace gfx {
//
// Line drawing
//
void writeLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color) {
    auto steep = std::abs(y_end - y_start) > std::abs(x_end - x_start);

    if (steep) {
        std::swap(x_start, y_start);
        std::swap(x_end, y_end);
    }

    if (x_start > x_end) {
        std::swap(x_start, x_end);
        std::swap(y_start, y_end);
    }

    const int16_t dx = x_end - x_start;
    const int16_t dy = std::abs(y_end - y_start);

    int16_t err = dx / 2;
    int16_t y_step;

    if (y_start < y_end) {
        y_step = 1;
    } else {
        y_step = -1;
    }

    for (; x_start <= x_end; x_start++) {
        if (steep) {
            oled.drawPixel(y_start, x_start, color);
        } else {
            oled.drawPixel(x_start, y_start, color);
        }

        err -= dy;

        if (err < 0) {
            y_start += y_step;
            err += dx;
        }
    }
}

void drawFastVLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t h, PixelColor color) {
    writeLine(oled, x_start, y_start, x_start, y_start + h - 1, color);
}

void drawFastHLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t w, PixelColor color) {
    writeLine(oled, x_start, y_start, x_start + w - 1, y_start, color);
}

void drawLine(OLED_SSD1306<I2C_Handler>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color) {
    if (x_start == x_end) {
        if (y_start > y_end) {
            std::swap(y_start, y_end);
        }
        drawFastVLine(oled, x_start, y_start, y_end - y_start + 1, color);
    } else if (y_start == y_end) {
        if (x_start > x_end) {
            std::swap(x_start, x_end);
        }
        drawFastVLine(oled, x_start, y_start, x_end - x_start + 1, color);
    } else {
        writeLine(oled, x_start, y_start, x_end, y_end, color);
    }
}

//
// Rectangle drawing
//
void drawRectangle(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, uint16_t width, uint16_t height, PixelColor color) {
    drawFastHLine(oled, x, y, width, color);
    drawFastHLine(oled, x, y + height - 1, width, color);
    drawFastVLine(oled, x, y, height, color);
    drawFastVLine(oled, x + width - 1, y, height, color);
}

void drawFillRectangle(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, uint16_t width, uint16_t height, PixelColor color) {
    for (int32_t i = x; i < x + width; i++) {
        drawFastVLine(oled, i, y, height, color);
    }
}
}  // namespace gfx
