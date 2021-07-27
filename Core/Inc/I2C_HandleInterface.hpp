#pragma once

#include "main.h"

class I2C_HandleInterface {
public:
    virtual ~I2C_HandleInterface() = default;

    virtual HAL_StatusTypeDef Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;
    virtual HAL_StatusTypeDef Mem_Write_DMA(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;
};
