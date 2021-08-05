#pragma once

enum class HAL_Status : uint8_t {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
};

template <typename T>
class I2C_HandleInterface {
public:
    HAL_Status Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
        return static_cast<T&>(*this).Mem_Write(devAddress, memAddress, memAddSize, data, size);
    }

    HAL_Status Mem_Write_DMA(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
        return static_cast<T&>(*this).Mem_Write_DMA(devAddress, memAddress, memAddSize, data, size);
    }

    HAL_Status Mem_Read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
        return static_cast<T&>(*this).Mem_Read(devAddress, memAddress, memAddSize, data, size);
    }

private:
    I2C_HandleInterface() = default;
    friend T;
};
