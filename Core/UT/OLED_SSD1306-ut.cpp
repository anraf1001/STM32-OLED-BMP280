#include <gtest/gtest.h>

#include <memory>

#include "I2C_MockHandler.hpp"
#include "OLED_SSD1306.hpp"
#include "OLED_consts.hpp"

using namespace ::testing;

TEST(oledTestSuite, shouldRunMemWriteDuringInit) {
    constexpr uint16_t bytesInCom = 1;
    constexpr int numOfComsToSend = 25;

    I2C_MockHandler hi2c;
    EXPECT_CALL(hi2c, Mem_Write(oled::oledI2CAddress, oled::commandsMemAddress, bytesInCom, NotNull(), bytesInCom)).Times(numOfComsToSend);

    OLED_SSD1306 oled{&hi2c};
}

TEST(oledTestSuite, shouldRunMemWriteDuringDisplaying) {
    constexpr uint16_t bytesInCom = 1;
    constexpr int numOfComsToSend = 6;

    NiceMock<I2C_MockHandler> hi2c;
    OLED_SSD1306 oled{&hi2c};

    EXPECT_CALL(hi2c, Mem_Write(oled::oledI2CAddress, oled::commandsMemAddress, bytesInCom, NotNull(), bytesInCom)).Times(numOfComsToSend);
    if constexpr (oled::useDMA) {
        EXPECT_CALL(hi2c, Mem_Write_DMA(oled::oledI2CAddress, oled::dataMemAddress, bytesInCom, NotNull(), oled::bufferSize)).Times(1);
    } else {
        EXPECT_CALL(hi2c, Mem_Write(oled::oledI2CAddress, oled::dataMemAddress, bytesInCom, NotNull(), oled::bufferSize)).Times(1);
    }

    oled.display();
}
