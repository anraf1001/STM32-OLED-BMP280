#pragma once

#include "main.h"

#include "I2C_HandleInterface.hpp"

constexpr uint32_t i2cTimeout = 1000;

class I2C_Handler : public I2C_HandleInterface<I2C_Handler> {
public:
    explicit I2C_Handler(I2C_HandleTypeDef* hi2c)
        : hi2c_{hi2c} {}

    HAL_Status Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size);
    HAL_Status Mem_Write_DMA(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size);
    HAL_Status Mem_Read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size);

private:
    I2C_HandleTypeDef* hi2c_;
};
