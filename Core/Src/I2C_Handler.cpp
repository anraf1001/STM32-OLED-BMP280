#include "I2C_Handler.hpp"

HAL_Status I2C_Handler::Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
    if (!hi2c_) {
        return HAL_Status::HAL_ERROR;
    }
    return static_cast<HAL_Status>(HAL_I2C_Mem_Write(hi2c_, devAddress << 1, memAddress, memAddSize, data, size, i2cTimeout));
}

HAL_Status I2C_Handler::Mem_Write_DMA(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
    if (!hi2c_) {
        return HAL_Status::HAL_ERROR;
    }

    if (hi2c_->hdmatx->State != HAL_DMA_STATE_READY) {
        return HAL_Status::HAL_BUSY;
    }

    return static_cast<HAL_Status>(HAL_I2C_Mem_Write_DMA(hi2c_, devAddress << 1, memAddress, memAddSize, data, size));
}
