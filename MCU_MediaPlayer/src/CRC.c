/***********************************************************************************
*
* @Name 				: CRC.c
* @Author				: Tran Ba Than
* @Created date			: July 9 2024
*
************************************************************************************/

#include "CRC.h"

static uint8_t crc8_table[256];

void init_crc8_table()
{
    uint8_t crc;
    for (uint16_t i = 0; i < 256; i++)
    {
        crc = i;
        for (uint8_t j = 8; j > 0; j--)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc <<= 1;
            }
        }
        crc8_table[i] = crc;
    }
}

uint8_t crc8(const uint8_t *data, uint8_t length)
{
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < length; i++)
    {
        uint8_t byte = data[i];
        uint8_t lookup_index = (crc ^ byte) & 0xFF;
        crc = crc8_table[lookup_index];
    }

    return crc;
}
