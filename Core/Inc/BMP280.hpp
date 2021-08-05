#pragma once

#include <cstdint>
#include <utility>

#include "BMP280_consts.hpp"
#include "I2C_HandleInterface.hpp"

template <typename T>
class BMP280 {
public:
    BMP280(I2C_HandleInterface<T>* hi2c, uint16_t address);

    float readTemperature();
    std::pair<float, float> readPressureAndTemperature();

    bool isChipIDValid() const;

    void setTemperatureOversampling(uint8_t tempOversamp);
    void setPressureOversampling(uint8_t pressOversamp);
    void setMode(uint8_t mode);

private:
    uint8_t read8Bits(uint8_t registerAddress);
    uint16_t read16Bits(uint8_t registerAddress);
    uint32_t readMeasurement(uint8_t registerAddress);
    void write8Bits(uint8_t registerAddress, uint8_t value);

    int32_t readRawTemperature();
    int32_t readRawPressure();

    I2C_HandleInterface<T>* hi2c_;
    const uint16_t address_;

    int32_t tFine_;

    uint16_t t1_;
    int16_t t2_;
    int16_t t3_;

    uint16_t p1_;
    int16_t p2_;
    int16_t p3_;
    int16_t p4_;
    int16_t p5_;
    int16_t p6_;
    int16_t p7_;
    int16_t p8_;
    int16_t p9_;
};

template <typename T>
BMP280<T>::BMP280(I2C_HandleInterface<T>* hi2c, uint16_t address)
    : hi2c_{hi2c}, address_{address} {
    t1_ = read16Bits(bmp280::DIG_T1);
    t2_ = read16Bits(bmp280::DIG_T2);
    t3_ = read16Bits(bmp280::DIG_T3);

    p1_ = read16Bits(bmp280::DIG_P1);
    p2_ = read16Bits(bmp280::DIG_P2);
    p3_ = read16Bits(bmp280::DIG_P3);
    p4_ = read16Bits(bmp280::DIG_P4);
    p5_ = read16Bits(bmp280::DIG_P5);
    p6_ = read16Bits(bmp280::DIG_P6);
    p7_ = read16Bits(bmp280::DIG_P7);
    p8_ = read16Bits(bmp280::DIG_P8);
    p9_ = read16Bits(bmp280::DIG_P9);

    setTemperatureOversampling(bmp280::TEMPERATURE_20BIT);
    setPressureOversampling(bmp280::ULTRAHIGHRES);
    setMode(bmp280::NORMALMODE);
}

template <typename T>
uint8_t BMP280<T>::read8Bits(uint8_t registerAddress) {
    uint8_t value;
    hi2c_->Mem_Read(address_, registerAddress, 1, &value, 1);
    return value;
}

template <typename T>
uint16_t BMP280<T>::read16Bits(uint8_t registerAddress) {
    uint8_t value[2];
    hi2c_->Mem_Read(address_, registerAddress, 1, value, 2);
    return (value[1] << 8) | value[0];
}

template <typename T>
uint32_t BMP280<T>::readMeasurement(uint8_t registerAddress) {
    uint8_t value[3];
    hi2c_->Mem_Read(address_, registerAddress, 1, value, 3);
    return (value[0] << 16) | (value[1] << 8) | value[2];
}

template <typename T>
void BMP280<T>::write8Bits(uint8_t registerAddress, uint8_t value) {
    hi2c_->Mem_Write(address_, registerAddress, 1, &value, 1);
}

template <typename T>
bool BMP280<T>::isChipIDValid() const {
    constexpr uint8_t correctChipID = 0x58;
    return read8Bits(bmp280::CHIPID) == correctChipID;
}

template <typename T>
void BMP280<T>::setTemperatureOversampling(uint8_t tempOversamp) {
    if (tempOversamp > 5) {
        tempOversamp = 5;
    }

    auto controlByte = read8Bits(bmp280::CONTROL);
    controlByte &= 0x1F;  // 000x xxxx
    controlByte |= tempOversamp << 5;

    write8Bits(bmp280::CONTROL, controlByte);
}

template <typename T>
void BMP280<T>::setPressureOversampling(uint8_t pressOversamp) {
    if (pressOversamp > 5) {
        pressOversamp = 5;
    }

    auto controlByte = read8Bits(bmp280::CONTROL);
    controlByte &= 0xE3;  // xxx0 00xx
    controlByte |= pressOversamp << 2;

    write8Bits(bmp280::CONTROL, controlByte);
}

template <typename T>
void BMP280<T>::setMode(uint8_t mode) {
    if (mode > 3) {
        mode = 3;
    }

    auto controlByte = read8Bits(bmp280::CONTROL);
    controlByte &= 0xFC;  //xxxx xx00
    controlByte |= mode;

    write8Bits(bmp280::CONTROL, controlByte);
}

template <typename T>
int32_t BMP280<T>::readRawTemperature() {
    return readMeasurement(bmp280::TEMPDATA) >> 4;
}

template <typename T>
int32_t BMP280<T>::readRawPressure() {
    return readMeasurement(bmp280::PRESSUREDATA) >> 4;
}

template <typename T>
float BMP280<T>::readTemperature() {
    int32_t tempFromADC = readRawTemperature();

    int32_t var1 = ((((tempFromADC >> 3) - (((int32_t)t1_) << 1))) * ((int32_t)t2_)) >> 11;
    int32_t var2 = (((((tempFromADC >> 4) - (int32_t)t1_) * ((tempFromADC >> 4) - (int32_t)t1_)) >> 12) * (int32_t)t3_) >> 14;
    tFine_ = var1 + var2;

    return ((tFine_ * 5 + 128) >> 8) / 100.0f;
}
template <typename T>
std::pair<float, float> BMP280<T>::readPressureAndTemperature() {
    float temperature = readTemperature();

    int32_t var1 = (((int32_t)tFine_) >> 1) - (int32_t)64000;
    int32_t var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)p6_);
    var2 += ((var1 * ((int32_t)p5_)) << 1);
    var2 = (var2 >> 2) + (((int32_t)p4_) << 16);
    var1 = (((p3_ * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)p2_) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)p1_)) >> 15);

    if (var1 == 0) {
        return {temperature, -1};
    }

    int32_t pressFromADC = readRawPressure();
    uint32_t p = (((int32_t)(((int32_t)1048576) - pressFromADC) - (var2 >> 12))) * 3125;

    if (p < 0x80000000) {
        p = (p << 1) / (uint32_t)var1;
    } else {
        p = (p / (uint32_t)var1) * 2;
    }

    var1 = (((int32_t)p9_) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(p >> 2)) * ((int32_t)p8_)) >> 13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + p7_) >> 4));

    return {temperature, p / 100.0f};
}
