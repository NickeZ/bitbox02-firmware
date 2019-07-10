/* LICENSE */

#include "smarteeprom.h"
#include <amd51j20a.h>
#include <stdint.h>

void smarteeprom_enable() {
    /* 16KiB of (virtual) EEPROM (49152 bytes of flash allocated)
     * One optimal setting is 6 blocks and a page size of 128 (SBLK = 3, PSZ = 5)
     * See other Optimal configurations in section 25.6.8.4.
     * Virtual Sector (SEES)
     * Virtual Page  (SEEP)
     * The whole area is called a virtual sector (or SEES)
     * Wear leveling factor is 288, which means it ca handle 2.88 million writes.
     */
}

void smarteeprom_write8(uint32_t offset, uint8_t value)
{
    volatile uint8_t* p8 = (uint8_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *p8 = value;
}

void smarteeprom_write16(uint32_t offset, uint16_t value)
{
    volatile uint16_t* p16 = (uint16_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *p16 = value;
}

void smarteeprom_write32(uint32_t offset, uint32_t value)
{
    volatile uint32_t* p32 = (uint32_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *p32 = value;
}

void smarteeprom_read8(uint32_t offset, uint8_t* value)
{
    volatile uint8_t* p8 = (uint8_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *value = *p8;
}

void smarteeprom_read16(uint32_t offset, uint16_t* value)
{
    volatile uint8_t* p16 = (uint16_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *value = *p16;
}

void smarteeprom_read32(uint32_t offset, uint32_t* value)
{
    volatile uint8_t* p32 = (uint32_t*)(SEEPROM_ADDR + offset);
    while (NVMCTRL->SEESTAT.bit.BUSY) {
    }
    *value = *p32;
}
