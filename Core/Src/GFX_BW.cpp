#include "GFX_BW.hpp"

#include <cmath>
#include <string>

#include "font.hpp"

namespace gfx {
//
// String drawing
//
constexpr char lastPrintableSign = '~';
constexpr uint8_t fontSize = 1;
void drawChar(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, char ch, PixelColor color) {
    if (ch > lastPrintableSign) {
        return;
    }

    for (uint8_t i = 0; i < font[1]; i++) {
        uint8_t line = font[(ch - 0x20) * font[1] + i + 2];

        for (uint8_t j = 0; j < font[0]; j++, line >>= 1) {
            if ((line & 1) != 0) {
                if constexpr (fontSize == 1) {
                    oled.drawPixel(x + i, y + j, color);
                } else {
                    drawFillRectangle(oled, x + i * fontSize, y + j * fontSize, fontSize, fontSize, color);
                }
            }
        }
    }
}

void drawString(OLED_SSD1306<I2C_Handler>& oled, int16_t x, int16_t y, std::string_view str, PixelColor color) {
    int16_t xTmp = x;

    for (auto letter : str) {
        drawChar(oled, xTmp, y, letter, color);
        xTmp += font[1] * fontSize + 1;
    }
}

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

//
// Circle drawing
//
void drawCircle(OLED_SSD1306<I2C_Handler>& oled, int16_t x0, int16_t y0, uint16_t radius, PixelColor color) {
    oled.drawPixel(x0, y0 + radius, color);
    oled.drawPixel(x0, y0 - radius, color);
    oled.drawPixel(x0 + radius, y0, color);
    oled.drawPixel(x0 - radius, y0, color);

    int16_t x{};
    int16_t y = radius;
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        oled.drawPixel(x0 + x, y0 + y, color);
        oled.drawPixel(x0 - x, y0 + y, color);
        oled.drawPixel(x0 + x, y0 - y, color);
        oled.drawPixel(x0 - x, y0 - y, color);

        oled.drawPixel(x0 + y, y0 + x, color);
        oled.drawPixel(x0 - y, y0 + x, color);
        oled.drawPixel(x0 + y, y0 - x, color);
        oled.drawPixel(x0 - y, y0 - x, color);
    }
}
}  // namespace gfx
