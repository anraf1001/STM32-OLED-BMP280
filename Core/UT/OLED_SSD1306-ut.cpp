#include <gtest/gtest.h>

#include <memory>

#include "I2C_MockHandler.hpp"
#include "OLED_SSD1306.hpp"

using namespace ::testing;

TEST(oledTestSuite, shouldRunMemWriteDuringInit) {
    auto hi2c = std::make_shared<NiceMock<I2C_MockHandler>>();
    EXPECT_CALL(*hi2c, Mem_Write(0x3C, 0x00, 1, NotNull(), 1)).Times(AtLeast(1));
    OLED_SSD1306 oled{hi2c};
}
