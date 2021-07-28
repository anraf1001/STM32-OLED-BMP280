#include "OLED_SSD1306.hpp"

using namespace oled;

void OLED_SSD1306::sendCommand(uint8_t command) {
    hi2c_->Mem_Write(oledI2CAddress, commandsMemAddress, 1, &command, 1);
}

void OLED_SSD1306::sendBuffer() {
    if constexpr (useDMA) {
        hi2c_->Mem_Write_DMA(oledI2CAddress, dataMemAddress, 1, buffer_.data(), bufferSize);
    } else {
        hi2c_->Mem_Write(oledI2CAddress, dataMemAddress, 1, buffer_.data(), bufferSize);
    }
}

OLED_SSD1306::OLED_SSD1306(std::shared_ptr<I2C_HandleInterface> hi2c)
    : hi2c_{hi2c} {
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

uint16_t positionInBuffer(uint16_t x, uint16_t y) {
    return static_cast<uint16_t>(x + (y / 8) * LCDWIDTH);
}

uint8_t prepareColorByte(uint16_t y, bool isBlack = false) {
    uint8_t colorByte = 1 << (y & 7);
    return isBlack ? ~colorByte : colorByte;
}

void OLED_SSD1306::drawPixel(uint16_t x, uint16_t y, PixelColor color) {
    if (x > 0 && x < LCDWIDTH && y > 0 && y < LCDHEIGHT) {
        switch (color) {
        case PixelColor::WHITE:
            buffer_[positionInBuffer(x, y)] |= prepareColorByte(y);
            break;

        case PixelColor::BLACK:
            buffer_[positionInBuffer(x, y)] &= prepareColorByte(y, true);
            break;

        case PixelColor::INVERTED:
            buffer_[positionInBuffer(x, y)] ^= prepareColorByte(y);
            break;
        }
    }
}

void OLED_SSD1306::clear(PixelColor color) {
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

void OLED_SSD1306::display() {
    sendCommand(PAGEADDR);
    sendCommand(0);     // Page start address
    sendCommand(0xFF);  // Page end (not really, but works here)
    sendCommand(COLUMNADDR);
    sendCommand(0);             // Column start address
    sendCommand(LCDWIDTH - 1);  // Column end address

    sendBuffer();
}
