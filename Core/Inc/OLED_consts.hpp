#pragma once

namespace oled {
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
}  // namespace oled
