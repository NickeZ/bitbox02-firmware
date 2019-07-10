/* LICENSE */

#include <stdint.h>

/**
 * Enable the Smart EEPROM
 */
void smarteeprom_enable();

/**
 * Writes 1 byte to SEEPROM_ADDR + offset
 * @param offset Offset in bytes
 * @param value Value to write
 */
void smarteeprom_write8(uint32_t offset, uint8_t value);

/**
 * Writes 2 bytes to SEEPROM_ADDR + offset
 * @param offset Offset in bytes (must be aligned)
 * @param value Value to write
 */
void smarteeprom_write16(uint32_t offset, uint16_t value);

/**
 * Writes 4 byte to SEEPROM_ADDR + offset
 * @param offset Offset in bytes (must be aligned)
 * @param value Value to write
 */
void smarteeprom_write32(uint32_t offset, uint32_t value);

/**
 * Reads 1 byte from SEEPROM_ADDR + offset
 * @param offset Offset in bytes
 * @param value Where to store the value
 */
void smarteeprom_read8(uint32_t offset, uint8_t* value);

/**
 * Reads 1 byte from SEEPROM_ADDR + offset
 * @param offset Offset in bytes
 * @param value Where to store the value
 */
void smarteeprom_read16(uint32_t offset, uint16_t* value);

/**
 * Reads 1 byte from SEEPROM_ADDR + offset
 * @param offset Offset in bytes
 * @param value Where to store the value
 */
void smarteeprom_read32(uint32_t offset, uint32_t* value);
