cmake_minimum_required(VERSION 3.20)
project(blink_f411re LANGUAGES C)

add_executable(blink src/main.c src/startup.c)
set_target_properties(blink PROPERTIES SUFFIX ".elf")

target_compile_options(blink PRIVATE -Wall -Wextra -Wpedantic -Werror -g)
target_link_options(blink PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F411RE.ld
    -nostartfiles
    -Wl,--gc-sections
    -Wl,-Map=$<TARGET_FILE_DIR:blink>/blink.map)

add_custom_command(TARGET blink POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:blink> $<TARGET_FILE_DIR:blink>/blink.bin
    COMMAND arm-none-eabi-size $<TARGET_FILE:blink>
    VERBATIM)
