# blink-f411re

Bare-metal LED blink for the NUCLEO-F411RE (LD2 on PA5), used to test the
`arm-gcc` toolchain image. No HAL, no CubeMX: startup code, linker script and
register-level GPIO/SysTick only. Builds for Cortex-M4F with the hard-float ABI.

## Build (in the arm-gcc container)

    docker run --rm -v "$PWD":/work -u $(id -u):$(id -g) arm-gcc:dev \
        cmake --workflow --preset firmware

Output: `build/firmware/blink.elf` and `build/firmware/blink.bin`.

## Flash

Drag and drop `blink.bin` onto the NODE_F411RE drive that appears when the
Nucleo is plugged in over USB, or use OpenOCD:

    openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
        -c "program build/firmware/blink.elf verify reset exit"

LD2 should blink twice per second.
