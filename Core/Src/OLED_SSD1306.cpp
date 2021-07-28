#include "OLED_SSD1306.hpp"

constexpr uint16_t commandsMemAddress = 0x00;
constexpr uint16_t dataMemAddress = 0x40;

// Registers
constexpr uint16_t MEMORYMODE = 0x20;           ///< See datasheet
constexpr uint16_t COLUMNADDR = 0x21;           ///< See datasheet
constexpr uint16_t PAGEADDR = 0x22;             ///< See datasheet
constexpr uint16_t SETCONTRAST = 0x81;          ///< See datasheet
constexpr uint16_t CHARGEPUMP = 0x8D;           ///< See datasheet
constexpr uint16_t SEGREMAP = 0xA0;             ///< See datasheet
constexpr uint16_t DISPLAYALLON_RESUME = 0xA4;  ///< See datasheet
constexpr uint16_t DISPLAYALLON = 0xA5;         ///< Not currently used
constexpr uint16_t NORMALDISPLAY = 0xA6;        ///< See datasheet
constexpr uint16_t INVERTDISPLAY = 0xA7;        ///< See datasheet
constexpr uint16_t SETMULTIPLEX = 0xA8;         ///< See datasheet
constexpr uint16_t DISPLAYOFF = 0xAE;           ///< See datasheet
constexpr uint16_t DISPLAYON = 0xAF;            ///< See datasheet
constexpr uint16_t COMSCANINC = 0xC0;           ///< Not currently used
constexpr uint16_t COMSCANDEC = 0xC8;           ///< See datasheet
constexpr uint16_t SETDISPLAYOFFSET = 0xD3;     ///< See datasheet
constexpr uint16_t SETDISPLAYCLOCKDIV = 0xD5;   ///< See datasheet
constexpr uint16_t SETPRECHARGE = 0xD9;         ///< See datasheet
constexpr uint16_t SETCOMPINS = 0xDA;           ///< See datasheet
constexpr uint16_t SETVCOMDETECT = 0xDB;        ///< See datasheet

constexpr uint16_t SETLOWCOLUMN = 0x00;   ///< Not currently used
constexpr uint16_t SETHIGHCOLUMN = 0x10;  ///< Not currently used
constexpr uint16_t SETSTARTLINE = 0x40;   ///< See datasheet

constexpr uint16_t EXTERNALVCC = 0x01;   ///< External display voltage source
constexpr uint16_t SWITCHCAPVCC = 0x02;  ///< Gen. display voltage from 3.3V

constexpr uint16_t RIGHT_HORIZONTAL_SCROLL = 0x26;               ///< Init rt scroll
constexpr uint16_t LEFT_HORIZONTAL_SCROLL = 0x27;                ///< Init left scroll
constexpr uint16_t VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;  ///< Init diag scroll
constexpr uint16_t VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;   ///< Init diag scroll
constexpr uint16_t DEACTIVATE_SCROLL = 0x2E;                     ///< Stop scroll
constexpr uint16_t ACTIVATE_SCROLL = 0x2F;                       ///< Start scroll
constexpr uint16_t SET_VERTICAL_SCROLL_AREA = 0xA3;              ///< Set scroll range

constexpr uint16_t BUFFER_SIZE = LCDHEIGHT * LCDWIDTH / 8;

void OLED_SSD1306::sendCommand(uint8_t command) {
    [[maybe_unused]]auto status = hi2c_->Mem_Write(oledI2CAddress, commandsMemAddress, 1, &command, 1);
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
