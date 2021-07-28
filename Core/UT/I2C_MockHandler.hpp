#pragma once

#include <gmock/gmock.h>

#include "I2C_HandleInterface.hpp"

class I2C_MockHandler : public I2C_HandleInterface {
public:
    MOCK_METHOD(HAL_Status, Mem_Write, (uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t), (override));
    MOCK_METHOD(HAL_Status, Mem_Write_DMA, (uint16_t, uint16_t, uint16_t, uint8_t*, uint16_t), (override));
};
