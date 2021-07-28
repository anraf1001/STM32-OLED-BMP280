#pragma once

enum class HAL_Status : uint8_t {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
};

class I2C_HandleInterface {
public:
    virtual ~I2C_HandleInterface() = default;

    virtual HAL_Status Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;
    virtual HAL_Status Mem_Write_DMA(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;
};
