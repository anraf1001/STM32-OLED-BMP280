set(UT_SRC
    OLED_SSD1306-ut.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${UT_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-lib gtest_main gmock)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main gmock)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fsanitize=undefined)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
