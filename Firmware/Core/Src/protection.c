#include "protection.h"

#define PROTECTION_VOLTAGE_LIMIT_RAW    1555u   /*ADC_Raw = Vout / 9.9786  × 4096 / 3.3 + 56*/
#define PROTECTION_CURRENT_LIMIT_RAW    2387u	/*ADC_Raw = Iout / 1.04 × 4096 / 3.3*/


uint8_t Protection_IsFaultDetected(uint16_t VoltageRawValue,
                                   uint16_t CurrentRawValue)
{
    if(VoltageRawValue >= PROTECTION_VOLTAGE_LIMIT_RAW ||
       CurrentRawValue >= PROTECTION_CURRENT_LIMIT_RAW)
    {
    	return 1;
    }

    return 0;
}
