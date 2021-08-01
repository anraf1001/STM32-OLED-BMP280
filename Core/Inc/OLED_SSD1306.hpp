#pragma once

#include <array>

#include "I2C_HandleInterface.hpp"
#include "OLED_consts.hpp"
#include "PixelColor.hpp"

namespace oled {
constexpr uint16_t oledI2CAddress = 0x3C;
constexpr bool useDMA = true;

constexpr uint16_t LCDWIDTH = 128;
constexpr uint16_t LCDHEIGHT = 64;
constexpr uint16_t bufferSize = LCDHEIGHT * LCDWIDTH / 8;
}  // namespace oled

template <typename T>
class OLED_SSD1306 {
public:
    explicit OLED_SSD1306(I2C_HandleInterface<T>* hi2c);

    void drawPixel(int16_t x, int16_t y, PixelColor color);
    void clear(PixelColor color);
    void display();

private:
    void sendCommand(uint8_t command);
    void sendBuffer();

    uint16_t positionInBuffer(uint16_t x, uint16_t y);
    uint8_t prepareColorByte(uint16_t y);

    I2C_HandleInterface<T>* hi2c_;
    std::array<uint8_t, oled::bufferSize> buffer_{};
};

template <typename T>
OLED_SSD1306<T>::OLED_SSD1306(I2C_HandleInterface<T>* hi2c)
    : hi2c_{hi2c} {
    using namespace oled;

    sendCommand(DISPLAYOFF);

    sendCommand(SETDISPLAYCLOCKDIV);
    sendCommand(0x80);

    sendCommand(LCDHEIGHT - 1);

    sendCommand(SETDISPLAYOFFSET);
    sendCommand(0x0);
    sendCommand(SETSTARTLINE | 0x0);

    sendCommand(CHARGEPUMP);
    sendCommand(0x14);  // Vcc from charge pump

    sendCommand(MEMORYMODE);
    sendCommand(0x00);
    sendCommand(SEGREMAP | 0x1);
    sendCommand(COMSCANDEC);

    sendCommand(SETCOMPINS);
    sendCommand(0x12);  // comPins
    sendCommand(SETCONTRAST);
    sendCommand(0xFF);  // contrast

    sendCommand(SETPRECHARGE);
    sendCommand(0xF1);

    sendCommand(SETVCOMDETECT);  // 0xDB
    sendCommand(0x40);
    sendCommand(DISPLAYALLON_RESUME);  // 0xA4
    sendCommand(NORMALDISPLAY);        // 0xA6
    sendCommand(DEACTIVATE_SCROLL);

    sendCommand(DISPLAYON);  // Main screen turn on
}

template <typename T>
uint16_t OLED_SSD1306<T>::positionInBuffer(uint16_t x, uint16_t y) {
    return static_cast<uint16_t>(x + (y / 8) * oled::LCDWIDTH);
}

template <typename T>
uint8_t OLED_SSD1306<T>::prepareColorByte(uint16_t y) {
    return 1 << (y & 7);
}

template <typename T>
void OLED_SSD1306<T>::sendCommand(uint8_t command) {
    hi2c_->Mem_Write(oled::oledI2CAddress, oled::commandsMemAddress, 1, &command, 1);
}

template <typename T>
void OLED_SSD1306<T>::sendBuffer() {
    if constexpr (oled::useDMA) {
        hi2c_->Mem_Write_DMA(oled::oledI2CAddress, oled::dataMemAddress, 1, buffer_.data(), oled::bufferSize);
    } else {
        hi2c_->Mem_Write(oled::oledI2CAddress, oled::dataMemAddress, 1, buffer_.data(), oled::bufferSize);
    }
}

template <typename T>
void OLED_SSD1306<T>::drawPixel(int16_t x, int16_t y, PixelColor color) {
    if (x > 0 && x < oled::LCDWIDTH && y > 0 && y < oled::LCDHEIGHT) {
        switch (color) {
        case PixelColor::WHITE:
            buffer_[positionInBuffer(x, y)] |= prepareColorByte(y);
            break;

        case PixelColor::BLACK:
            buffer_[positionInBuffer(x, y)] &= ~prepareColorByte(y);
            break;

        case PixelColor::INVERTED:
            buffer_[positionInBuffer(x, y)] ^= prepareColorByte(y);
            break;
        }
    }
}

template <typename T>
void OLED_SSD1306<T>::clear(PixelColor color) {
    switch (color) {
    case PixelColor::WHITE:
        buffer_.fill(0xFF);
        break;

    case PixelColor::BLACK:
        buffer_.fill(0x00);
        break;

    default:
        break;
    }
}

template <typename T>
void OLED_SSD1306<T>::display() {
    using namespace oled;

    sendCommand(PAGEADDR);
    sendCommand(0);     // Page start address
    sendCommand(0xFF);  // Page end (not really, but works here)
    sendCommand(COLUMNADDR);
    sendCommand(0);             // Column start address
    sendCommand(LCDWIDTH - 1);  // Column end address

    sendBuffer();
}
