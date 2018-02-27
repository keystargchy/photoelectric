#ifndef _KR_GPIO_H
#define _KR_GPIO_H

#include "windef.h"

typedef enum {
   INPUT  = 0,
   OUTPUT = 1
} GPIO_DIRECTION;

typedef enum {
   LOW  = 0,
   HIGH = 1
} GPIO_VALUE;


typedef enum {
    STATE_OFF = 0,
    STATE_ON = 1
} OUTPUT_STATE_E;

/*
 * @brief: control pin num = type number.
 */
#define CONTROL_PIN_NUM 3

typedef enum {
    TYPE_PE_FRONT = 0,
	TYPE_PE_BEHIND
} INPUT_TYPE_E;



typedef enum {
    PE_FRONT_N = 0,
    PE_BEHIND_N = 1,
    POWER_5V_EN_N = 2,
} OUTPUT_TYPE_E;

#endif
