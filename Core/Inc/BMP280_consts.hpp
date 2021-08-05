#pragma once

#include <cstdint>

namespace bmp280 {
// Mode
constexpr uint8_t SLEEPMODE = 0;
constexpr uint8_t FORCEDMODE = 1;
constexpr uint8_t NORMALMODE = 3;

// Temperature resolution
constexpr uint8_t TEMPERATURE_16BIT = 1;
constexpr uint8_t TEMPERATURE_17BIT = 2;
constexpr uint8_t TEMPERATURE_18BIT = 3;
constexpr uint8_t TEMPERATURE_19BIT = 4;
constexpr uint8_t TEMPERATURE_20BIT = 5;

// Pressure oversampling
constexpr uint8_t ULTRALOWPOWER = 1;
constexpr uint8_t LOWPOWER = 2;
constexpr uint8_t STANDARD = 3;
constexpr uint8_t HIGHRES = 4;
constexpr uint8_t ULTRAHIGHRES = 5;

//
//	Coeffs registers
//
constexpr uint8_t DIG_T1 = 0x88;
constexpr uint8_t DIG_T2 = 0x8A;
constexpr uint8_t DIG_T3 = 0x8C;

constexpr uint8_t DIG_P1 = 0x8E;
constexpr uint8_t DIG_P2 = 0x90;
constexpr uint8_t DIG_P3 = 0x92;
constexpr uint8_t DIG_P4 = 0x94;
constexpr uint8_t DIG_P5 = 0x96;
constexpr uint8_t DIG_P6 = 0x98;
constexpr uint8_t DIG_P7 = 0x9A;
constexpr uint8_t DIG_P8 = 0x9C;
constexpr uint8_t DIG_P9 = 0x9E;

//
//	Registers
//
constexpr uint8_t CHIPID = 0xD0;
constexpr uint8_t VERSION = 0xD1;
constexpr uint8_t SOFTRESET = 0xE0;
constexpr uint8_t CAL26 = 0xE1;  // R calibration stored in 0xE1-0xF0
constexpr uint8_t STATUS = 0xF3;
constexpr uint8_t CONTROL = 0xF4;
constexpr uint8_t CONFIG = 0xF5;
constexpr uint8_t PRESSUREDATA = 0xF7;
constexpr uint8_t TEMPDATA = 0xFA;

//
//	Control bits
//
constexpr uint8_t MEASURING = (1 << 3);  // Conversion in progress
}  // namespace bmp280
