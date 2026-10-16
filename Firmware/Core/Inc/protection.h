#ifndef __PROTECTION_H
#define __PROTECTION_H

#include <stdint.h>


uint8_t Protection_IsFaultDetected(uint16_t VoltageRawValue,
                                   uint16_t CurrentRawValue);

#endif
