#include "infrared_sensor.h"
#include "gpio.h"

uint8_t ia_flag = FALSE;

void ia_init(void)
{
    
}

uint8_t ia_flag_get(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
        return TRUE;
    else
        return FALSE;
}
