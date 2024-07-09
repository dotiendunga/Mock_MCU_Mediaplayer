/***********************************************************************************
*
* @Name 				: S32K144_IT.c
* @Author				: Tran Ba Than
* @Created date			: July 9 2024
*
************************************************************************************/

#ifndef CRC_H_
#define CRC_H_

#include "stdint.h"

void init_crc8_table();
uint8_t crc8(const uint8_t *data, uint8_t length);

#endif /* CRC_H_ */
