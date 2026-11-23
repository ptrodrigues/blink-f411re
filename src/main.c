#include <stdint.h>

/* NUCLEO-F411RE: user LED LD2 is on PA5. Clock: default HSI, 16 MHz. */

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RCC_AHB1ENR REG32(0x40023830u)
#define GPIOA_MODER REG32(0x40020000u)
#define GPIOA_ODR   REG32(0x40020014u)

#define SYST_CSR    REG32(0xE000E010u)
#define SYST_RVR    REG32(0xE000E014u)
#define SYST_CVR    REG32(0xE000E018u)

#define LED_PIN     5u
#define HCLK_HZ     16000000u

static volatile uint32_t ticks;

void SysTick_Handler(void)
{
    ticks++;
}

static void delay_ms(uint32_t ms)
{
    const uint32_t start = ticks;
    while ((ticks - start) < ms) {
        __asm volatile("wfi");
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1u << 0);                       /* GPIOAEN: clock for GPIOA */

    /* PA5 as general purpose output (MODER = 01). Keep the other pins
     * untouched: PA13/PA14 are the SWD pins and must stay in AF mode. */
    GPIOA_MODER = (GPIOA_MODER & ~(0x3u << (LED_PIN * 2u))) | (0x1u << (LED_PIN * 2u));

    SYST_RVR = (HCLK_HZ / 1000u) - 1u;              /* 1 ms tick */
    SYST_CVR = 0;
    SYST_CSR = (1u << 2) | (1u << 1) | (1u << 0);   /* HCLK source, interrupt, enable */

    for (;;) {
        GPIOA_ODR ^= (1u << LED_PIN);
        delay_ms(500);
    }
}
