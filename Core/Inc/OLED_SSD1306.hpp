#pragma once

#include <array>
#include <memory>

#include "I2C_HandleInterface.hpp"

constexpr uint16_t oledI2CAddress = 0x3C;

constexpr uint16_t LCDWIDTH = 128;
constexpr uint16_t LCDHEIGHT = 64;

constexpr uint16_t bufferSize = LCDHEIGHT * LCDWIDTH / 8;

constexpr bool useDMA = true;

enum class PixelColor : uint8_t {
    BLACK,
    WHITE,
    INVERTED
};

class OLED_SSD1306 {
public:
    explicit OLED_SSD1306(std::shared_ptr<I2C_HandleInterface> hi2c);

    void drawPixel(uint16_t x, uint16_t y, PixelColor color);
    void clear(PixelColor color);
    void display();

private:
    void sendCommand(uint8_t command);
    void sendBuffer();

    std::shared_ptr<I2C_HandleInterface> hi2c_;
    std::array<uint8_t, bufferSize> buffer_{};
};
