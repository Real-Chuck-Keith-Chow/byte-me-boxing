#include <stdint.h>

// -----------------------------------------------------------------------------
// RP2350 Memory Map & Register Base Addresses
// -----------------------------------------------------------------------------
#define RESETS_BASE       0x40020000
#define IO_BANK0_BASE     0x40028000
#define PADS_BANK0_BASE   0x40038000
#define SIO_BASE          0x20000000

// Hardware register layout definitions
#define RESETS_RESET      *(volatile uint32_t *)(RESETS_BASE + 0x00)
#define RESETS_RESET_CLR  *(volatile uint32_t *)(RESETS_BASE + 0x04)
#define RESETS_DONE       *(volatile uint32_t *)(RESETS_BASE + 0x08)

#define PADS_BANK0_GP15   *(volatile uint32_t *)(PADS_BANK0_BASE + 0x04 + (15 * 4))
#define IO_BANK0_GP15_CTRL *(volatile uint32_t *)(IO_BANK0_BASE + 0x04 + (15 * 8))

#define SIO_GPIO_OE_SET   *(volatile uint32_t *)(SIO_BASE + 0x038)
#define SIO_GPIO_OUT_SET  *(volatile uint32_t *)(SIO_BASE + 0x018)
#define SIO_GPIO_OUT_CLR  *(volatile uint32_t *)(SIO_BASE + 0x01c)

// ARM Cortex-M SysTick Registers (for accurate delays)
#define SYSTICK_CTRL      *(volatile uint32_t *)(0xE000E010)
#define SYSTICK_LOAD      *(volatile uint32_t *)(0xE000E014)
#define SYSTICK_VAL       *(volatile uint32_t *)(0xE000E018)

#define LED_PIN 15

// Simple delay function using CPU spin loop
void delay_ms(uint32_t ms) {
    // RP2350 default clock speed is ~150 MHz
    for (volatile uint32_t i = 0; i < ms * 12000; i++) {
        __asm volatile("nop");
    }
}

int main(void) {
    // -------------------------------------------------------------------------
    // 1. Release RESETS for IO_BANK0 and PADS_BANK0
    // Bit 6 = io_bank0, Bit 9 = pads_bank0
    // -------------------------------------------------------------------------
    RESETS_RESET_CLR = (1 << 6) | (1 << 9);
    while ((RESETS_DONE & ((1 << 6) | (1 << 9))) != ((1 << 6) | (1 << 9))) {
        // Wait for peripherals to un-reset
    }

    // -------------------------------------------------------------------------
    // 2. Configure Pin Pad Settings (PADS_BANK0)
    // Enable output drive and input enable bits (Bits 6 and 7)
    // -------------------------------------------------------------------------
    PADS_BANK0_GP15 = (1 << 6) | (1 << 7);

    // -------------------------------------------------------------------------
    // 3. Configure Pin Multiplexer (IO_BANK0)
    // Select Function 5 (SIO - Software Control)
    // -------------------------------------------------------------------------
    IO_BANK0_GP15_CTRL = 5; // 5 = SIO function

    // -------------------------------------------------------------------------
    // 4. Set Pin Direction to Output via SIO
    // -------------------------------------------------------------------------
    SIO_GPIO_OE_SET = (1 << LED_PIN);

    // Initial 2-second boot delay
    delay_ms(2000);

    // -------------------------------------------------------------------------
    // 5. Main Loop
    // -------------------------------------------------------------------------
    while (1) {
        // Turn LED ON (Set Pin 15 High)
        SIO_GPIO_OUT_SET = (1 << LED_PIN);
        delay_ms(1000);

        // Turn LED OFF (Clear Pin 15 Low)
        SIO_GPIO_OUT_CLR = (1 << LED_PIN);
        delay_ms(1000);
    }

    return 0;
}