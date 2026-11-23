#include <stdint.h>

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

typedef void (*isr_t)(void);
typedef union {
    isr_t handler;
    void *stack;
} vector_t;

/* Cortex-M core vectors only; no device interrupts are used. */
__attribute__((section(".isr_vector"), used))
const vector_t vector_table[16] = {
    { .stack = &_estack },
    { .handler = Reset_Handler },
    { .handler = NMI_Handler },
    { .handler = HardFault_Handler },
    { .handler = MemManage_Handler },
    { .handler = BusFault_Handler },
    { .handler = UsageFault_Handler },
    { .handler = 0 }, { .handler = 0 }, { .handler = 0 }, { .handler = 0 },
    { .handler = SVC_Handler },
    { .handler = DebugMon_Handler },
    { .handler = 0 },
    { .handler = PendSV_Handler },
    { .handler = SysTick_Handler },
};

void Reset_Handler(void)
{
    /* Enable the FPU (CP10/CP11 full access): the build uses -mfloat-abi=hard. */
    *(volatile uint32_t *)0xE000ED88u |= (0xFu << 20);
    __asm volatile("dsb");
    __asm volatile("isb");

    const uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata;) {
        *dst++ = *src++;
    }
    for (uint32_t *dst = &_sbss; dst < &_ebss;) {
        *dst++ = 0;
    }

    (void)main();
    for (;;) {
    }
}

void Default_Handler(void)
{
    for (;;) {
    }
}
